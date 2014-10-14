/*****************************************************************************

	FILE ........... TWI-TWI-8(16)PF/TWI-8(16)A.H
	FUNCTION ....... Header file for Telewind - TWI-8(16)PF/TWI-8(16)A API
	VERSION ........ 1.00

*****************************************************************************/
#ifndef __TWI_16APF_H
#define __TWI_16APF_H

#define MAX_ADAPTERS	    16
#define MAX_CHANNELS    	(16 * MAX_ADAPTERS)
#define PHONE_CHANNELS	    MAX_CHANNELS
#define CHANNEL_MASK	    (MAX_CHANNELS-1)

#define GROUP_NUM           8			// conference max group 
#define CONF_CHANNELS       16		// conference max conference channels

#define FILE_FLAG	        1        	/* For TV_PlaySentence(...) */
#define DTMF_LEN	        24

#define MAX_CALLINGID_LEN	24

/*	Return code for function call
*/
#define E_DRIVER		    0xff	/* TW8VID driver not installed */
#define E_OK				0x00	/* OK */
#define E_COMMAND			0x01	/* Invalid command */
#define E_LENGTH			0x02	/* Too few buffer length */
#define E_PLAY_RECORD		0x03	/* Play/Record conflict */
#define E_CHANNEL			0x04	/* Invalid channel number */
#define E_ARGUMENT			0x05
#define E_ERR_SYNC			0x6
#define E_OUT_OF_MEMORY		0x7
#define E_ERR				0x8
#define E_RECORD_BUSY		0x9
#define E_FILEOPEN			0xa
#define E_PLAY_BUSY			0xb
#define E_NOCARD			0xc
#define E_GROUP				0xd

#define E_NO_CONF_PORT		0x20
#define E_NO_FAX_PORT		0x21

			// conference define
#define E_NO_GROUP			-1
#define E_NO_CHANNEL		-2
#define	E_PARAM				-3
#define E_NO_SUCH_GROUP		-4
#define E_NO_CHANNELS		-5
#define E_EXIST_CHANNEL		-6

#define E_NO_MEMORY			-7


#define CT_INTERNAL			0	/* Internal channel */
#define CT_EXTERNAL		    1	/* External channel */
#define CT_EMPTY			2
#define CT_VOLTAGECTRL		3	/* 搭线 channel */
#define CT_VOICECTRL		4	/* 声控 channel */
#define CT_PLAY_REC			5	/* 录放音模块 channel */

#define CT_DA_XIAN			CT_VOLTAGECTRL

#define RATE_64K	        0
#define RATE_32K			1
#define RATE_16K			2

#define SIGNAL_TYPE			8
/*
 *	Signal type
 */
#define	SIG_UNKNOWN			0x60
#define SIG_TIMEOUT			0x61
#define SIG_OFFHOOK			0x62
#define SIG_NOBODY			0x63

#define SIG_SILENCE			0x40
#define SIG_DIAL			0x41

#define SIG_RING			0x00
#define SIG_BUSY1			0x01
#define SIG_BUSY2			0x02

	// Add two new signal type    may set by TV_SetSignalParam
#define SIG_USER_SIG1		0x3
#define SIG_USER_SIG2		0x4

#define CM_NORMAL			0x0		   /* channel work in not compress mode */
#define CM_COMPRESS		    0x1		   /* channel work in compress mode */

#define AFTERDIAL			0
#define RECIEVEDIAL			1
	// Set Macro Define
