#include <stdio.h>
#include <string.h>
#include <assert.h>
static FILE *test_open(const char *p,const char *m){if(!strncmp(p,"/app0/library/",14))return tmpfile();if(!strcmp(p,"/app0/rom-scan.log"))return tmpfile();return fopen(p,m);}
#define fopen test_open
#define GG_RETROARCH_ROMS "/tmp/retrohub-scan08-fixture"
#include "../src/rom_scanner.c"
int main(void){static GGGameList games;const char *ids[]={"n64","psx","saturn"};for(int i=0;i<3;i++){assert(gg_scan_games(ids[i],&games)==(i==0?1:2));assert(games.manifest_found);assert(strstr(games.games[0].filename,"/nested/"));assert(!games.games[0].core[0]);}puts("PASS: nested N64/PSX/Saturn, empty manifest fallback, uppercase formats, symlink ignored");}
