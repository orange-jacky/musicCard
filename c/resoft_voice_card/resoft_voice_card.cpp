#include "com_resoft_ResoftVoice.h"
#include "resoft_voice_card.h"

/*debug function*/
/*return s_true respresent success*/
int debug_init(char *filename){

	FILE *fp = NULL;
	if(NULL == filename) return filename_empty;

	fp = fopen(filename, "a");
	if(NULL == fp){
		printf("open %s failed. err is [%s].\n", filename, strerror(errno));
		return openfile_fail;
	}
	setbuf(fp, NULL);

	debugInfo.fileName = strdup(filename);
	debugInfo.fp = fp;
	debugInfo.outputType = debug_to_file; /*default write debug infor into log file*/

	return s_true;

}

void debug_write(const char *fmt, ...){

	time_t now;
	struct tm *tm_ptr;
	char dt[256];
    	va_list  args;

	time(&now);
	tm_ptr = localtime(&now);

	sprintf(dt, "[%d%02d%02d-%02d:%02d:%02d] %s", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday,
				 tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec,
				 "voice_debug:");

    	va_start(args, fmt);

	if(debugInfo.outputType == debug_to_file){
		fprintf(debugInfo.fp, "%s ", dt);
		vfprintf(debugInfo.fp, fmt, args);
	}else{
		printf("%s ", dt);
		vprintf(fmt, args);
	}

	va_end(args);

	return;
}

void debug_free(){

	if(debugInfo.fileName)
		free(debugInfo.fileName);
	if(debugInfo.fp)
		fclose(debugInfo.fp);
	return;
}

/*channel*/
/*return value respresent the number of channel*/
int channel_init_all(){
	int i;
	for(i=0; i < MAX_CHANNEL_NUMBER; i++)
		channel_init(i);

	//(*resoft_disable)();
	//Sleep(1000);
	channel_total = (*resoft_install)();
	if( channel_total <= 0){
		debug_write("error : driver is not installed. channel number is %d\n",channel_total);
	}else{
		debug_write("ok : driver is installed. avaiable channel number is %d\n",channel_total);
		for(i=0; i<channel_total; i++){
			switch((*resoft_channeltype)(i)){
				case CT_INTERNAL:
					debug_write("channe %d is 内线\n", i);
					break;
				case CT_EXTERNAL:
					debug_write("channe %d is 外线\n", i);
					break;
				default:
					debug_write("channe %d is unknow\n", i);
					break;			
			}
		}
	}
	/*initalize TV*/
	(*resoft_initalize)();

	return channel_total;
}

void channel_init(int chnlno){
	cInfo[chnlno].chn = chnlno;
	if(cInfo[chnlno].chnstate != cs_hangup ){
		(*resoft_hangupctrl)(chnlno);
	}
	cInfo[chnlno].chnstate = cs_hangup;
	return;
}

/*state equal cs_hangup, avialable*/
/*return avaiable channel number or not_find*/
int find_available_channel(){
	int i;
	for(i=0; i<channel_total; i++){
		if( avaiable[i] == s_avaiable)
			return i;
	}

	return not_find;
}

