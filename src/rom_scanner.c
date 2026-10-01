#include "rom_scanner.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#ifndef GG_RETROARCH_ROMS
#define GG_RETROARCH_ROMS "/data/homebrew/RetroArch/roms"
#endif

static int eq(const char*a,const char*b){while(*a&&*b){if(tolower((unsigned char)*a++)!=tolower((unsigned char)*b++))return 0;}return!*a&&!*b;}
static int folder_match(const char *id,const char *folder){
 char normalized[96];unsigned n=0;
 for(const unsigned char*p=(const unsigned char*)folder;*p;p++){
  if(isalnum(*p)){if(n+1>=sizeof(normalized))return 0;normalized[n++]=(char)tolower(*p);}
 }
 normalized[n]=0;
 if(eq(id,normalized))return 1;
 static const struct {const char*id,*folder;} aliases[]={
  {"psx","ps1"},{"psx","playstation"},{"psx","sonyplaystation"},
  {"nes","nintendoentertainmentsystem"},{"snes","supernintendo"},
  {"n64","nintendo64"},{"gb","gameboy"},{"gbc","gameboycolor"},
  {"gba","gameboyadvance"},{"genesis","megadrive"},{"genesis","segagenesis"},
  {"segacd","megacd"},{"x32","sega32x"},{"saturn","segasaturn"},
  {"pce","pcengine"},{"pce","turbografx16"},{"arcade","fbneo"},
  {"c64","commodore64"},{"atari2600","atari2600"},{"atari7800","atari7800"}
 };
 for(unsigned i=0;i<sizeof(aliases)/sizeof(aliases[0]);i++)
  if(eq(id,aliases[i].id)&&eq(normalized,aliases[i].folder))return 1;
 return 0;
}
static int allowed(const char*id,const char*ext){
 if(eq(id,"nes"))return eq(ext,"nes")||eq(ext,"zip");
 if(eq(id,"snes"))return eq(ext,"sfc")||eq(ext,"smc")||eq(ext,"zip");
 if(eq(id,"n64"))return eq(ext,"z64")||eq(ext,"n64")||eq(ext,"v64")||eq(ext,"7z")||eq(ext,"zip");
 if(eq(id,"genesis"))return eq(ext,"md")||eq(ext,"gen")||eq(ext,"bin")||eq(ext,"zip");
 if(eq(id,"psx"))return eq(ext,"cue")||eq(ext,"chd")||eq(ext,"pbp")||eq(ext,"bin");
 if(eq(id,"gb"))return eq(ext,"gb")||eq(ext,"zip");
 if(eq(id,"gbc"))return eq(ext,"gbc")||eq(ext,"zip");
 if(eq(id,"gba"))return eq(ext,"gba")||eq(ext,"zip");
 if(eq(id,"saturn"))return eq(ext,"cue")||eq(ext,"chd")||eq(ext,"bin");
 if(eq(id,"segacd"))return eq(ext,"cue")||eq(ext,"chd");
 if(eq(id,"x32"))return eq(ext,"32x")||eq(ext,"bin");
 if(eq(id,"atari2600"))return eq(ext,"a26")||eq(ext,"bin")||eq(ext,"zip");
 if(eq(id,"atari7800"))return eq(ext,"a78")||eq(ext,"bin")||eq(ext,"zip");
 if(eq(id,"lynx"))return eq(ext,"lnx")||eq(ext,"zip");
 if(eq(id,"jaguar"))return eq(ext,"j64")||eq(ext,"jag")||eq(ext,"zip");
 if(eq(id,"pce"))return eq(ext,"pce")||eq(ext,"zip");
 if(eq(id,"arcade"))return eq(ext,"zip");
 if(eq(id,"amiga"))return eq(ext,"adf")||eq(ext,"hdf")||eq(ext,"lha");
 if(eq(id,"c64"))return eq(ext,"d64")||eq(ext,"t64")||eq(ext,"crt");
 return 0;
}
static void title_from(const char*name,char*out){snprintf(out,GG_NAME_MAX,"%s",name);char*p=strrchr(out,'.');if(p)*p=0;}