#define TSet_Mode			0	    	// Flag of  poll or event mode set
#define TSet_Busy			1		    // Flag of  busy set
#define TSet_Dial			2	    	// Flag of  dial speed set
#define TSet_Play			3	    	// Flag of  play speed set
#define TSet_Record			4	     	// Flag of  record speed set
#define MaxDataLen			176
typedef struct {
	int  Type;	                 	// Type of Event
	int  Channel;		            // Channel 
	union EDATA
	{
		int  Result;
		char ptrData[MaxDataLen];	
	} data;
	int  len;
} TV_Event;			
/* CHAR_NAME is for TV_MakeSentence(...)
*/
typedef enum {
	CN_END = 0,		/* End of sentence (Also end of string) */
	CN_NOTHING,		/* Do nothing */
	CN_DIGIT0, CN_DIGIT1, CN_DIGIT2, CN_DIGIT3, CN_DIGIT4,	/* 0 - 4 */
	CN_DIGIT5, CN_DIGIT6, CN_DIGIT7, CN_DIGIT8, CN_DIGIT9,	/* 5 - 9 */
	CN_TEN,			/*          10 */
	CN_HUNDRED,		/*         100 */
	CN_THOUSAND,	/*       1,000 */
	CN_10THOUSAND,	/*      10,000 */
	CN_100MILLION,	/* 100,000,000 */
	CN_POINT,		/* "." */
	CN_NEGATIVE,	/* "-" */

	CN_LAST			/* To be continued by YOU ! */
} CHAR_NAME;

typedef struct {
	int	silence_sig_min;	/* Min. count of SIG_SILENCE */
	int	dial_sig_min;   	/* Min. count of SIG_DIAL */
	int	signal_para[SIGNAL_TYPE][4];
} PCB_STRUC;            	/* Parameter Control Block Structure */

typedef struct {
	unsigned char	MajorVer;
	unsigned char	MinorVer;
	unsigned char	IRQNo;
	unsigned char	IntrNo;
	PCB_STRUC 		PCB;
	int	AdapterNum;
	int	ChannelNum;
	unsigned int	TW16VIDSeg[MAX_ADAPTERS];
	int*		reserved1;
	unsigned char   * reserved2[512];
} SP_STRUC;	/* System Parameter Structure */


extern int		TV_ReturnCode;	/* Return code for last call */
extern int		TV_CloseFile;	/* Close files in TV_StartRecordFile() & TV_StartPlayFile() ? */
extern char		**TV_VoiceData;	/* Voice data for CHAR_NAME */
extern int	*TV_VoiceLen;	    /* Voice data length for CHAR_NAME */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define TWCALLFUNC
#else
#define TWCALLFUNC	WINAPI
#endif

int  	TWCALLFUNC TV_Installed (void);
int 	TWCALLFUNC TV_Initialize(void);
void	TWCALLFUNC TV_Disable   (void);

int 	TWCALLFUNC TV_ChannelType  (int ch);
int 	TWCALLFUNC TV_OffHookDetect(int ch);
int 	TWCALLFUNC TV_RingDetect (int ch);
void	TWCALLFUNC TV_HangUpCtrl (int ch);
void	TWCALLFUNC TV_OffHookCtrl(int ch);
void	TWCALLFUNC TV_RingCtrl   (int ch);
void	TWCALLFUNC TV_PowerCtrl  (int ch);
void	TWCALLFUNC TV_SysPara    (SP_STRUC far *szsysinfo);
void	TWCALLFUNC TV_CompressRatio (int compressrate);//rate: 0,1,2
void 	TWCALLFUNC TV_CompressRatioEx(int ch,int cr);

long	TWCALLFUNC TV_StartRecord (int ch, char far *pchBuffer, int Len);
long	TWCALLFUNC TV_StartRecord2(int Ch, char *pchBuffer, int Len);
long    TWCALLFUNC TV_RecordRest  (int ch);
long	TWCALLFUNC TV_StopRecord  (int ch);

long	TWCALLFUNC TV_StartPlay(int ch, char *pchBuffer, int Len);
long    TWCALLFUNC TV_PlayRest (int ch);
long	TWCALLFUNC TV_StopPlay (int ch);

int 	TWCALLFUNC TV_StartDial(int ch, char *szdtmfbuf);
int 	TWCALLFUNC TV_DialRest (int ch);
int 	TWCALLFUNC TV_StopDial (int ch);

