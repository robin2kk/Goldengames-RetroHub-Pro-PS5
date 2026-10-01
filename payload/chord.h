/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <stdint.h>
#define GG_RETURN_CHORD (UINT32_C(0x400)|UINT32_C(0x800)|UINT32_C(0x100000))
#define GG_PAD_INTERCEPTED UINT32_C(0x80000000)
typedef struct {int released;uint64_t start;int fired;} GGChord;
static int gg_chord_update(GGChord* s,uint32_t b,int connected,uint64_t now){
 if(s->fired)return 0;
 if(!connected||(b&GG_PAD_INTERCEPTED)){s->released=0;s->start=0;return 0;}
 if(!(b&GG_RETURN_CHORD)){s->released=1;s->start=0;return 0;}
 if(!s->released||(b&GG_RETURN_CHORD)!=GG_RETURN_CHORD){s->start=0;return 0;}
 if(!s->start){s->start=now;return 0;}
 if(now>=s->start&&now-s->start>=500000){s->fired=1;return 1;}return 0;
}
