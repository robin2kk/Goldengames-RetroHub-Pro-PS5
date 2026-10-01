/* RetroHub Pro controller/normal-exit helper. GPL-3.0-or-later.
 * Pad ABI: public ProsperoLight/PS5_RetroArch. Process query: payload SDK ps sample.
 * It never kills, patches or writes another process. */
#include "chord.h"
#include <sys/types.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#ifndef LOG_PATH
#define LOG_PATH "/data/homebrew/PPSA99202/return-chord.log"
#endif
#ifndef LOCK_PATH
#define LOCK_PATH "/data/homebrew/PPSA99202/return-chord.lock"
#endif
#define COMMAND_PORT 55359
int sceSysmoduleLoadModuleInternal(uint32_t);
struct LoginUsers {int32_t user[4];};
int sceUserServiceGetLoginUserIdList(struct LoginUsers*);
int sceUserServiceInitialize(const void*);int sceUserServiceGetInitialUser(int*);
int sceUserServiceTerminate(void);int scePadInit(void);int scePadOpen(int,int,int,const void*);
int scePadRead(int,void*,int);int scePadClose(int);
struct PadSample {uint32_t buttons;uint8_t controls[72];int32_t connected;uint64_t timestamp;uint8_t rest[32];};
_Static_assert(sizeof(struct PadSample)==120,"pad ABI size");
_Static_assert(offsetof(struct PadSample,connected)==0x4c,"pad connected ABI");
static void record(const char* message,int value){FILE* f=fopen(LOG_PATH,"a");if(f){fprintf(f,"%s %d\n",message,value);fclose(f);}}
static void record_hex(const char* message,int value){FILE* f=fopen(LOG_PATH,"a");if(f){fprintf(f,"%s 0x%08x\n",message,(unsigned)value);fclose(f);}}
static uint64_t now_us(void){struct timespec t;if(clock_gettime(CLOCK_MONOTONIC,&t))return 0;return (uint64_t)t.tv_sec*1000000+(uint64_t)t.tv_nsec/1000;}
static int retroarch_pid(void){
 int mib[4]={CTL_KERN,KERN_PROC,KERN_PROC_PROC,0};size_t size=0;
 if(sysctl(mib,4,NULL,&size,NULL,0)||!size||size>8*1024*1024)return -1;
 void* data=malloc(size);if(!data)return -1;
 if(sysctl(mib,4,data,&size,NULL,0)){free(data);return -1;}
 unsigned char* p=data;unsigned char* end=p+size;int pid=0;
 while((size_t)(end-p)>=sizeof(struct kinfo_proc)){struct kinfo_proc* k=(void*)p;
  if(k->ki_structsize<=0||(size_t)k->ki_structsize<sizeof(*k)||(size_t)k->ki_structsize>(size_t)(end-p)){pid=-1;break;}
  if(!strncmp(k->ki_comm,"retroarch.elf",sizeof(k->ki_comm))||!strncmp(k->ki_comm,"retroarch",sizeof(k->ki_comm))){if(pid){pid=-1;break;}pid=k->ki_pid;}
  p+=k->ki_structsize;
 }free(data);return pid;
}
static int query_status(int fd,char* response,size_t capacity){
 if(send(fd,"GET_STATUS\n",11,0)!=11)return 0;
 int n=recv(fd,response,capacity-1,0);if(n<=0)return 0;response[n]=0;
 return !strncmp(response,"GET_STATUS PLAYING ",19)||!strncmp(response,"GET_STATUS PAUSED ",18);
}
/* Explicit module initialization and a unique logged-in-user fallback.
 * Negative init can mean already initialized; record it and attempt Open so
 * the real service result is available, rather than leaving a -1 sentinel. */
