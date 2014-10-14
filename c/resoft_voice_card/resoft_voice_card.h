#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <jni.h>
#include <conio.h>

#include "tw16vid.h"
#include "playch.h"

/*---------------------------------------------------------------------------*/
/*-----------------------define marco-----------------------*/
/*---------------------------------------------------------------------------*/

/*debug file*/
#define DEBUG_FILE  "resoft_voice_card_log.txt"

/*configure file*/
#define CFG_FILE "voicecard.ini"


/*define max user information number*/
#define MAX_USERINFO_NUMBER 100
/*define max channel number*/
#define MAX_CHANNEL_NUMBER 100
/*define max thread number*/
#define MAX_THREADS 100

/*dial time out, unit is s*/
#define MAX_OFFHOOK_TIMEOUT	40
/*play time out, unit is s*/
#define MAX_PLAY_TIMEOUT	120


/*---------------------------------------------------------------------------*/
/*-----------------------define enum -----------------------*/
/*---------------------------------------------------------------------------*/
enum {cs_hangup,cs_callining,cs_startcallout,cs_dial,cs_waitdialsig,
	  cs_waitoffhook,cs_callouting,cs_callouterr,cs_timeout}channelState;

enum{debug_to_file,	debug_to_console}debugOutType;

enum{filename_empty=-5, openfile_fail=-6, verify_pw_fail=-7, play_string_fail=-8};
enum{s_false=0, s_true=1};
enum{s_not_avaiable=0, s_avaiable=1};
enum{offhook_timeout=-1, play_timeout=-2 };
enum{string_empty=-100, not_find=-101};

/*---------------------------------------------------------------------------*/
/*-----------------------typedef data struct-----------------------*/
/*---------------------------------------------------------------------------*/

/*define inputParam struct*/
typedef struct{
	char *alertCode;
	char *summary;
	char *userInfo[MAX_USERINFO_NUMBER];
	char *reserved1;
	char *reserved2;
}inputParam;

/*define outputResult struct*/
typedef struct{
	char *alertCode;
	char *dialResult[MAX_USERINFO_NUMBER];
	char *reserved1;
	char *reserved2;
}outputResult;

/*define channel struct*/
typedef struct{
	int chn;
	int chnstate;
}channelInfo;

/*define thread input param struct*/
typedef struct{
	int chnlno;
	int userind;
	char *userinfo;
	char *summary;
	JNIEnv *env;
}threadInputParam, *pThreadInputParam;


/*define debug log info*/
typedef struct{
	char *fileName;
	FILE *fp;
	int outputType;  
}debug_info;



/*---------------------------------------------------------------------------*/
/*-----------------------declare funtion-----------------------*/
/*---------------------------------------------------------------------------*/

/*debug function*/
int debug_init(char *filename);
void debug_write(const char *fmt, ...);
void debug_free();


/*channel*/
int channel_init_all();
void channel_init(int chnlno);
int find_available_channel(); /*state equal cs_hangup, avialable*/



/*init inputParam*/
void init_inputParam();
/*get inputParam object*/
int get_inputParam_oject(JNIEnv *env, jobject inputObj);
/*display inputParam*/
void display_inputParam();
/*free inputParam*/
void free_inputParam();


/*init outputResult*/
void init_outputResult();
/*set outputResult object*/
int set_public_outputResult_object(JNIEnv *env, jobject outputObj);
int set_dialResult(JNIEnv *env, jobject obj, jmethodID jmid, char *str);
/*display outputResult*/
void display_outputResult();
/*free outputResult*/
void free_outputResult();


/*dynamic library*/
int load_library();
void unload_library();



/*find function in tw16vid.dll */
int find_dial_number_function();
int dial_number(int chlno, char *userinfo);


/*find function in playch32.dll */
int find_play_string_function();
int play_string(int chlno, char *str);


/*check dial line inserted or not*/
int check_insert_dialline(int chlno);


/*support chinese*/
char* jstringToWindows(JNIEnv* env, jstring jstr);
jstring windowToJstring(JNIEnv* env, char* str);

/*thread call function*/
DWORD WINAPI ThreadFunc(LPVOID lpParam);


/*---------------------------------------------------------------------------*/
/*-----------------------define voice function ponitor-----------------------*/
/*---------------------------------------------------------------------------*/

/************dial function ponitor***********************/