/*init inputParam*/
void init_inputParam(){
	int i;
	inParam.alertCode = NULL;
	inParam.summary = NULL;
	inParam.reserved1 = NULL;
	inParam.reserved2 = NULL;

	for(i=0; i<MAX_USERINFO_NUMBER ; i++)
		inParam.userInfo[i] = NULL;

	return;
}
/*get inputParam object, retrun value is number of userinfo*/
/*return userinfo number*/
int get_inputParam_oject(JNIEnv *env, jobject inputObj){

	int i;
	jclass inputClass;
	jfieldID fidAlertCode;
	jobject alertCode;
	jfieldID fidAlertSummary;
	jobject alertSummary;
	jfieldID fidAttribute1;
	jobject attribute1;
	jfieldID fidAttribute2;
	jobject attribute2;
	jclass cls;
	jmethodID midSize;
	jmethodID midGet;
	jfieldID fidAlertUserInfo;
	jobject alertUserInfo;
	jint size;
	jobject result;

	//get a referenc to input object's class
	inputClass = env->GetObjectClass(inputObj);
	
	//1
	//get tht Field ID of the instance variables "alertCode"
	fidAlertCode = env->GetFieldID( inputClass, "alertCode", "Ljava/lang/String;");
	if (NULL == fidAlertCode) return not_find;
	//get string given the Field ID
	alertCode = env->GetObjectField( inputObj, fidAlertCode);
	//get content, support chinese
	inParam.alertCode = jstringToWindows(env, (jstring)alertCode);

	//2
	//get tht Field ID of the instance variables "alertSummary"
	fidAlertSummary = env->GetFieldID( inputClass, "alertSummary", "Ljava/lang/String;");
	if (NULL == fidAlertSummary) return not_find;
	//get string given the Field ID
	alertSummary = env->GetObjectField( inputObj, fidAlertSummary);
	//get content, support chinese
	inParam.summary = jstringToWindows(env, (jstring)alertSummary);

	//3
	//get tht Field ID of the instance variables "attribute1"
	fidAttribute1 = env->GetFieldID( inputClass, "attribute1", "Ljava/lang/String;");
	if (NULL == fidAttribute1) return not_find;
	//get string given the Field ID
	attribute1 = env->GetObjectField( inputObj, fidAttribute1);
	//get content, support chinese
	inParam.reserved1 = jstringToWindows(env, (jstring)attribute1);
	
	//4
	//get tht Field ID of the instance variables "attribute2"
	fidAttribute2 = env->GetFieldID( inputClass, "attribute2", "Ljava/lang/String;");
	if (NULL == fidAttribute2) return not_find;
	//get string given the Field ID
	attribute2 = env->GetObjectField( inputObj, fidAttribute2);
	//get content, support chinese
	inParam.reserved2 = jstringToWindows(env, (jstring)attribute2);

	//5
	//get a class reference for java.util.ArrayList
	cls = env->FindClass("java/util/ArrayList");
	//get the method id of the constructor which take an size
	midSize = env->GetMethodID(cls, "size", "()I");
	if(midSize == NULL) return not_find;
	//get the method id of the constructor which take an get
	midGet = env->GetMethodID(cls, "get", "(I)Ljava/lang/Object;");
	if(midGet == NULL) return not_find;


	//get tht Field ID of the instance variables "alertUserInfo"
	fidAlertUserInfo = env->GetFieldID( inputClass, "alertUserInfo", "Ljava/util/ArrayList;");
	if (NULL == fidAlertUserInfo) return not_find;

	//get string given the Field
	alertUserInfo = env->GetObjectField( inputObj, fidAlertUserInfo);
	//get ArrayList size
	size = (jint)env->CallObjectMethod(alertUserInfo, midSize);
	//get all userinfo
	for(i=0; i<size ; i++){
		//get one userinfo
		result = env->CallObjectMethod(alertUserInfo, midGet, i);
		inParam.userInfo[i] = jstringToWindows(env, (jstring)result);
	}
	
	return size;

}
/*display inputParam*/
void display_inputParam(){

	int i;
	debug_write("----->%s\n", "display_inputParam");
	if(inParam.alertCode)
		debug_write("%s[%s]\n", "alertCode", inParam.alertCode);
	if(inParam.summary)
		debug_write("%s[%s]\n", "summary", inParam.summary);
	if(inParam.reserved1)
		debug_write("%s[%s]\n", "reserved1", inParam.reserved1);
	if(inParam.reserved2)
		debug_write("%s[%s]\n", "reserved2", inParam.reserved2);

	for(i=0; i<MAX_USERINFO_NUMBER ; i++){
		if(inParam.userInfo[i])
			debug_write("%s%d[%s]\n", "userInfo", i, inParam.userInfo[i]);
	}
	
	return;

}
/*free inputParam*/
void free_inputParam(){

	int i;
	if(inParam.alertCode)
		free(inParam.alertCode);
	if(inParam.summary)
		free(inParam.summary);
	if(inParam.reserved1)
		free(inParam.reserved1);
	if(inParam.reserved2)
		free(inParam.reserved2);

	for(i=0; i<MAX_USERINFO_NUMBER ; i++){
		if(inParam.userInfo[i])
			free(inParam.userInfo[i]);
	}

	return;
}