int     TWCALLFUNC TV_ReceiveCallingID (int ch, char far *rb, int rl);
void	TWCALLFUNC TV_FlushDTMF   (int ch);
int 	TWCALLFUNC TV_GetDTMFChar (int ch);
char far *TWCALLFUNC TV_GetDTMFStr(int ch);

long	TWCALLFUNC TV_StartRecordFile(int ch,char *pchFileName,long lStartByte,long lRecordLen);
long	TWCALLFUNC TV_RecordFileRest (int ch);
long	TWCALLFUNC TV_StopRecordFile (int ch);
long	TWCALLFUNC TV_StartPlayFile  (int ch,char *pchFileName,long lStartByte,long lPlayLen);
long	TWCALLFUNC TV_PlayFileRest   (int ch);
long	TWCALLFUNC TV_StopPlayFile   (int ch);

long	TWCALLFUNC TV_StartRecordWaveFile(int ch,char *pchFileName);
long	TWCALLFUNC TV_RecordWaveFileRest (int ch);
long	TWCALLFUNC TV_StopRecordWaveFile (int ch);
long	TWCALLFUNC TV_StartPlayWaveFile  (int ch,char *pchFileName);
long	TWCALLFUNC TV_PlayWaveFileRest   (int ch);
long	TWCALLFUNC TV_StopPlayWaveFile   (int ch);

long	TWCALLFUNC TV_DStartRecordWaveFile(int ch1,int ch2,char *pchFileName);
long	TWCALLFUNC TV_DRecordWaveFileRest (int ch1);
long	TWCALLFUNC TV_DStopRecordWaveFile (int ch1);
long    TWCALLFUNC TV_DStartRecordFile(int ch1, int ch2, char * pchFileName);
long    TWCALLFUNC TV_DRecordFileRest (int ch);
long    TWCALLFUNC TV_DStopRecordFile (int ch);

void	TWCALLFUNC TV_MakeSentence    (double f, unsigned char *s);
long    TWCALLFUNC TV_PlaySentenceRest(int ch);
long	TWCALLFUNC TV_PlaySentence    (int ch, unsigned char *s);
long    TWCALLFUNC TV_StopPlaySentence(int ch);

void	TWCALLFUNC TV_ConnectChannels   (int ch1, int ch2);
void	TWCALLFUNC TV_DisconnectChannels(int ch1, int ch2);
void	TWCALLFUNC TV_ConnectTo  (int ch1, int ch2);
void	TWCALLFUNC TV_Disconnect (int ch);
void	TWCALLFUNC TV_Connect3   (int ch1, int ch2, int ch3);
void	TWCALLFUNC TV_Disconnect3(int ch1, int ch2, int ch3);

void	TWCALLFUNC TV_GenerateSignal(int ch, int st);
void	TWCALLFUNC TV_GenerateRing  (int ch);

void	TWCALLFUNC TV_StartTimer  (int ch, long tl);
long	TWCALLFUNC TV_TimerElapsed(int ch);
void	TWCALLFUNC TV_GetSerial   (char far *pSerial);
void    TWCALLFUNC TV_GetSerialEx (int na,char far *pSerial);

int 	TWCALLFUNC TV_CheckSignal (int ch, int *SigCount, int *SigLen);
int 	TWCALLFUNC TV_CheckOffHook(int ch, int *SigCount, int *SigLen);
int 	TWCALLFUNC TV_ListenerOffHook(int ch);
void	TWCALLFUNC TV_StartMonitor  (int ch);
int 	TWCALLFUNC TV_MonitorOffHook(int ch, int w);
int 	TWCALLFUNC TV_MonitorBusy   (int ch, int b, int c);

void	TWCALLFUNC TV_SetSignalParam(int is, int lmin, int lmax,int hmin,int hmax);
int 	TWCALLFUNC TV_SetSignalParamEx( int is, double hlen, double llen,double linterval1,double linterval2);
void 	TWCALLFUNC TV_SetDTMFSendSpeed( int ch, int speed);
void 	TWCALLFUNC TV_SetSendMode     ( int ch, int m);

