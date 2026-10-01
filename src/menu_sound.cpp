/* SPDX-License-Identifier: GPL-3.0-or-later */
#include <stdint.h>
extern "C" {int sceSysmoduleLoadModuleInternal(uint32_t);int sceAudioOutInit();int sceAudioOutOpen(int,int,int,uint32_t,uint32_t,uint32_t);int sceAudioOutOutput(int,const void*);}
extern "C" void gg_sound(int confirm){
 static int port=-2;static int16_t pcm[256*2];
 if(port==-2){sceSysmoduleLoadModuleInternal(0x80000001);sceAudioOutInit();port=sceAudioOutOpen(0xff,0,0,256,48000,1);}
 if(port<0)return;
 unsigned phase=0,step=confirm?800:560;
 for(unsigned block=0;block<6;block++){
  for(unsigned i=0;i<256;i++){phase+=step;int sample=((phase%48000)<24000?1:-1)*(int)(1600*(1536-block*256-i)/1536);pcm[i*2]=pcm[i*2+1]=(int16_t)sample;}
  sceAudioOutOutput(port,pcm);
 }
}