/*init outputResult*/
void init_outputResult(){

	int i;
	outResult.alertCode = NULL;
	outResult.reserved1 = NULL;
	outResult.reserved2 = NULL;

	for(i=0; i<MAX_USERINFO_NUMBER ; i++)
		outResult.dialResult[i] = NULL;

	return;

}
/*set outputResult object*/
int set_public_outputResult_object(JNIEnv *env, jobject outputObj){

	int len ;
	jclass outputClass;
	jfieldID fidAlertCode;
	jobject alertCode;
	jfieldID fidAttribute1;
	jobject attribute1;
	jfieldID fidAttribute2;
	jobject attribute2;
	jfieldID fidDialResult;
	jobject dialResult;

	jclass cls;
	jmethodID midAdd;
	jmethodID midInit;
	
	jstring str = NULL;
	char *ptr = NULL;

	//get a referenc to output object's class
	outputClass = env->GetObjectClass(outputObj);

	//1
	//get tht Field ID of the instance variables "alertCode"
	fidAlertCode = env->GetFieldID( outputClass, "alertCode", "Ljava/lang/String;");
	if (NULL == fidAlertCode) return not_find;
	
	if(inParam.alertCode)
		ptr = strdup(inParam.alertCode);
	if(ptr){
		outResult.alertCode = ptr;
		alertCode = (jobject)windowToJstring(env, ptr);
		env->SetObjectField(outputObj, fidAlertCode, alertCode);
		ptr = NULL;
	}
	
	//2
	//get tht Field ID of the instance variables "attribute1"
	fidAttribute1 = env->GetFieldID( outputClass, "attribute1", "Ljava/lang/String;");
	if (NULL == fidAttribute1) return not_find;
	if(inParam.reserved1)
		ptr = strdup(inParam.reserved1);
	if(ptr){
		outResult.reserved1 = ptr;
		attribute1 = (jobject)windowToJstring(env, ptr);
		env->SetObjectField(outputObj, fidAttribute1, attribute1);
		ptr = NULL;
	}

	//3
	//get tht Field ID of the instance variables "attribute1"
	fidAttribute2 = env->GetFieldID( outputClass, "attribute2", "Ljava/lang/String;");
	if (NULL == fidAttribute2) return not_find;
	if(inParam.reserved2)
		ptr = strdup(inParam.reserved2);
	if(ptr){
		outResult.reserved2 = ptr;
		attribute2 = (jobject)windowToJstring(env, ptr);
		env->SetObjectField(outputObj, fidAttribute2, attribute2);
		ptr = NULL;
	}

	//4
	//get a class reference for java.util.ArrayList
	cls = env->FindClass("java/util/ArrayList");
	//get the method id of the constructor which take an init
	midInit = env->GetMethodID(cls, "<init>", "()V");
	if(midInit == NULL) return not_find;
	//get the method id of the constructor which take an add
	midAdd = env->GetMethodID(cls, "add", "(Ljava/lang/Object;)Z");
	if(midAdd == NULL) return not_find;
	dialResult = env->NewObject(cls, midInit);
	if(dialResult == NULL) return not_find;

	//get tht Field ID of the instance variables "alertUserInfo"
	fidDialResult = env->GetFieldID( outputClass, "alertDialResult", "Ljava/util/ArrayList;");
	if (NULL == fidDialResult) return not_find;

	env->SetObjectField(outputObj, fidDialResult, dialResult);

	/*set global variable for thread*/
	arrayList = dialResult;
	addMethod = midAdd;

	return s_true;
}


int set_dialResult(JNIEnv *env, jobject obj, jmethodID jmid, char *str){

	int i = 1;
	env->CallBooleanMethod(obj, jmid, windowToJstring(env, str));
	return i;
}

