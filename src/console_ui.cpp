/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "console_ui.h"
#include "ps5_pngdec.hpp"
#include "auto_covers.h"
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
extern "C" int sceSysmoduleLoadModule(uint16_t);
namespace {
const char* ids[]={"nes","snes","n64","gb","gbc","gba","genesis","segacd","x32","saturn","psx","atari2600","atari7800","lynx","jaguar","pce","arcade","amiga","c64"};
struct Image {char path[1100]; unsigned w,h; unsigned char* pixels; int system,index; bool reported;};
Image cache[7]{};
void cover_error(Image& im,const char* stage,int value){
 if(im.reported)return;im.reported=true;
 FILE* file=fopen("/app0/cover-load-errors.log","a");
 if(file){fprintf(file,"glass05 %s %d %s\n",stage,value,im.path);fclose(file);}
}
// VideoOut colors are ABGR (red in the low byte).
constexpr uint32_t navy=0xff64301a, blue=0xffc87814, gold=0xff22a6de;
void styled(GGSurface s,int x,int y,const char* text,int scale,uint32_t color){
 gg_draw_text(s,x+2,y+3,text,scale,0xffded8d3);
 gg_draw_text(s,x,y,text,scale,color);
}
void centered(GGSurface s,int y,const char* text,int scale,uint32_t color){
 int width=(int)strlen(text)*6*scale-scale;
 styled(s,(1920-width)/2,y,text,scale,color);
}
const char* system_names[]={"NINTENDO ENTERTAINMENT SYSTEM","SUPER NINTENDO","NINTENDO 64","GAME BOY","GAME BOY COLOR","GAME BOY ADVANCE","SEGA GENESIS","SEGA CD","SEGA 32X","SEGA SATURN","SONY PLAYSTATION","ATARI 2600","ATARI 7800","ATARI LYNX","ATARI JAGUAR","PC ENGINE","ARCADE","COMMODORE AMIGA","COMMODORE 64"};
const char* default_cores[]={"fceumm_libretro.so","snes9x_libretro.so","parallel_n64_libretro.so","gambatte_libretro.so","gambatte_libretro.so","mgba_libretro.so","genesis_plus_gx_libretro.so","picodrive_libretro.so","picodrive_libretro.so","yabause_libretro.so","pcsx_rearmed_libretro.so","stella2023_libretro.so","prosystem_libretro.so","handy_libretro.so","virtualjaguar_libretro.so","mednafen_pce_fast_libretro.so","fbneo_libretro.so","puae_libretro.so","vice_x64_libretro.so"};
const char* core_names[]={"FCEUMM","SNES9X","PARALLEL N64","GAMBATTE","GAMBATTE","MGBA","GENESIS PLUS GX","PICODRIVE","PICODRIVE","YABAUSE","PCSX REARMED","STELLA 2023","PROSYSTEM","HANDY","VIRTUAL JAGUAR","BEETLE PCE FAST","FINALBURN NEO","PUAE","VICE X64"};
void rect(int x,int y,int w,int h,uint32_t c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)if(xx>=0&&xx<1920&&yy>=0&&yy<1080)gg_platform_put_pixel(xx,yy,c);}
void line(int x0,int y0,int x1,int y1,uint32_t c){
 int dx=abs(x1-x0),sx=x0<x1?1:-1,dy=-abs(y1-y0),sy=y0<y1?1:-1,err=dx+dy;
 for(;;){rect(x0-1,y0-1,3,3,c);if(x0==x1&&y0==y1)break;int e=2*err;if(e>=dy){err+=dy;x0+=sx;}if(e<=dx){err+=dx;y0+=sy;}}
}
void buttons(int x,int y){
 uint32_t green=0xff80a522,red=0xff5254df,pink=0xffaa62c9;
 line(x,y+17,x+15,y-12,green);line(x+15,y-12,x+30,y+17,green);line(x+30,y+17,x,y+17,green);
 int cx=x+75,cy=y+3;for(int yy=-16;yy<=16;yy++)for(int xx=-16;xx<=16;xx++){int d=xx*xx+yy*yy;if(d>=196&&d<=256)rect(cx+xx,cy+yy,1,1,red);}
 line(x+120,y-12,x+148,y+16,blue);line(x+148,y-12,x+120,y+16,blue);
 line(x+180,y-12,x+208,y-12,pink);line(x+208,y-12,x+208,y+16,pink);line(x+208,y+16,x+180,y+16,pink);line(x+180,y+16,x+180,y-12,pink);
}
void bar_edge(int y){
 rect(0,y,1920,2,0xffe8e2dd);rect(0,y+2,1920,3,navy);
 rect(0,y+5,640,3,blue);rect(640,y+5,640,3,gold);rect(1280,y+5,640,3,blue);
}
Image* image(int system,int index,const char* title){
 Image& im=cache[index%7];char clean[512];unsigned n=0;
 for(;title[n]&&n+1<sizeof(clean);n++)clean[n]=strchr("&*/:<>?\\|",title[n])?'_':title[n];clean[n]=0;
 char path[1100];snprintf(path,sizeof(path),"/app0/boxarts/%s/%s.png",ids[system],clean);
 if(!strcmp(im.path,path)&&(im.pixels||access(path,F_OK)!=0))return &im;
 free(im.pixels);im.pixels=nullptr;im.system=system;im.index=index;if(strcmp(im.path,path))im.reported=false;im.w=im.h=0;snprintf(im.path,sizeof(im.path),"%s",path);
 FILE* f=fopen(path,"rb");if(!f){gg_cover_request(system,title,path);return &im;}
 if(fseek(f,0,SEEK_END)){fclose(f);return &im;}long size=ftell(f);
 if(size<=0||size>8*1024*1024||fseek(f,0,SEEK_SET)){fclose(f);return &im;}
 void* png=malloc(size);if(!png){cover_error(im,"png-memory",(int)size);fclose(f);return &im;}
 bool ok=fread(png,1,size,f)==(size_t)size;fclose(f);
 ScePngDecParseParam parse{png,(uint32_t)size,0};ScePngDecImageInfo info{};
 int parsed=ok?scePngDecParseHeader(&parse,&info):-1;
 if(parsed<0||!info.image_width||!info.image_height||info.image_width>2048||info.image_height>2048){cover_error(im,"png-header-or-size",parsed);free(png);return &im;}
 ScePngDecCreateParam create{sizeof(create),info.bit_depth>8?1u:0u,info.image_width};
 int worksize=scePngDecQueryMemorySize(&create);void* handle=nullptr;
 if(worksize<=0||worksize>16*1024*1024){cover_error(im,"decoder-work-size",worksize);free(png);return &im;}
 void* work=malloc(worksize);
 int created=work?scePngDecCreate(&create,work,worksize,&handle):-1;
 if(created<0){cover_error(im,work?"decoder-create":"decoder-memory",created);free(work);free(png);return &im;}
 unsigned bytes=info.image_width*info.image_height*4;im.pixels=(unsigned char*)malloc(bytes);
 if(im.pixels){ScePngDecDecodeParam decode{png,im.pixels,(uint32_t)size,bytes,1,255,info.image_width*4};ScePngDecImageInfo out{};int decoded=scePngDecDecode(handle,&decode,&out);if(decoded<0){cover_error(im,"decode",decoded);free(im.pixels);im.pixels=nullptr;}else {im.w=info.image_width;im.h=info.image_height;}}
 if(!im.pixels)cover_error(im,"texture-memory",(int)bytes);
 scePngDecDelete(handle);free(work);free(png);
 // Keep a bounded display-sized texture, not a full-resolution scan, in cache.
 if(im.pixels&&(im.w>512||im.h>768)){
  unsigned w=im.w,h=im.h;
  if(w>512){h=(unsigned)((uint64_t)h*512/w);w=512;}
  if(h>768){w=(unsigned)((uint64_t)w*768/h);h=768;}
  if(!w)w=1;if(!h)h=1;
  unsigned char* small=(unsigned char*)malloc((size_t)w*h*4);
  if(small){for(unsigned y=0;y<h;y++)for(unsigned x=0;x<w;x++){
   const unsigned char* source=im.pixels+((size_t)(y*im.h/h)*im.w+x*im.w/w)*4;
   memcpy(small+((size_t)y*w+x)*4,source,4);
  }free(im.pixels);im.pixels=small;im.w=w;im.h=h;}
 }
 return &im;
}
/* Inverse perspective mapping of a plane rotated about its vertical axis.
 * X=f*(cx+cos(a)*u)/(z+sin(a)*u); Y=f*v/(z+sin(a)*u). */