static FILE *scan_log;
static int scan_dir(const char *path,const char *id,GGGameList*out,unsigned depth){
 DIR *dir=opendir(path);
 if(!dir){if(scan_log)fprintf(scan_log,"OPEN_FAIL %d %s\n",errno,path);return out->count;}
 out->folder_found=1;
 if(scan_log)fprintf(scan_log,"DIRECTORY %s\n",path);
 struct dirent *entry;
 while(out->count<GG_MAX_GAMES&&(entry=readdir(dir))){
  const char *name=entry->d_name;if(name[0]=='.')continue;
  char full[GG_NAME_MAX];struct stat st;
  if(snprintf(full,sizeof(full),"%s/%s",path,name)>=(int)sizeof(full))continue;
  if(lstat(full,&st)!=0){if(scan_log)fprintf(scan_log,"STAT_FAIL %d %s\n",errno,full);continue;}
  if(S_ISDIR(st.st_mode)){if(depth<8)scan_dir(full,id,out,depth+1);continue;}
  if(!S_ISREG(st.st_mode))continue;
  const char *ext=strrchr(name,'.');
  if(!ext||!allowed(id,ext+1)){if(scan_log)fprintf(scan_log,"SKIP_FORMAT %s\n",full);continue;}
  GGGame*g=&out->games[out->count];snprintf(g->filename,GG_NAME_MAX,"%s",full);
  title_from(name,g->title);out->count++;
  if(scan_log)fprintf(scan_log,"FOUND %s\n",full);
 }
 closedir(dir);return out->count;
}


static int scan_manifest(const char *id,GGGameList*out){
 char manifest[256];snprintf(manifest,sizeof(manifest),"/app0/library/%s.lst",id);
 FILE *fp=fopen(manifest,"r");if(!fp)return 0;
 out->manifest_found=1;
 char line[GG_NAME_MAX*3];
 while(out->count<GG_MAX_GAMES&&fgets(line,sizeof(line),fp)){
  size_t n=strlen(line);
  if(n&&line[n-1]!='\n'&&!feof(fp)){int ch;while((ch=fgetc(fp))!='\n'&&ch!=EOF){}continue;}
  while(n&&(line[n-1]=='\n'||line[n-1]=='\r'))line[--n]=0;
  if(!n||line[0]=='#')continue;
  char *title=strchr(line,'\t');char *core=0;
  if(title){*title++=0;core=strchr(title,'\t');if(core)*core++=0;}
  const char *base=strrchr(line,'/');base=base?base+1:line;
  const char *p=strrchr(base,'.');if(!p||!allowed(id,p+1))continue;
  /* Manifests store the real path used by the existing payload RetroArch. */
  if(strncmp(line,"/data/homebrew/RetroArch/",25)!=0&&
     strncmp(line,"/mnt/usb",8)!=0&&strncmp(line,"/mnt/ext",8)!=0)continue;
  if(strlen(line)>=GG_NAME_MAX-1)continue;
  GGGame *g=&out->games[out->count];snprintf(g->filename,GG_NAME_MAX,"%s",line);if(title&&*title)snprintf(g->title,GG_NAME_MAX,"%s",title);else title_from(base,g->title);if(core)snprintf(g->core,GG_NAME_MAX,"%s",core);else g->core[0]=0;out->count++;
 }
 fclose(fp);return out->count;
}
int gg_scan_games(const char*id,GGGameList*out){
 if(!id||!out)return 0;
 memset(out,0,sizeof(*out));
 out->count=0;
 out->folder_found=0;out->manifest_found=0;
 scan_log=fopen("/app0/rom-scan.log","a");
 scan_manifest(id,out);
 if(out->count){if(scan_log){fprintf(scan_log,"MANIFEST %s accepted=%d\n",id,out->count);fclose(scan_log);scan_log=0;}return out->count;}
 if(scan_log)fprintf(scan_log,"SCAN %s manifest=%d root=%s\n",id,out->manifest_found,GG_RETROARCH_ROMS);
 char path[512];const char *root=GG_RETROARCH_ROMS;
 DIR *dirs=opendir(root);
 if(dirs){
  struct dirent *entry;
  while(out->count<GG_MAX_GAMES&&(entry=readdir(dirs))){
   if(!folder_match(id,entry->d_name))continue;
   if(snprintf(path,sizeof(path),"%s/%s",root,entry->d_name)>=(int)sizeof(path))continue;
   scan_dir(path,id,out,0);
  }
  closedir(dirs);
 }else if(scan_log)fprintf(scan_log,"ROOT_OPEN_FAIL %d %s\n",errno,root);
 /* A manifest supplies paths when the native title cannot enumerate that folder. */
 if(scan_log){fprintf(scan_log,"RESULT %s %d\n",id,out->count);fclose(scan_log);scan_log=0;}
 return out->count;
}