static int gg_open_pad(int* owns_user){
 record_hex("module-user-service",sceSysmoduleLoadModuleInternal(0x80000011));
 record_hex("module-pad",sceSysmoduleLoadModuleInternal(0x80000024));
 int initialized=sceUserServiceInitialize(NULL);*owns_user=initialized==0;
 record_hex("user-initialize",initialized);
 int user=-1,initial=sceUserServiceGetInitialUser(&user);
 record_hex("user-initial-result",initial);
 if(initial<0||user<0){
  struct LoginUsers users={{-1,-1,-1,-1}};
  int result=sceUserServiceGetLoginUserIdList(&users);record_hex("user-login-list-result",result);
  int count=0;user=-1;
  if(result>=0)for(unsigned i=0;i<4;i++)if(users.user[i]>=0){user=users.user[i];count++;}
  record("user-login-count",count);
  if(result<0||count!=1){record("stop-no-unique-active-user",count);return -1;}
  record("user-source-unique-login",1);
 }else record("user-source-initial",1);
 int initialized_pad=scePadInit();record_hex("pad-init-result",initialized_pad);
 int handle=-1;
 for(int i=0;i<20;i++){
  handle=scePadOpen(user,0,0,NULL);
  if(i==0)record_hex("pad-open-first-result",handle);
  if(handle>=0)break;
  usleep(100000);
 }
 record_hex("pad-open-final-result",handle);return handle;
}
int main(void){
 int lock=open(LOCK_PATH,O_CREAT|O_RDWR,0600);if(lock<0)return 1;
 if(flock(lock,LOCK_EX|LOCK_NB)){close(lock);return 0;}
 FILE* log=fopen(LOG_PATH,"w");if(log){fputs("RetroHub return-chord test03\n",log);fclose(log);}
 int existing=retroarch_pid();if(existing!=0){record("stop-existing-or-unreadable-retroarch",existing);close(lock);return 2;}
 int target=0;for(int i=0;i<120;i++){usleep(250000);target=retroarch_pid();if(target!=0)break;}
 if(target<=0){record("stop-no-new-retroarch",target);close(lock);return 3;}record("target-retroarch-pid",target);
 int fd=socket(AF_INET,SOCK_DGRAM,0);if(fd<0){record("stop-socket",fd);close(lock);return 4;}
 struct sockaddr_in address={0};
#ifndef __linux__
 address.sin_len=sizeof(address);
#endif
 address.sin_family=AF_INET;address.sin_port=htons(COMMAND_PORT);address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
 struct timeval timeout={0,250000};setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
 if(connect(fd,(void*)&address,sizeof(address))){record("stop-connect",-1);close(fd);close(lock);return 5;}
 char response[2048];int ready=0;
 for(int i=0;i<40;i++){if(retroarch_pid()!=target)break;if(query_status(fd,response,sizeof(response))){ready=1;break;}usleep(250000);}
 if(!ready){record("stop-no-retroarch-command-reply",0);close(fd);close(lock);return 6;}record("retroarch-command-ready",1);
 int owns_user=0;int pad=gg_open_pad(&owns_user);
 record("pad-handle",pad);if(pad<0){if(owns_user)sceUserServiceTerminate();close(fd);close(lock);return 7;}
 GGChord chord={0};struct PadSample samples[64];unsigned reads=0,connected=0,intercepted=0;uint32_t observed=0;
 uint64_t started=now_us(),lastcheck=started,lastlog=started;
 for(;;){uint64_t now=now_us();if(!now||now<started||now-started>6ULL*60*60*1000000){record("stop-time-limit",0);break;}
  if(now-lastcheck>=1000000){lastcheck=now;if(retroarch_pid()!=target){record("stop-target-process-gone-or-changed",0);break;}}
  int n=scePadRead(pad,samples,64);if(n<=0||n>64){gg_chord_update(&chord,0,0,now);}else{reads++;struct PadSample* sample=&samples[n-1];if(sample->connected)connected++;if(sample->buttons&GG_PAD_INTERCEPTED)intercepted++;observed|=sample->buttons&GG_RETURN_CHORD;
   if(gg_chord_update(&chord,sample->buttons,sample->connected,now)){
    if(retroarch_pid()==target&&query_status(fd,response,sizeof(response))){int sent=(int)send(fd,"QUIT\n",5,0);record("quit-command-bytes",sent);
     for(int i=0;i<40;i++){usleep(250000);if(retroarch_pid()!=target){record("normal-exit-observed",1);break;}if(i==39)record("quit-sent-but-process-still-running",1);}
    }else record("stop-command-service-or-target-changed",0);break;
   }
  }
  if(now-lastlog>=10000000){lastlog=now;record("pad-valid-reads",(int)reads);record("pad-connected-reads",(int)connected);record("pad-intercepted-reads",(int)intercepted);record("chord-buttons-observed",(int)observed);}
  usleep(16000);
 }
 scePadClose(pad);if(owns_user)sceUserServiceTerminate();close(fd);close(lock);return 0;
}