int 	TWCALLFUNC TV_OcDetect (int ch);
int 	TWCALLFUNC TV_SetOcTime(int t);
int 	TWCALLFUNC TV_SetOcInterval  (int omin,int omax);
int 	TWCALLFUNC TV_SetOcIntervalEx(int ch,int omin,int omax);

void    TWCALLFUNC TV_SetVoicei   (int i,char far *filename);
int 	TWCALLFUNC TV_GetLastError(void);

int 	TWCALLFUNC TV_SetAmp(int ch,int Amp);	// 20 -- 80
int 	TWCALLFUNC TV_GetAmp(int ch);//031110

int 	TWCALLFUNC TV_SetChannelFreq(int ch,int Hz);

int 	TWCALLFUNC TV_InternalRingDetect( int ch );
void    TWCALLFUNC TV_SetRingTime(int ch,int maxringinterval,int ringingtime,int ringstoptime);
                   //maxringinterval,ringingtime,ringstoptime，时间单位均是ms,要>160ms
int     TWCALLFUNC TV_SetZoomPcmRate(float rate);
int 	TWCALLFUNC TV_GetFaxSig     (int ch);


int 	TWCALLFUNC TV_SetRcvFskStatus(int ch,int checkout);//此函数只需对任意一个通道设置一次即可对所有通道生效
void    TWCALLFUNC TV_StartGetFskData(int ch);
int 	TWCALLFUNC TV_GetFSKBuffer   (int ch,unsigned char far *buf,int cb);

//该函数发送数据的格式均要求采用  类型（0x84/0x87）＋信息长度＋信息＋校验码方式
int 	TWCALLFUNC TV_StartSendFskData(int ch,unsigned char *buf,int len);
int 	TWCALLFUNC TV_StopSendFskData (int ch);
int 	TWCALLFUNC TV_SendFskDataRest (int ch);
//为了兼容16PF的音量调节，如果使用16A或者16R，请尽量避免使用此函数
int     TWCALLFUNC TV_SetVoiceDB(int ch,int rp,int db);
//////////////////////////////////////////////////////////////////////////
// 下面7个函数是8A所没有的，因为8A无级联功能，所以不提供下列函数
//////////////////////////////////////////////////////////////////////////

void	TWCALLFUNC CT_Func(int func,int a,int lc,int ct_line,int ct_slot);
int 	TWCALLFUNC TV_AttachTwpcm( int ch );
int 	TWCALLFUNC TV_AttachTwpcmEx1 ( int ach,int dch );
int 	TWCALLFUNC TV_AttachTwpcmEx7 ( int ach,int dch );
void	TWCALLFUNC TV_ConnectToTW8VID(int nCard,int srcSt, int srcCh, int dstSt,int dstCh);//l
void    TWCALLFUNC TV_UnAttachTwpcm(int vidch);
void    TWCALLFUNC TWC_VidJoinDigGroup(int vch,int cch);//l
void    TWCALLFUNC TWC_VidQuitDigGroup(int vch,int cch);//l
//////////////////////////////////////////////////////////////////////////
// 上面7个函数为8/16PF和8/16A－CT专有
//////////////////////////////////////////////////////////////////////////

int 	TWCALLFUNC TV_SetChannelParam(int ch,int which,int v);
//which =0,v=0 不测dtmf,1 一次,2 二次,3 多次确认,else v=3 
//return 上次设定值
int 	TWCALLFUNC TV_SetSendFskTime (char *buf);
int 	TWCALLFUNC TV_DtmfAbsent     (int ch);

