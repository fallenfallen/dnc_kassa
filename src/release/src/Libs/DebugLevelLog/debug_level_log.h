#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifndef LOG_WITH_DEBUG_LEVEL
#define LOG_WITH_DEBUG_LEVEL

#define DEFAULTIDENT "DancyProcess"
#define DEFAULTLOGPATH "./onsrean"
#define ERSTR "ERROR Cannot open configuration file "

typedef enum { INFO = 0, WARN = 1, ERROR = 2, FATAL = 3} MESSAGE_TYPES_ENUM;
typedef enum { NODEBUG = 0, LOGFILE = 1, STDOUT = 2} DEBUG_LEVEL_ENUM;

//typedef enum { D = 1, M = 2, Y = 4, T = 8} TIME_LABEL_ENUM;
#define DDAY 1
#define MMONTH 2
#define YYEAR 4
#define TTIME 8

class DebugLevelLog
{
  private :
    char * process_ident;
    char * logpath;
	
    //bool def_name;
    bool create_log_flag;
 
    bool archive_flag;
	 bool print_time;

  public :
    	DebugLevelLog();
	DebugLevelLog(const char * process_id, const char * log_path, bool arch_flag);
	~DebugLevelLog();
	
        //void SetArchiveFlag(bool arch_flag);
	void SetLogPath(const char * log_path);
	char * GetLogPath(); 
	//void SetProcessIdent(char * process_id);
   void CreateLog(const char * log_path, const char * process_id, bool arch_flag);
	void PrintTime(const DEBUG_LEVEL_ENUM);
	void PrintString(const DEBUG_LEVEL_ENUM debug_level, const MESSAGE_TYPES_ENUM msg_type,  const char* format_str, ...);
	void SetMode( bool aprint_time );

  private :
	FILE* OpenLog(void);
	void ArchiveLogFile(const char * logpath);
	void CreateDateString(const char * str_time, char *timelabel, int timelabelopt);
	
};

#endif
