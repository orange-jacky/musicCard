/**************************************************************************

	FILE ........... PLAYCH.H
	FUNCTION ....... 语音库头文件
	VERSION ........ 1.6

	注意: 凡是随 TeleWind 电话卡提供的 C 库函数, 都以 TV 开头,
	如 TV_Installed(), TV_Initialize(), TV_Disable() 等.

**************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define WINAPI
#else
#define TM_NOTIFY		(WM_USER+7)
#endif

#ifndef MAX_LONG
#define MAX_LONG 0x7fffffff
#endif

int WINAPI PasswordDetect(char far *password,char *path);
int WINAPI TV_StartPlayCh(int chn,char *sentence);
int WINAPI TV_PlayChRest(int chn);
int WINAPI TV_StopPlayCh(int chn);
int WINAPI TV_StartPlayChFile(int chn,char *fn);
int WINAPI TV_PlayChFileRest(int chn);
int WINAPI TV_StopPlayChFile(int chn);

#ifdef __cplusplus
}
#endif

