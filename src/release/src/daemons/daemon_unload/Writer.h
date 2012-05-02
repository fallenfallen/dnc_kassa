#ifndef WRITER
#define WRITER

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h> 
#include "debug_level_log.h"
#include <qtextcodec.h>
 
const DEBUG_LEVEL_ENUM log_type = LOGFILE;
const DEBUG_LEVEL_ENUM user_log_type = LOGFILE;

extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;

extern char log_str[2000];

class Writer 
{                         
   public:
     Writer();
     ~Writer();
     static int FileUnloadSymbolLen();
     static void FileUnloadSymbolAppend(char* str);
     static void FileUnloadSymbolRewrite(char* str);
     //-----(*)-----Zuskin-----19/05/2011-----
     //static int IncReportNumber(int n, char* fileunload_path);
     static int IncReportNumber(int n, char* fileunload_path, int protocol);
     //static int WriteHeader(FILE * f_unload, int report_number, int WS_number);
     static int WriteHeader(FILE * f_unload, int report_number, int WS_number, int protocol, char* comm_str, char* param_str, int count);
     static int WriteFooter(FILE * f_unload, int protocol);
     //---------------------------------------
     static int SaveLastTrans(int num);
     static int WriteConfig(int report_num, int num_last_trans);
  private:
     static void get_temp_file_path(const char* base_path, char* temp_path);   
};

#endif
