#include <stdio.h>
#include <string.h>
#include <assert.h>
static FILE *fixture_open(const char*p,const char*m){char path[600];if(!strncmp(p,"/app0/library/",14)){snprintf(path,sizeof(path),"CODIGO/tests/fixtures/%s",p+14);return fopen(path,m);}if(!strcmp(p,"/app0/rom-scan.log"))return tmpfile();return fopen(p,m);}
#define fopen fixture_open
#include "../src/rom_scanner.c"
int main(void){static GGGameList g;const char*ids[]={"n64","psx","saturn"};int expected[]={113,54,35};for(int i=0;i<3;i++){assert(gg_scan_games(ids[i],&g)==expected[i]);assert(g.manifest_found);assert(g.games[0].title[0]);assert(!strchr(g.games[0].filename,'\t'));assert(!g.games[0].core[0]);assert(!scan_log);}assert(!allowed("segacd","bin"));puts("PASS uploaded manifests: N64 113, PSX 54, Saturn 35; titles/paths intact; log closed; Sega CD unchanged");}
