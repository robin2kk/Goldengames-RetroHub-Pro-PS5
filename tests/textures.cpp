// PNG service simulated to exercise real large-texture cache/resampling paths.
#include "diagnostic_frame.h"
#include "ps5_pngdec.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <initializer_list>
static FILE* fixture_open(const char* path,const char* mode){if(strstr(path,"/boxarts/"))return fopen("/tmp/retrohub-texture-fixture.png",mode);return nullptr;}
#define fopen fixture_open
#include "../src/console_ui.cpp"
#undef fopen
static unsigned center;
extern "C" void gg_platform_put_pixel(unsigned x,unsigned y,unsigned c){assert(x<1920&&y<1080);if(x==960&&y==440)center=c;}
extern "C" void gg_draw_text(GGSurface,int,int,const char*,int,uint32_t){}
extern "C" void gg_cover_request(int,const char*,const char*){}
extern "C" void gg_cover_status(char* out,unsigned capacity){snprintf(out,capacity,"HOST");}
extern "C" int sceSysmoduleLoadModule(uint16_t){return 0;}
extern "C" int scePngDecParseHeader(ScePngDecParseParam*,ScePngDecImageInfo* info){info->image_width=1024;info->image_height=1536;return 0;}
extern "C" int scePngDecQueryMemorySize(ScePngDecCreateParam*){return 4096;}
extern "C" int scePngDecCreate(ScePngDecCreateParam*,void*,uint32_t,void** handle){*handle=(void*)1;return 0;}
extern "C" int scePngDecDecode(void*,ScePngDecDecodeParam* param,ScePngDecImageInfo*){auto p=(unsigned char*)param->image_mem_addr;for(unsigned y=0;y<1536;y++)for(unsigned x=0;x<1024;x++){size_t at=((size_t)y*1024+x)*4;p[at]=(unsigned char)x;p[at+1]=(unsigned char)y;p[at+2]=66;p[at+3]=255;}return 0;}
extern "C" int scePngDecDelete(void*){return 0;}
int main(){FILE* file=fopen("/tmp/retrohub-texture-fixture.png","wb");assert(file);fputs("mock",file);fclose(file);static GGGameList games{};games.count=20;for(int i=0;i<20;i++)snprintf(games.games[i].title,GG_NAME_MAX,"DEMO %d",i);
for(int selected: {2,5,10,17,2}){gg_draw_console_browser({1920,1080},7,selected,&games,0);unsigned count=0;size_t bytes=0;
for(Image& im:cache)if(im.pixels){count++;assert(im.system==7&&im.index>=selected-2&&im.index<=selected+2);assert(im.w==512&&im.h==768);assert(im.pixels[0]==0);assert(im.pixels[((size_t)im.w*im.h-1)*4]==254);bytes+=(size_t)im.w*im.h*4;}
assert(count==5);assert(bytes<=5*512*768*4);}
unsigned char bgra[]={0,0,255,255};Image red{};red.w=red.h=1;red.pixels=bgra;cover(&red,0,true,false);assert(center==0xff0000ffu);bgra[0]=255;bgra[2]=0;cover(&red,0,true,false);assert(center==0xffff0000u);
for(Image& im:cache){free(im.pixels);im.pixels=nullptr;}puts("PASS: five large covers remain visible across navigation; resampling endpoints/aspect; bounded cache; screen bounds");}
