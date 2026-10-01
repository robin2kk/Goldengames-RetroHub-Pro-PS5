/* SPDX-License-Identifier: GPL-3.0-or-later
 * HTTP ABI checked against OpenOrbis public headers; no certificate bypass.
 */
#include "auto_covers.h"
#include "ps5_pngdec.hpp"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
extern "C" {
int sceSysmoduleLoadModuleInternal(uint32_t);
int sceNetInit();int sceNetPoolCreate(const char*,int,int);
int sceSslInit(size_t);int sceHttpInit(int,int,size_t);
int sceHttpCreateTemplate(int,const char*,int,int);
int sceHttpCreateConnectionWithURL(int,const char*,bool);
int sceHttpCreateRequestWithURL(int,int,const char*,uint64_t);
int sceHttpSetConnectTimeOut(int,uint32_t);int sceHttpSetResolveTimeOut(int,uint32_t);
int sceHttpSetSendTimeOut(int,uint32_t);int sceHttpSetRecvTimeOut(int,uint32_t);
int sceHttpSendRequest(int,const void*,size_t);int sceHttpGetStatusCode(int,int*);
int sceHttpReadData(int,void*,uint32_t);int sceHttpDeleteRequest(int);
int sceHttpDeleteConnection(int);int sceHttpDeleteTemplate(int);
}
namespace {
const char* catalogs[]={"Nintendo - Nintendo Entertainment System","Nintendo - Super Nintendo Entertainment System","Nintendo - Nintendo 64","Nintendo - Game Boy","Nintendo - Game Boy Color","Nintendo - Game Boy Advance","Sega - Mega Drive - Genesis","Sega - Mega-CD - Sega CD","Sega - 32X","Sega - Saturn","Sony - PlayStation","Atari - 2600","Atari - 7800","Atari - Lynx","Atari - Jaguar","NEC - PC Engine - TurboGrafx 16","Arcade","Commodore - Amiga","Commodore - 64"};
struct Job{char url[2400],path[1100];};Job queue[8];unsigned head=0,tail=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition=PTHREAD_COND_INITIALIZER;
int state=0,downloaded=0,missing=0,failed=0;bool started=false;
bool encode(char* out,size_t cap,const char* text){const char* hex="0123456789ABCDEF";size_t n=0;for(const unsigned char* p=(const unsigned char*)text;*p;p++){bool simple=(*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||(*p>='0'&&*p<='9')||strchr("-_.~",*p);size_t need=simple?1:3;if(n+need>=cap)return false;if(simple)out[n++]=*p;else{out[n++]='%';out[n++]=hex[*p>>4];out[n++]=hex[*p&15];}}out[n]=0;return true;}
int init(){
 if(sceSysmoduleLoadModuleInternal(0x8000001c)<0||sceSysmoduleLoadModuleInternal(0x8000000a)<0||sceSysmoduleLoadModuleInternal(0x8000000b)<0)return -1;
 sceNetInit();int net=sceNetPoolCreate("retrohub-covers",256*1024,0);if(net<0)return -1;
 int ssl=sceSslInit(256*1024);if(ssl<0)return -1;return sceHttpInit(net,ssl,512*1024);
}
int fetch(int http,const Job& job){
 if(access(job.path,F_OK)==0)return 0;
 int tpl=sceHttpCreateTemplate(http,"RetroHubPro/3D-Test",2,1);if(tpl<0)return -1;
 sceHttpSetConnectTimeOut(tpl,5000000);sceHttpSetResolveTimeOut(tpl,5000000);sceHttpSetSendTimeOut(tpl,5000000);sceHttpSetRecvTimeOut(tpl,5000000);
 int conn=sceHttpCreateConnectionWithURL(tpl,job.url,false),request=-1,result=-1;
 if(conn>=0){request=sceHttpCreateRequestWithURL(conn,0,job.url,0);
  if(request>=0&&sceHttpSendRequest(request,nullptr,0)>=0){int code=0;if(sceHttpGetStatusCode(request,&code)>=0){
   if(code==404)result=2;
   else if(code==200){unsigned char* data=(unsigned char*)malloc(8*1024*1024);size_t size=0;int count=-1;
    if(data){while(size<8*1024*1024){count=sceHttpReadData(request,data+size,(uint32_t)((8*1024*1024-size)>16384?16384:(8*1024*1024-size)));if(count<=0)break;size+=(unsigned)count;}
     ScePngDecParseParam parse{data,(uint32_t)size,0};ScePngDecImageInfo info{};
     if(count==0&&size>=24&&scePngDecParseHeader(&parse,&info)>=0&&info.image_width&&info.image_height&&info.image_width<=2048&&info.image_height<=2048){
      ScePngDecCreateParam create{sizeof(create),info.bit_depth>8?1u:0u,info.image_width};int n=scePngDecQueryMemorySize(&create);
      if(n>0&&n<16*1024*1024){void* work=malloc(n),*handle=nullptr;unsigned bytes=info.image_width*info.image_height*4;void* pixels=malloc(bytes);
       if(work&&pixels&&scePngDecCreate(&create,work,n,&handle)>=0){ScePngDecDecodeParam decode{data,pixels,(uint32_t)size,bytes,1,255,info.image_width*4};ScePngDecImageInfo output{};
        if(scePngDecDecode(handle,&decode,&output)>=0){char directory[1100];snprintf(directory,sizeof(directory),"%s",job.path);char* slash=strrchr(directory,'/');if(slash)*slash=0;mkdir("/app0/boxarts",0777);mkdir(directory,0777);
         char temporary[1120];snprintf(temporary,sizeof(temporary),"%s.part",job.path);FILE* file=fopen(temporary,"wb");if(file){bool ok=fwrite(data,1,size,file)==size;if(fclose(file)!=0)ok=false;if(ok&&access(job.path,F_OK)!=0&&rename(temporary,job.path)==0)result=1;else unlink(temporary);}
        }scePngDecDelete(handle);
       }free(pixels);free(work);
      }
     }free(data);
    }
   }
  }}
 }
 if(request>=0)sceHttpDeleteRequest(request);if(conn>=0)sceHttpDeleteConnection(conn);sceHttpDeleteTemplate(tpl);return result;
}
void* worker(void*){
 int http=init();pthread_mutex_lock(&mutex);state=http>=0?1:-1;pthread_mutex_unlock(&mutex);if(http<0)return nullptr;
 for(;;){pthread_mutex_lock(&mutex);while(head==tail)pthread_cond_wait(&condition,&mutex);Job job=queue[head];head=(head+1)%8;pthread_mutex_unlock(&mutex);
  int result=fetch(http,job);pthread_mutex_lock(&mutex);if(result==1)downloaded++;else if(result==2)missing++;else if(result<0)failed++;pthread_mutex_unlock(&mutex);
 }return nullptr;
}
}
extern "C" void gg_cover_request(int system,const char* title,const char* destination){
 if(system<0||system>=19||!title||!*title||access(destination,F_OK)==0)return;
 Job job{};char cat[256],name[1600],safe[520];size_t n=0;for(;title[n]&&n+5<sizeof(safe);n++){unsigned char c=title[n];if(c<32)return;safe[n]=strchr("&*/:<>?\\|",c)?'_':c;}if(title[n]||safe[0]=='.')return;memcpy(safe+n,".png",5);
 if(!encode(cat,sizeof(cat),catalogs[system])||!encode(name,sizeof(name),safe))return;
 snprintf(job.url,sizeof(job.url),"https://thumbnails.libretro.com/%s/Named_Boxarts/%s",cat,name);snprintf(job.path,sizeof(job.path),"%s",destination);
 pthread_mutex_lock(&mutex);if(!started){started=true;pthread_t thread;if(pthread_create(&thread,nullptr,worker,nullptr)!=0)state=-1;else pthread_detach(thread);}
 if(state>=0&&(tail+1)%8!=head){queue[tail]=job;tail=(tail+1)%8;pthread_cond_signal(&condition);}pthread_mutex_unlock(&mutex);
}
extern "C" void gg_cover_status(char* output,unsigned capacity){pthread_mutex_lock(&mutex);if(state<0)snprintf(output,capacity,"COVERS: NETWORK UNAVAILABLE");else snprintf(output,capacity,"COVERS %d  NOT FOUND %d  ERRORS %d",downloaded,missing,failed);pthread_mutex_unlock(&mutex);}