//////////////////////////////////////////////////////////////////////////
//
//                            部分空函数
//
//////////////////////////////////////////////////////////////////////////
void	TWCALLFUNC TV_SetSignalLevel( int );
void	TWCALLFUNC TV_SetChannelMode( int , int );
int 	TWCALLFUNC TV_SetFaxPort    ( int port );
int 	TWCALLFUNC TV_SetConfPort ( int inPort, int outPort );
int 	TWCALLFUNC TV_SetConfPort2( int inPort, int outPort );
int 	TWCALLFUNC TV_AttachFax   ( int ch, int fax );
void	TWCALLFUNC TV_SetVoi(int ch, int v);
int 	TWCALLFUNC TV_SetVos( int v);
int 	TWCALLFUNC TV_SetAdapterParam (int which,int v);
int 	TWCALLFUNC TV_GetVoltageBuffer(int ch,unsigned char far *buf);
int 	TWCALLFUNC TV_WaitForEvent    (int ch,int timeout,TV_Event *event);
int 	TWCALLFUNC TV_ClearEvent(int ch);
int 	TWCALLFUNC TV_GetEvent  (int ch,TV_Event *event);
int 	TWCALLFUNC TV_SetMonitorCalled(int ch,int Flag);
int 	TWCALLFUNC TV_SetNobodyTime   (int time);
int 	TWCALLFUNC TV_Buf2FSKFile(unsigned char *buf,int len,char *file);
int     TWCALLFUNC TV_SetRecAGC  (int ch,int bEnable);
void    TWCALLFUNC TV_SetExInDaV (int doit);

////////////////////////////////////////////////////////////////////////
//
//          TWI-(2/4/8/16)A        新增函数
//
//////////////////////////////////////////////////////////////////////////
//*******************************************************************
//在使用该统一接口时，针对8/16PF也能使用
void 	TWCALLFUNC TV_ConnectCardChannels   (int card, int ch);
void 	TWCALLFUNC TV_DisconnectCardChannels(int card, int ch);
void 	TWCALLFUNC TV_ConnectCardTo  (int card, int ch);
void 	TWCALLFUNC TV_ConnectChToCard(int ch, int card);
void 	TWCALLFUNC TV_DisconnectCard (int card);

int     TWCALLFUNC TV_GetCardNumOfCh(int ch);
int     TWCALLFUNC TV_GetChIndexOfCh(int ch);

int     TWCALLFUNC TV_GetCardType   (void);
int  	TWCALLFUNC TV_GetCardTypeEx (int ncard,int *chNumofthiscard,int *cardrealno);
//*******************************************************************

//0内线，1外线，2  空，3搭线，4声控
void    TWCALLFUNC TV_SetChannelType   (int ch,int type);
void    TWCALLFUNC TV_SetVoiceModel    (int value,int time);
void    TWCALLFUNC TV_SetVoltageModelEx(int ch,int offhookval1,int offhookval2,int hangupval1,int hangupval2 );
void    TWCALLFUNC TV_SetVoltageModel  ( int offhookval1,int offhookval2,int hangupval1,int hangupval2 );
void    TWCALLFUNC TV_SetTroubleVoltageEx(int ch,int val);
void    TWCALLFUNC TV_SetTroubleVoltage(int val);
int     TWCALLFUNC TV_GetExternalStatus(int ch);
int     TWCALLFUNC TV_GetLineVoltage   (int ch);
void    TWCALLFUNC TV_InitBasicVoltage (void);

void    TWCALLFUNC TV_SetRcvVoiceVolume (int ch,float val);
void    TWCALLFUNC TV_SetSendVoiceVolume(int ch,float val);

long 	TWCALLFUNC TV_PhoneStartPlayFile(int  card,        char * pchFileName,
										  long lStartByte,long   lPlayLen);
long    TWCALLFUNC TV_PhonePlayFileRest (int card);
long 	TWCALLFUNC TV_PhoneStopPlayFile (int card);
long 	TWCALLFUNC TV_PhoneStartPlay    (int card, char *pchBuffer, int Len);
long 	TWCALLFUNC TV_PhoneStopPlay     (int card);
long 	TWCALLFUNC TV_PhonePlayRest     (int card);
long 	TWCALLFUNC TV_MicStartRecordFile(int  card,        char * pchFileName,
										 long lStartByte,long   lRecordLen);
