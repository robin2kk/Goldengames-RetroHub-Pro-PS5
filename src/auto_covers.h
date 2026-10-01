#pragma once
#ifdef __cplusplus
extern "C" {
#endif
void gg_cover_request(int system,const char* title,const char* destination);
void gg_cover_status(char* output,unsigned capacity);
#ifdef __cplusplus
}
#endif
