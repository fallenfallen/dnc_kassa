
#ifndef PATH_CONST
#define CONF_WAY "/etc/hwsrv/hw.conf"
#define CODES_WAY "/etc/hwsrv/codes.tab"
#define TMP_WAY "/tmp/tmp.tab"
#define TEMP_WAY "/tmp/hw.temp"
#define SERIAL_WAY "/etc/hwsrv/serial"
#define ACTKEY_WAY "/etc/hwsrv/actkey"
#define SCROLL_WAY "/tmp/stop_scroll.flg"
#define LOG_WAY "/tmp/dancy/conf/confGUI_log"
#endif


int FileAcces(const char * file_path);
int  CreateListComPort (void);
int GetStrParamForCodes(int SchemeNum, char * PName, char * Res, int MaxResLen);
int GetCodesParam(char * PName, char * Res, int MaxResLen, int PNum);
int GetIntParam(char * PName, int * Res);
int GetStrSchemeParam(int SchemeNum, char * PName,  char *Res, FILE * fp);
int GetStrParam( char * PName,  char * Res, int MaxResLen,long POS, FILE * fp);
int GetSchemeParam(int SchemeNum, char * PName, int * Res, FILE * fp);
int GetIntParam1(char * PName, int * Res, long POS, FILE * fp);
int getStrParam(char * str,char * tmp_str);
FILE * OpenFile(const char * FName);
int CloseFile(FILE * fp);
char* int_to_str(int n);