long 	TWCALLFUNC TV_MicRecordFileRest (int card);
long 	TWCALLFUNC TV_MicStopRecordFile (int card);
long 	TWCALLFUNC TV_MicStartRecord    (int card, char *pchBuffer, int Len);
long 	TWCALLFUNC TV_MicStopRecord     (int card);
long 	TWCALLFUNC TV_MicRecordRest     (int card);
long	TWCALLFUNC TV_PhonePlaySentence (int card, unsigned char *s);
long	TWCALLFUNC TV_PhonePlaySentenceRest (int card);
long 	TWCALLFUNC TV_PhoneStopPlaySentence (int card);

void 	TWCALLFUNC TV_SendCID_Ring(int ch,int type,char far *cid);

long 	TWCALLFUNC TV_MicStartRecordWaveFile(int card,char *pchFileName);
long 	TWCALLFUNC TV_MicRecordWaveFileRest (int card);
long 	TWCALLFUNC TV_MicStopRecordWaveFile (int card);
long 	TWCALLFUNC TV_PhoneStartPlayWaveFile(int card,char *pchFileName);
long 	TWCALLFUNC TV_PhonePlayWaveFileRest (int card);
long 	TWCALLFUNC TV_PhoneStopPlayWaveFile (int card);

long    TWCALLFUNC TV_DStartRecord(int ch, char *pchBuffer, int Len);
long    TWCALLFUNC TV_DStartRecord2(int ch, char *pchBuffer, int Len);
long    TWCALLFUNC TV_DRecordRest (int ch);
long    TWCALLFUNC TV_DStopRecord (int ch);
long 	TWCALLFUNC TV_StartPlay2  (int ch, char *pchBuffer, int Len);

//为了获取语音卡驱动的日期，其返回值包括了动态库，驱动和Dsp程序的时间
//其中cardtype为0,表示取8A相关信息，如果是1则表示取16A的信息
char*   TWCALLFUNC TV_GetSysVersion(int cardtype,char *buffer); 

//该函数是上面发送FSK数据函数的封装版，第一个参数为通道号，第二个参数为信息类型，常用的为0x84
//和0x87，第三个参数为用户要发送的数据，这儿用户不用对要发送的数据添加信息头和校验位，应该直接填写
int     TWCALLFUNC TV_SendDataAsFSK(int ch,int type,unsigned char* buf);
UCHAR*  TWCALLFUNC TV_MixVoice   (UCHAR** MixData,int nMixNum,UCHAR* buffer,int nbuffersize);

int     TWCALLFUNC TV_GetPhoneIDTotal(int ch,char* rcvbuf,int len);
void    TWCALLFUNC TV_ClearPhoneID(int ch);
int     TWCALLFUNC TV_DetectBargeIn(int ch);
//voicevalue (1-4095),sensitivevalue(>1即可，其值×16ms表示连续多长时间测到才认为有语音)
//所有参数均是越小越灵敏，但也容易误判。默认值分别为200，6，2
//外线呼叫时必须等到对方摘机才可以，否则因为有回铃音，容易误判
int     TWCALLFUNC TV_SetBargeInParam(int ch,int voicevalue, int sensitivevalue);
int     TWCALLFUNC TV_MonitorChannelTalking(int talkingch,int monitorch);

//USB-8A 专用函数
void    TWCALLFUNC TV_SetWatchdog();
int     TWCALLFUNC TV_GetCardStatus();
int     TWCALLFUNC TV_GetCardStatusEx(int ncard);
void    TWCALLFUNC TV_GetUserSymbol(int ncard,char far* symbolstr);
int     TWCALLFUNC TV_SetBiSignalFreq(int ch,int sDialfreq1,int sDialfreq2,int sRingbackfreq1,int sRingbackfreq2, int sBusyonefreq1,
									  int sBusyonefreq2,int sBusytwofreq1,int sBusytwofreq2,int sUserfreq1,int sUserfreq2);