uint32_t mix(uint32_t front,uint32_t back,unsigned alpha){
 unsigned inv=255-alpha;
 unsigned r=(((front>>16)&255)*alpha+((back>>16)&255)*inv)/255;
 unsigned g=(((front>>8)&255)*alpha+((back>>8)&255)*inv)/255;
 unsigned b=((front&255)*alpha+(back&255)*inv)/255;
 return 0xff000000u|(r<<16)|(g<<8)|b;
}
uint32_t backdrop(int x,int y){
 unsigned t=(unsigned)y*255/1080;
 uint32_t color=mix(0xff83c5f2,0xffb9e2ff,t);
 // Broad window light across the frosted glass; no noisy texture.
 int distance=x-1100;if(distance<0)distance=-distance;
 unsigned glow=distance<1100?(unsigned)(1100-distance)*42/1100:0;
 color=mix(0xffffffff,color,glow);
 if(y>=550){unsigned floor=(unsigned)(y-550)*255/530;color=mix(0xff9ad3f7,color,floor);}
 // VideoOut uses red in the low byte; backdrop literals above are RGB.
 return (color&0xff00ff00u)|((color&0xffu)<<16)|((color>>16)&0xffu);
}
// Small fixed cache keeps the new background out of the PNG allocator's heap.
static uint32_t background[480*270];
static bool background_ready=false;
uint32_t background_at(int x,int y){return background[(size_t)(y/4)*480+x/4];}
void draw_background(){
 if(!background_ready){
  for(int y=0;y<270;y++)for(int x=0;x<480;x++){
   uint32_t c=backdrop(x*4,y*4);if(y*4<176||y*4>=866)c=0xffffffff;
   background[(size_t)y*480+x]=c;
  }background_ready=true;
 }
 for(int y=0;y<1080;y++)for(int x=0;x<1920;x++)gg_platform_put_pixel(x,y,background_at(x,y));
}
void cover(Image* im,float offset,bool active,bool reflection){
 const float f=1050,half=172,h=420,cx=offset*330,z=1050+((offset<0?-offset:offset)*190);
 float sine=offset<-0.1f?0.65f:offset>0.1f?-0.65f:0;float cosine=sine?0.76f:1;
 for(int x=120;x<1800;x++){
  float screen=(float)x-960;float denominator=f*cosine-screen*sine;
  if(denominator<1)continue;float u=(screen*z-f*cx)/denominator;
  if(u < -half||u>half)continue;float depth=z+sine*u;
  int height=(int)(f*h/depth),top=440-height/2;
  int rows=reflection?height/2:height;
  for(int row=0;row<rows;row++){
   int y=reflection?top+height+5+row:top+row;
   if(y<0||y>=860)continue;
   int source_y=reflection?height-1-row*2:row;
   uint32_t color=active?0xffb68748:0xff91a6b1;
   if(source_y>4&&source_y<height-4&&u>-half+4&&u<half-4){
    color=0xffc0d0d8;
    if(im&&im->pixels){unsigned tx=(unsigned)((u+half)/(2*half)*im->w);unsigned ty=(unsigned)((float)source_y/height*im->h);if(tx>=im->w)tx=im->w-1;if(ty>=im->h)ty=im->h-1;const unsigned char* p=im->pixels+((size_t)ty*im->w+tx)*4;color=0xff000000u|((uint32_t)p[0]<<16)|((uint32_t)p[1]<<8)|p[2];}
   }
   if(reflection){unsigned opacity=(unsigned)(rows-row)*125/(unsigned)rows;color=mix(color,background_at(x,y),opacity);}
   gg_platform_put_pixel(x,y,color);
  }
 }
}
}
extern "C" void gg_draw_console_browser(GGSurface s,int system,int selected,const GGGameList* list,int status){
 static bool decoder=false;static int lastsystem=-1,lastselected=0;static float shift=0;
 if(!decoder){sceSysmoduleLoadModule(0x008c);decoder=true;}
 if(system!=lastsystem){shift=0;lastsystem=system;lastselected=selected;}
 if(selected!=lastselected){int delta=selected-lastselected;if(delta>1)delta=-1;if(delta<-1)delta=1;shift+=(float)delta;lastselected=selected;}
 shift*=0.76f;if(shift<0.005f&&shift>-0.005f)shift=0;
 draw_background();
 bar_edge(168);
 buttons(72,63);buttons(1640,63);
 const int hx=(1920-((10+1+8+1+3)*36-6))/2;
 styled(s,hx,37,"GOLDENGAMES",6,blue);
 styled(s,hx+11*36,37,"RETROHUB",6,navy);
 styled(s,hx+20*36,37,"PRO",6,gold);
 centered(s,94,system_names[system],4,blue);
 centered(s,137,"VERSION 0.1.0",2,navy);
 // Evict off-screen textures before decoding the next visible cover.
 for(Image& im:cache)if(im.pixels&&(im.system!=system||im.index<selected-2||im.index>selected+2)){
  free(im.pixels);im.pixels=nullptr;im.path[0]=0;im.w=im.h=0;
 }
 const int order[]={-2,2,-1,1,0};
 for(int pass=0;pass<2;pass++)for(int d:order){int idx=selected+d;if(list&&idx>=0&&idx<list->count)cover(image(system,idx,list->games[idx].title),(float)d+shift,d==0,pass==0);}
 bar_edge(866);
 if(list&&list->count){char title[65];snprintf(title,sizeof(title),"%.64s",list->games[selected].title);for(char* p=title;*p;p++)*p=(char)toupper((unsigned char)*p);centered(s,892,title,strlen(title)>50?4:strlen(title)>40?5:6,navy);
 const char* core=list->games[selected].core[0]?list->games[selected].core:default_cores[system];
 char emulator[96];const char* friendly=nullptr;for(int i=0;i<19;i++)if(!strcmp(core,default_cores[i])){friendly=core_names[i];break;}
 if(friendly)snprintf(emulator,sizeof(emulator),"%s",friendly);else {const char* base=strrchr(core,'/');snprintf(emulator,sizeof(emulator),"%.80s",base?base+1:core);char* suffix=strstr(emulator,"_libretro");if(suffix)*suffix=0;for(char* p=emulator;*p;p++){if(*p=='_')*p=' ';*p=(char)toupper((unsigned char)*p);}}
 centered(s,952,emulator,strlen(emulator)>60?3:4,blue);char count[64];snprintf(count,sizeof(count),"%d OF %d",selected+1,list->count);styled(s,64,961,count,3,navy);}else centered(s,901,"NO GAMES FOUND",5,navy);
 styled(s,64,1024,"L1 R1 SYSTEM   LEFT RIGHT GAME   X PLAY   O RESCAN",2,navy);
 char network[96];gg_cover_status(network,sizeof(network));
 int nw=(int)strlen(network)*12-2;styled(s,1856-nw,1024,network,2,navy);

 if(status<0)styled(s,1560,961,"LAUNCH FAILED",2,0xff3232c9);
}