/*display outputResult*/
void display_outputResult(){

	int i;
	debug_write("----->%s\n", "display_outputResult");
	if(outResult.alertCode)
		debug_write("%s[%s]\n", "alertCode", outResult.alertCode);
	if(outResult.reserved1)
		debug_write("%s[%s]\n", "reserved1", outResult.reserved1);
	if(outResult.reserved2)
		debug_write("%s[%s]\n", "reserved2", outResult.reserved2);

	for(i=0; i<MAX_USERINFO_NUMBER ; i++){
		if(outResult.dialResult[i])
			debug_write("%s%d[%s]\n", "dialResult", i, outResult.dialResult[i]);
	}
	
	return;
	
}
/*free outputResult*/
void free_outputResult(){

	int i;
	if(outResult.alertCode)
		free(outResult.alertCode);
	if(outResult.reserved1)
		free(outResult.reserved1);
	if(outResult.reserved2)
		free(outResult.reserved2);

	for(i=0; i<MAX_USERINFO_NUMBER ; i++){
		if(outResult.dialResult[i])
			free(outResult.dialResult[i]);
	}

	return;
}


/*dynamic library*/
int load_library(){

	int ret = s_true;
	char *dllname = "tw16vid.dll";
	char *dllname1 = "playch32.dll";

	if( !(dlh_tw16vid=LoadLibrary(dllname))){
		debug_write("Load %s faild. error %d\n", dllname, GetLastError());
		ret = not_find;
	}

	if( !(dlh_playch32=LoadLibrary(dllname1))){
		debug_write("Load %s faild. error %d\n", dllname1, GetLastError());
		ret = not_find;
	}

	return ret;
}
void unload_library(){

	FreeLibrary(dlh_tw16vid);
	FreeLibrary(dlh_playch32);
	return;
}