int     TWCALLFUNC TV_SetSendFaxSpeed(int ch,int speed); //speed:0;快速;非0,精细
int     TWCALLFUNC TV_GetSendFaxSpeed(int ch);
void    TWCALLFUNC TV_SetDetectSignalLevel(int ch,int nlevel);
int     TWCALLFUNC TV_SetSendFaxSpeed(int chno,int speed);
int     TWCALLFUNC TV_GetSendFaxSpeed(int chno);
int     TWCALLFUNC TV_OffHookCheck(int ch);
void	TWCALLFUNC TV_SetCardSendVolume(int ncard,float nvolume);
void	TWCALLFUNC TV_SetCardRcvVolume(int ncard,float nvolume);
int		TWCALLFUNC TV_SwitchEchoCancel(int ch,int benable);
//为GSM模块卡添加函数
int     TWCALLFUNC TV_ChannelTypeGSM(int ch);
int		TWCALLFUNC TV_StartSendShortMessage(int ch,char* phonenumber,char* sms);
//注意，该pdu字符串不包括编码格式，日期时间和实际内容长度，固定按照汉字编码进行发送
int     TWCALLFUNC TV_StartSendShortMessageEx(int ch,char* pdustr,char*sms,int servlen);
int     TWCALLFUNC TV_SendShortMessageRest(int ch);
int		TWCALLFUNC TV_StartRcvShortMessage(int ch,char *rcvbuff,int bufflen,char* phonenumber,int phlen);
int		TWCALLFUNC TV_RcvShortMessageRest(int ch);
void    TWCALLFUNC TV_SetMsgServiceCenter(int ch,char* msgcenter,int bsave);
int		TWCALLFUNC TV_SetTransferCall(int ch,int reason,int mode,char* phnumber);
int     TWCALLFUNC TV_SendATCommand(int ch,char* buf);
int     TWCALLFUNC TV_SendATRest(int ch);
int     TWCALLFUNC TV_ClearATCommand(int ch);
int     TWCALLFUNC TV_ReceiveATCommand(int ch,char* buf,int buflen);
int     TWCALLFUNC TV_QuerySignalStrengthLevel(int ch);
int     TWCALLFUNC TV_GetSignalStrengthLevel(int ch, int *signallevel);
int		TWCALLFUNC TV_GetHangupReason(int ch);
int		TWCALLFUNC TV_GetGsmDialStatus(int ch);
void    TWCALLFUNC TV_SetChannelUseForInternet(int ch);
int     TWCALLFUNC TV_ReceiveATCommandEx(int ch,char* buf,int buflen,int atid);
int     TWCALLFUNC TV_AnalyseMsgForMmsInfo(char* msgtoanalyse,int* mmsexpirytime,int* mmslen,char* mmstransID,int tranidlen,char* outurlbuf,int urlbuflen);
int     TWCALLFUNC TV_DetectShortMsgorPhoneWhenInnet(int ch);
int     TWCALLFUNC TV_GetComNoFromCh(int ch ,char* comno);
int     TWCALLFUNC TV_IsGsmChannelInitOver(int ch);
/************************************************************************/
/*                       统一接口添加的函数                             */
/************************************************************************/
int     TWCALLFUNC TV_GetLastErrorEx(int ntype);//ntype = 2:8A;1:16A;0:16(8)PF
int     TWCALLFUNC TV_GetAllCardTypesInSystem(int *card8a,int *ch8a,int *card16a,int *ch16a,int *card16p,int *ch16pf);//返回值表示有几种卡，最大为3
int     TWCALLFUNC TV_GetCardTypeOfCh(int ch);
int     TWCALLFUNC TV_GetCardTypeOfCard(int card);
int     TWCALLFUNC TV_GetCardNumOfCgroup(int group);
int     TWCALLFUNC TV_GetChIndexOfCgroup(int group);

#ifdef __cplusplus
}
#endif

#endif