/*int  	TWCALLFUNC TV_Installed (void);*/
typedef int (__stdcall *RESOFT_INSTALL)(void);
/*int 	TWCALLFUNC TV_Initialize(void);*/
typedef int (__stdcall *RESOFT_INITIALIZE)(void);
/*long	TWCALLFUNC TV_TimerElapsed(int ch);*/
typedef long (__stdcall *RESOFT_TIMERELAPSED)(int ch);
/*int 	TWCALLFUNC TV_StartDial(int ch, char *szdtmfbuf);*/
typedef int (__stdcall *RESOFT_STARTDIAL)(int ch, char *szdtmfbuf);
/*int 	TWCALLFUNC TV_StopDial (int ch);*/
typedef int (__stdcall *RESOFT_STOPDIAL)(int ch);
/*int 	TWCALLFUNC TV_DialRest (int ch);*/
typedef int (__stdcall *RESOFT_DIALREST)(int ch);
/*void	TWCALLFUNC TV_HangUpCtrl (int ch);*/
typedef int (__stdcall *RESOFT_HANGUPCTRL)(int ch);
/*void	TWCALLFUNC TV_StartTimer  (int ch, long tl);*/
typedef int (__stdcall *RESOFT_STARTTIMER)(int ch, long tl);
/*void	TWCALLFUNC TV_OffHookCtrl(int ch);*/
typedef void (__stdcall *RESOFT_OFFHOOKCTRL)(int ch);
/*int 	TWCALLFUNC TV_CheckSignal (int ch, int *SigCount, int *SigLen);*/
typedef int (__stdcall *RESOFT_CHECKSIGNAL)(int ch, int *SigCount, int *SigLen);
/*void	TWCALLFUNC TV_StartMonitor  (int ch);*/
typedef void (__stdcall *RESOFT_STARTMONITOR)(int ch);
/*int 	TWCALLFUNC TV_MonitorOffHook(int ch, int w);*/
typedef int (__stdcall *RESOFT_MONITOROFFHOOK)(int ch, int w);
/*int 	TWCALLFUNC TV_ListenerOffHook(int ch);*/
typedef int (__stdcall *RESOFT_LISTENEROFFHOOK)(int ch);
/*void	TWCALLFUNC TV_FlushDTMF   (int ch);*/
typedef void (__stdcall *RESOFT_FLUSHDTMF)(int ch);
/*int 	TWCALLFUNC TV_ChannelType  (int ch);*/
typedef int (__stdcall *RESOFT_CHANNELTYPE)(int ch);
/*int 	TWCALLFUNC TV_GetLastError(void);*/
typedef int (__stdcall *RESOFT_GETLASTERROR)();
/*void	TV_Disable (void);			/½ûÖ¹ÓïÒô¿¨¹¤×÷*/
typedef int (__stdcall *RESOFT_DISABLE)();



/***************play function ponitor***********************/

/*int PasswordDetect(char *password,char *path);*/
typedef int (__stdcall *RESOFT_PASSWORDDECTECT)(char *password, char *path);     
/*int TV_StartPlayCh(int chn,char *sentence);*/
typedef int (__stdcall *RESOFT_STARTPLAYCH)(int chn, char *sentence);   
/*int TV_PlayChRest(int chn);*/
typedef int (__stdcall *RESOFT_PLAYCHREST)(int chn);   
/*int TV_StopPlayCh(int chn);*/
typedef int (__stdcall *RESOFT_STOPPLAYCH)(int chn);   


/***************check dial line inserted or not function ponitor***********************/

/*int	TV_GetLineVoltage(int ch);*/
typedef int (__stdcall *RESOFT_GETLINEVOLTAGE)(int chn);   


/*---------------------------------------------------------------------------*/
/*-----------------------define global variable-----------------------*/
/*---------------------------------------------------------------------------*/

/*vairable*/
inputParam inParam;
outputResult outResult;
channelInfo cInfo[MAX_CHANNEL_NUMBER];/*channel infor*/
debug_info debugInfo; /*debug file*/
int channel_total = 0;  /*the hardware total aviable channel number*/
int resultSet[MAX_USERINFO_NUMBER];/*save every dial and play result*/
jobject arrayList; /*ponint outResult object ArrayList*/
jmethodID addMethod;/*the add method id of java.utlis.ArrayList*/
CRITICAL_SECTION critical_section;/*mulitthread define critical section*/
int avaiable[MAX_CHANNEL_NUMBER];/*channel insert dial line state*/


HMODULE dlh_tw16vid = NULL; /*ponit dynamic library*/
HMODULE dlh_playch32 = NULL; /*ponit dynamic library*/




/*define function pointer variable*/
RESOFT_INSTALL resoft_install = NULL;
RESOFT_INITIALIZE resoft_initalize = NULL;
RESOFT_TIMERELAPSED	resoft_timerelapsed = NULL;
RESOFT_STARTDIAL resoft_startdial = NULL;
RESOFT_STOPDIAL	resoft_stopdial = NULL;
RESOFT_DIALREST	resoft_dialrest = NULL;
RESOFT_HANGUPCTRL resoft_hangupctrl = NULL;
RESOFT_STARTTIMER resoft_starttimer = NULL;
RESOFT_OFFHOOKCTRL resoft_offhookctrl = NULL;
RESOFT_CHECKSIGNAL resoft_checksignal = NULL;
RESOFT_STARTMONITOR resoft_startmonitor = NULL;
RESOFT_MONITOROFFHOOK resoft_monitoroffhook = NULL;
RESOFT_LISTENEROFFHOOK resoft_listeneroffhook = NULL;
RESOFT_FLUSHDTMF resoft_flushdtmf = NULL;
RESOFT_CHANNELTYPE resoft_channeltype = NULL;
RESOFT_GETLASTERROR	resoft_getlasterror = NULL;
RESOFT_DISABLE resoft_disable = NULL;



/*play string and file*/
RESOFT_PASSWORDDECTECT resoft_passworddectect = NULL;
RESOFT_STARTPLAYCH	resoft_startplaych = NULL;
RESOFT_PLAYCHREST	resoft_playchrest = NULL;
RESOFT_STOPPLAYCH	resoft_stopplaych = NULL;


/*get dail line voltage*/
RESOFT_GETLINEVOLTAGE resoft_getlinevoltage = NULL;