/*find function in tw16vid.dll */
int find_dial_number_function(){
	/*1*/
	if(!(resoft_install = (RESOFT_INSTALL)GetProcAddress(dlh_tw16vid,"TV_Installed"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_initalize = (RESOFT_INITIALIZE)GetProcAddress(dlh_tw16vid,"TV_Initialize"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}	

	if(!(resoft_timerelapsed = (RESOFT_TIMERELAPSED)GetProcAddress(dlh_tw16vid,"TV_TimerElapsed"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_startdial = (RESOFT_STARTDIAL)GetProcAddress(dlh_tw16vid,"TV_StartDial"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}
	

	if(!(resoft_stopdial = (RESOFT_STOPDIAL)GetProcAddress(dlh_tw16vid,"TV_StopDial"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}
	/*6*/
	if(!(resoft_dialrest = (RESOFT_DIALREST)GetProcAddress(dlh_tw16vid,"TV_DialRest"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}	

	if(!(resoft_hangupctrl = (RESOFT_HANGUPCTRL)GetProcAddress(dlh_tw16vid,"TV_HangUpCtrl"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_starttimer = (RESOFT_STARTTIMER)GetProcAddress(dlh_tw16vid,"TV_StartTimer"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}
	
	if(!(resoft_offhookctrl = (RESOFT_OFFHOOKCTRL)GetProcAddress(dlh_tw16vid,"TV_OffHookCtrl"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}	
	
	if(!(resoft_checksignal = (RESOFT_CHECKSIGNAL)GetProcAddress(dlh_tw16vid,"TV_CheckSignal"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}	

	/*11*/
	if(!(resoft_startmonitor = (RESOFT_STARTMONITOR)GetProcAddress(dlh_tw16vid,"TV_StartMonitor"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}
	
	if(!(resoft_monitoroffhook = (RESOFT_MONITOROFFHOOK)GetProcAddress(dlh_tw16vid,"TV_MonitorOffHook"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_listeneroffhook= (RESOFT_LISTENEROFFHOOK)GetProcAddress(dlh_tw16vid,"TV_ListenerOffHook"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_flushdtmf = (RESOFT_FLUSHDTMF)GetProcAddress(dlh_tw16vid,"TV_FlushDTMF"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_channeltype = (RESOFT_CHANNELTYPE)GetProcAddress(dlh_tw16vid,"TV_ChannelType"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_getlasterror = (RESOFT_GETLASTERROR)GetProcAddress(dlh_tw16vid,"TV_GetLastError"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_getlinevoltage = (RESOFT_GETLINEVOLTAGE)GetProcAddress(dlh_tw16vid,"TV_GetLineVoltage"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	if(!(resoft_disable = (RESOFT_DISABLE)GetProcAddress(dlh_tw16vid,"TV_Disable"))){
		debug_write("GetProcAddress faild. error %d\n",GetLastError());
		return not_find;
	}

	return s_true;
}


/*find function in playch32.dll */
int find_play_string_function(){

	if(!(resoft_passworddectect = (RESOFT_PASSWORDDECTECT)GetProcAddress(dlh_playch32,"PasswordDetect"))){
		debug_write("GetProcAddress faild. error %d\n", GetLastError());
		return not_find;
	}
	
	if(!(resoft_startplaych = (RESOFT_STARTPLAYCH)GetProcAddress(dlh_playch32,"TV_StartPlayCh"))){
		debug_write("GetProcAddress faild. error %d\n", GetLastError());
		return not_find;
	}
	
	if(!(resoft_playchrest = (RESOFT_PLAYCHREST)GetProcAddress(dlh_playch32,"TV_PlayChRest"))){
		debug_write("GetProcAddress faild. error %d\n", GetLastError());
		return not_find;
	}

	if(!(resoft_stopplaych = (RESOFT_STOPPLAYCH)GetProcAddress(dlh_playch32,"TV_StopPlayCh"))){
		debug_write("GetProcAddress faild. error %d\n", GetLastError());
		return not_find;
	}

	return s_true;	
}

/*return time waiting for offhook or offhook_timeout flag*/
int dial_number(int chlno, char *userinfo){

	int iret, sigcount, siglen;
	static time_t stime;
	time_t utime;
	char *de = "|";
	char *buff = NULL, *number = NULL, *name = NULL;
	
	if(userinfo == NULL) return string_empty;
	buff = strdup(userinfo);

	if(buff){
		name = strtok(buff, de);
		number = strtok(NULL, de);	
	}

	if(NULL == name){
		//debug_write("user name is empty\n");
		return string_empty;
	}
	
	if(NULL == number){
		//debug_write("dial number is empty\n");
		return string_empty;
	} 

	while(1){

		switch(cInfo[chlno].chnstate)
		{
			case cs_hangup:
				cInfo[chlno].chnstate = cs_startcallout; 
				(*resoft_hangupctrl)(chlno);
				break;
			case cs_callouting:
				return iret;
			case cs_startcallout:
				(*resoft_offhookctrl)(chlno);
				(*resoft_starttimer)(chlno, 3);
				cInfo[chlno].chnstate = cs_waitdialsig;
				break;
			case cs_waitdialsig:
				if( (*resoft_timerelapsed)(chlno) < 0)
				{
					(*resoft_hangupctrl)(chlno);
					(*resoft_starttimer)(chlno, 3);
					cInfo[chlno].chnstate = cs_callouterr;
					break;
				}
				iret=(*resoft_checksignal)(chlno, &sigcount, &siglen);
				if(iret == SIG_DIAL)
				{
					iret=(*resoft_startdial)(chlno, number);
					cInfo[chlno].chnstate = cs_dial;
				//	debug_write("拨号中...\n");
				}
				break;
			case cs_dial:
				iret=(*resoft_dialrest)(chlno);
				if(!iret)
				{
					(*resoft_stopdial)(chlno);
					(*resoft_starttimer)(chlno, 2);
					cInfo[chlno].chnstate= cs_waitoffhook;
				//	debug_write("拨号完毕，等待摘机\n");
					(*resoft_startmonitor)(chlno);
					stime = time(NULL);
				}

				break;
			case cs_waitoffhook:
				if( (*resoft_timerelapsed)(chlno) < 0)
				{
					utime = (int)difftime(time(NULL),stime);
					//debug_write("wait for offhook, start time=%ul,wait time=%d\n", stime, utime);

					if(utime < MAX_OFFHOOK_TIMEOUT){
						/*
							iret = (*resoft_monitoroffhook)(chlno, 25);
						*/
						iret = (*resoft_listeneroffhook)(chlno);//iret=0 not offhook  inet not equal 0 have offhook
						if(iret)
						{
							utime = (int)difftime(time(NULL),stime);
							debug_write("打通后，等待摘机耗时%ds\n", utime);
							cInfo[chlno].chnstate =cs_callouting;						
							iret = utime;
							//	debug_write("摘机\n");
						}
					}else{
						cInfo[chlno].chnstate =cs_timeout;						
						iret = offhook_timeout;
						//	debug_write("等待摘机超时\n");
					}
					
				}	
				break;
			case cs_callouterr:
				if( (*resoft_timerelapsed)(chlno) < 0)
				{
					channel_init(chlno);
				}
				break;
			case cs_timeout:
					channel_init(chlno);
				return iret;/*exit dial_number*/
			}
		
			Sleep(100);	
	}

	return iret;
}

/*return time of playing string or play_timeout flag*/
int play_string(int chlno, char *str){

	int i, j;
	char password[20];
	char path[100];
	static time_t stime;
	time_t utime;
	int signal,count,unused;
	int iret ,state = 1;
	static long TotalPlay = 0 ;

	FILE *fp = NULL;
	fp = fopen("password", "r");
	if(fp == NULL){
		debug_write("无法找到 PASSWORD 文件\n");
		return openfile_fail;
	}
	
	strcpy(password, "");
	strcpy(path, "");

	fscanf(fp, "%s", password);

	while(fgetc(fp) != 10);
	fscanf(fp, "%s", path);
	if(fp)
		fclose(fp);

	//debug_write("password is %s\n", password);
	//debug_write("data file path is %s\n", path);

	if( (j = (*resoft_passworddectect)(password, path)) < 0){
		debug_write("语音库密码错误. 返回值是%d\n", j);
		return verify_pw_fail;
	}
	
	while(state){
	
		switch(state){

		case 1:
			//debug_write("通道正在放音\n");
			stime = time(NULL);
			TotalPlay=(*resoft_startplaych)(chlno, str);
			//debug_write("播放字符串长度是%l\n", TotalPlay);
			if(TotalPlay<0) {
				debug_write("通道:%d 字符串放音失败 %d\n", chlno, (*resoft_getlasterror)());
				return play_string_fail;
			}
			
			state = 2;
			break;
		case 2:
			utime = (int)difftime(time(NULL),stime);
			//debug_write("play string, start time=%ul,play time=%d\n", stime, utime);

			if(utime < MAX_PLAY_TIMEOUT){
				signal=(*resoft_checksignal)(chlno,&count,&unused);
				i=(*resoft_playchrest)(chlno);
				//debug_write("还剩%d  个字符串需要播放\n", i);
				if(i > 0){
					/*do nothing*/
				}else{
					(*resoft_stopplaych)(chlno);
					utime = (int)difftime(time(NULL),stime);
					debug_write("finish play string,耗时=%ds\n", utime);
					iret = utime;
					state = 0;
				}
			}else{	
					(*resoft_stopplaych)(chlno);
					iret = play_timeout;
					state = 0;
			}	
			break;
		}
		Sleep(100);
	}

	return iret;
}


/*check dial line inserted or not*/
/*channel voltage >= 20  respresent insert dial line , or not insert dial line*/
int check_insert_dialline(int chlno){

	int isInsert , i ;
	if( (i=(*resoft_getlinevoltage)(chlno)) >= 20 ){
		isInsert = s_avaiable;
	}else{
		isInsert = s_not_avaiable;
	}
	debug_write("channel %d voltage is %d\n", chlno, i);
	return isInsert;
}


/*support chinese*/
char* jstringToWindows(JNIEnv* env, jstring jstr){

	jsize len ;
	int size = 0;
	const jchar *jcstr;
	char *str = NULL;

	len	=  env->GetStringLength(jstr);
	jcstr = env->GetStringChars(jstr, NULL);
	str =(char *)malloc(len*2 +1);
	if(str == NULL) return NULL;

	if( (size = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(jcstr), len, str, len*2 + 1, NULL, NULL)) == 0)
		return NULL;
	env->ReleaseStringChars(jstr, jcstr);
	str[size] = 0;
	return str;

}

jstring windowToJstring(JNIEnv* env, char* str){

	jstring rtn = 0;
	int slen = strlen(str);
	jchar *buffer = NULL;

	if(slen == 0){
		rtn = env->NewStringUTF(str);
	} 
	else{
		int length = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, NULL, 0);
		buffer = (jchar*)malloc(length*2 + 1);
		if(MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, (LPWSTR)buffer, length) > 0)
			rtn = env->NewString(buffer, length);
	}

	if(buffer)
		free(buffer);

	return rtn;
}

/*thread call function*/
DWORD WINAPI ThreadFunc(LPVOID pThreadInParam){
	int chno;
	int ret;
	pThreadInputParam pInPar = (pThreadInputParam)pThreadInParam;
	debug_write("start handle %s\n", pInPar->userinfo);
	while(1){

		EnterCriticalSection(&critical_section);
		chno = find_available_channel();
		if(chno != not_find) avaiable[chno] = s_not_avaiable;
		LeaveCriticalSection(&critical_section);

		if(chno == not_find){
			Sleep(100);
		}else{

			char *de = "|";
			char *buff = NULL,  *name = NULL; 
			buff = strdup(pInPar->userinfo);
			if(buff){
				name = strtok(buff, de);
			}
			
			char arr[1024];
			sprintf(arr,"%s,%s", name ? name : "", pInPar->summary);
			debug_write("channel [%d] start dial [%s]\n", chno, pInPar->userinfo);
			pInPar->chnlno = chno;
			ret = dial_number(chno, pInPar->userinfo);
			debug_write("channel [%d] dial [%s], and the dial return state is [%d]\n", chno, pInPar->userinfo, ret);
			if(ret == string_empty || ret == offhook_timeout){
				goto err;
			}
			
		
			debug_write("channel [%d] start play string [%s]\n", chno, arr);
			ret = play_string(chno,arr);
			debug_write("channel [%d] play string [%s], and the play string return state is [%d]\n", chno, arr, ret);
		
err:			
			char b[1024];
			sprintf(b, "%s|%d", pInPar->userinfo, ret);
			set_dialResult(pInPar->env, arrayList, addMethod, b);
			debug_write("write output %s\n", b );
			channel_init(chno);		
			if(buff)free(buff);

			EnterCriticalSection(&critical_section);
			avaiable[chno] = s_avaiable;
			LeaveCriticalSection(&critical_section);
			
			break;
		}
	}
	
	debug_write("finish handle %s\n", pInPar->userinfo);
	return ret;
}


/*
 * Class:     com_resoft_ResoftVoice
 * Method:    resoftJniCall
 * Signature: (Lcom/resoft/InputParam;Lcom/resoft/OutputResult;)I
 */
JNIEXPORT jint JNICALL Java_com_resoft_ResoftVoice_resoftJniCall
(JNIEnv *env, jobject thisObj, jobject inputObj, jobject outputObj){
	
	int usernums = 0, i, diallines = 0;
	
	InitializeCriticalSection(&critical_section);

	if(debug_init(DEBUG_FILE) != s_true){
		printf("dll init log file failed.\n");
		DeleteCriticalSection(&critical_section);
		return s_false;
	} 
		
	FILE *cfp = NULL;
	cfp = fopen(CFG_FILE, "r");
	if(NULL == cfp){
		debug_write("配置文件%s 不存在\n", CFG_FILE);
		DeleteCriticalSection(&critical_section);
		debug_free();
		return s_false;
	}	

	/*load resource*/
	if(load_library() == not_find) {

		debug_free();
		if(cfp) fclose(cfp);
		DeleteCriticalSection(&critical_section);
		return s_false;
	}
	if(find_dial_number_function() == not_find) {
		unload_library();
		debug_free();
		if(cfp) fclose(cfp);
		DeleteCriticalSection(&critical_section);
		return s_false;
	}
	if(find_play_string_function() == not_find) {
		unload_library();
		debug_free();
		if(cfp) fclose(cfp);
		DeleteCriticalSection(&critical_section);
		return s_false;
	}

	if(channel_init_all() <= 0 ) {
		unload_library();
		debug_free();
		if(cfp) fclose(cfp);
		DeleteCriticalSection(&critical_section);
		return s_false;
	}

	/*default all avaiable channel have not inserted dial line*/
	for(i=0; i<MAX_CHANNEL_NUMBER; i++){
		avaiable[i] = s_not_avaiable;
	}
	/*according to fact,if the channel not inserted dial line, set the value s_not_avaiable*/
//	for(i=0; i< channel_total; i++){
//		avaiable[i] = check_insert_dialline(i);
//		debug_write("channel %d %s dial line\n", i, avaiable[i] ? "insert" : "not insert");
//	}
	
	/*read cfg file*/
	char line[1024];
	char *ptr = line, *ptr1 = NULL;
    while (fgets(ptr, 1024, cfp) != NULL) {
		if(line[0]=='#' || line[0]=='\0')continue;
		ptr1 = strchr(ptr, '[');
		if(ptr1) {
			ptr1++; 
			i=(int)*ptr1 - 48;
		//	debug_write("i=%d\n", i);
			ptr1 = strchr(ptr1, '=');
			if(ptr1){
				ptr1++;
				if(!strncmp(ptr1, "on", 2)) avaiable[i] = s_avaiable;
			}
		}
    }

	
	for(i=0; i< channel_total; i++){
		debug_write("channel %d %s dial line\n", i, avaiable[i] ? "insert" : "not insert");
		if(avaiable[i] ==  s_avaiable) diallines++;
	}

	/*all channel not insert dial line*/
	if(diallines == 0){
		debug_write("%s\n", "all channel have not inserted dial line");
		unload_library();
		debug_free();
		if(cfp) fclose(cfp);
		DeleteCriticalSection(&critical_section);

		return s_false;
	}

	/*handle input*/
	init_inputParam();
	usernums = get_inputParam_oject(env, inputObj);
	display_inputParam();

	/*handle output*/
	init_outputResult();
	set_public_outputResult_object(env, outputObj);
	display_outputResult();

	/*thread handle*/
	DWORD threadIds[MAX_THREADS];
	HANDLE hThreads[MAX_THREADS];
	pThreadInputParam pInputParams[MAX_THREADS];
	
	for(i=0; i<MAX_THREADS; i++){
		threadIds[i] = 0;
		hThreads[i] = NULL;
		pInputParams[i] = NULL;
	}
	for(i=0; i<usernums; i++){
		pInputParams[i] = (pThreadInputParam)malloc(sizeof(threadInputParam));
		if(pInputParams[i] == NULL){
			debug_write("%s\n", "alloc threadInputParam %d memory failed.", i);
			continue;
		}
		
		pInputParams[i]->chnlno = -1;
		pInputParams[i]->userind = i;
		pInputParams[i]->userinfo = inParam.userInfo[i];
		pInputParams[i]->summary = inParam.summary;
		pInputParams[i]->env = env;

		hThreads[i] = CreateThread(NULL, NULL, ThreadFunc, (LPVOID)pInputParams[i], 0, &threadIds[i]);
		if(hThreads[i] == NULL){
			debug_write("%s\n", "create thread %d failed.\n", i);
			continue;
		}
	}
	/*wait all thread finish*/
	if(usernums > 0){
		WaitForMultipleObjects(usernums, hThreads, 1, INFINITE);
		debug_write("%s\n", "all thread finished");
	}else{
		debug_write("%s\n", "no userinfo");
	}
	/*free resource*/
	for(i=0; i<usernums; i++){
		if(hThreads[i] != NULL) CloseHandle(hThreads[i]);
		if(pInputParams[i] != NULL){
			free(pInputParams[i]);
			pInputParams[i] = NULL;
		}
	}

	//(*resoft_disable)();
	free_outputResult();
	free_inputParam();
	unload_library();
	debug_free();
	if(cfp) fclose(cfp);

	DeleteCriticalSection(&critical_section);

	return s_true;

}
