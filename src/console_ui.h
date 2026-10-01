#pragma once
#include "diagnostic_frame.h"
#include "rom_scanner.h"
#ifdef __cplusplus
extern "C" {
#endif
void gg_draw_console_browser(GGSurface,int,int,const GGGameList*,int);
void gg_sound(int);
#ifdef __cplusplus
}
#endif
