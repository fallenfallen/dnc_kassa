#ifndef ERROR_STRINGS_LOG
#define ERROR_STRINGS_LOG


#include <malloc.h>
#include <qtextcodec.h>
#include "debug_level_log.h"
#include "upload_const.h"
#include "addon_func.h"


//const char* WinCodec = "CP1251";

///extern DebugLevelLog pgsql_onscreanlog;
extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;

class ErStrLog
{
  public :
    ErStrLog();
    ~ErStrLog();
    int GetErrCount();
  void AddError(int error_type, int command_type = 0, int n_str = 0, int n_param = 0);

  private :
    int er_count;
    DebugLevelLog * er_log;
		
    void CommandTypeStr(int command_type, char * str);
    void ErrorTypeStr(int error_type, char * str, int * ert);
};

#endif
