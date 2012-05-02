/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "debug_level_log.h"

//========================================================================//
DebugLevelLog::DebugLevelLog()
{
//puts("---DebugLevelLog::DebugLevelLog()---");
  create_log_flag = false;
  
  process_ident = NULL;
  logpath = NULL;
	print_time = true;

 /* process_ident = (char *) calloc(strlen(DEFAULTIDENT) + 1, sizeof(char));
  strcpy(process_ident, DEFAULTIDENT);

  logpath = (char *) calloc(strlen(DEFAULTLOGPATH) + 5, sizeof(char));
  strcpy(logpath, DEFAULTLOGPATH);
  strcat(logpath, ".log");

  archive_flag = false;

  ArchiveLogFile(logpath);
  def_name = true;*/

//printf("logpath = %s \n", logpath);

//puts("---DebugLevelLog::DebugLevelLog()---end---");




}
//========================================================================//
DebugLevelLog::DebugLevelLog(const char * process_id, const char *log_path, bool arch_flag)
{
 //def_name = false;
 //puts("DebugLevelLog::DebugLevelLog(const char * process_id, const char *log_path, bool arch_flag)");  
 //printf("DebugLevelLog::DebugLevelLog(process_id = %s, log_path = %s )\n", process_id, log_path);

  logpath = NULL;
  process_ident = NULL;
  
  process_ident = (char *) calloc(strlen(process_id) + 1, sizeof(char));
  strcpy(process_ident, process_id);
  
  
  logpath = (char *) calloc(strlen(log_path) + 5, sizeof(char));
  strcpy(logpath, log_path);
  archive_flag = arch_flag;
  ArchiveLogFile(logpath);
  print_time = true;
  create_log_flag = true;

//printf("logpath = %s \n", logpath);

//puts("DebugLevelLog::DebugLevelLog(const char * process_id, const char *log_path, bool arch_flag)   ------ end");    
}
//========================================================================//
DebugLevelLog::~DebugLevelLog()
{
//puts("DebugLevelLog::~DebugLevelLog()");
  if(process_ident != NULL) free(process_ident);
  if(logpath != NULL) free(logpath);
//puts("DebugLevelLog::~DebugLevelLog() -- end");
}
//========================================================================//
void DebugLevelLog::CreateLog(const char * log_path, const char * process_id, bool arch_flag)
{
//printf("void DebugLevelLog::CreateLog(log_path = %s, process_id = %s) \n", log_path, process_id);
  archive_flag = arch_flag;
  
  if(process_ident != NULL) free(process_ident);
  
  process_ident = (char *) calloc(strlen(process_id) + 1, sizeof(char));
  strcpy(process_ident, process_id);
  
  if(logpath != NULL) free(logpath);
  
  logpath = (char *) calloc(strlen(log_path) + 5, sizeof(char));
  strcpy(logpath, log_path);
  //strcat(logpath, ".log");
  
  if(!create_log_flag)    ArchiveLogFile(logpath);

  create_log_flag = true;	


//printf("logpath = %s \n", logpath);
//puts("void DebugLevelLog::CreateLog(const char * log_path, const char * process_id, bool arch_flag) --- end");  
}
//========================================================================//
void DebugLevelLog::SetMode( bool aprint_time ) { 

 print_time = aprint_time;  

}
//========================================================================//
char* DebugLevelLog::GetLogPath() { 

	return logpath; 
}
//========================================================================//
void DebugLevelLog::SetLogPath(const  char * log_path)
{
//printf("void DebugLevelLog::SetLogPath(log_path = %s)\n", log_path);
  //if(def_name)
  //  remove(logpath); //Удаляем файл с именем поумолчанию!!!!!!!!!

  if(logpath != NULL) free(logpath);

  logpath = (char *) calloc(strlen(log_path) + 5, sizeof(char));
  strcpy(logpath, log_path);
  //strcat(logpath, ".log");


//  if(!create_log_flag)
 //   ArchiveLogFile(logpath);

//  CreateHeader();	//

  create_log_flag = true;	

//printf("logpath = %s \n", logpath);
//puts("void DebugLevelLog::SetLogPath(const  char * log_path) --- end");
}
 /*
//========================================================================//
void DebugLevelLog::SetArchiveFlag(bool arch_flag)
{
  archive_flag = arch_flag;
}

//========================================================================//
void DebugLevelLog::SetProcessIdent(char * process_id)
{
  if(process_ident != NULL) free(process_ident);

  process_ident = (char *) calloc(strlen(process_id) + 1, sizeof(char));
  strcpy(process_ident, process_id);
}
*/

//========================================================================//
void DebugLevelLog::PrintString(const DEBUG_LEVEL_ENUM debug_level, const MESSAGE_TYPES_ENUM msg_type, const char* format_str, ...)
{
   //printf("void DebugLevelLog::PrintString(format_str = <%s>) \n", format_str);

   if ( print_time )  PrintTime(debug_level);

   if(create_log_flag)  {
	FILE * log = NULL;
	char * type_str = (char *) calloc(11, sizeof(char));
   	
   	switch(msg_type)   {
 	     case WARN :
 	       strcpy(type_str, "WARNING : ");
 	       break;
 	     case ERROR :
 	       strcpy(type_str, "ERROR : ");
 	       break;
 	     case FATAL :
 	       strcpy(type_str, "FATAL : ");
 	       break;
 	     case INFO :
 	       break;
 	}
	
	switch(debug_level)    {
      	    case LOGFILE :
      		log = OpenLog();
      		if (log != NULL)        {
	    	    fprintf(log, type_str);
	            va_list param;
	            va_start(param, format_str);
	            vfprintf(log, format_str, param);
          	    va_end(param);        
		    fclose(log);
		}   
   	        break;
	     case STDOUT :
   	        printf(type_str);
   	        va_list param;
	    	va_start(param, format_str);
	        vprintf(format_str, param);
	        va_end(param);
	        break;	    
	}
	
	free(type_str);
    }
    //puts("DebugLevelLog :: PrinString ----- end");
}

//========================================================================//
void DebugLevelLog::CreateDateString(const char * str_time, char * timelabel, int timelabelopt = 7)
//Под параметр  datestr должно быть выделено не менее DDAY - 2 + 1  +  MMONTH - 3 + 1  +  YEAR - 4 +1  +  TTIME - 8 + 1 символов в памяти перед вызовом функции
// всего 21
// timelabelopt: [DDAY] | [MMONTH] | [YYEAR] | [TTIME]
{
//puts("void DebugLevelLog::CreateDateString(char * str_time, char *datestr, int timelabelopt)");
//  char * str = (char *) calloc(30, sizeof(char));
  if(str_time != NULL)
  {
    char * ptr; // Будет указывать на символ в строке, начиная с которого копировать
    if((timelabelopt & DDAY) == DDAY)
    {
      ptr = (char *)(str_time + 8); // День
      strncat(timelabel, ptr, 2);
      if(*timelabel == ' ')
        *timelabel = '0';
    }		
    if((timelabelopt & MMONTH) == MMONTH)
    {
      if(strlen(timelabel) != 0) strcat(timelabel, "-");
      ptr = (char *)(str_time + 4); // Месяц
      strncat(timelabel, ptr, 3);
    }
    if((timelabelopt & YYEAR) == YYEAR)
    {
      if(strlen(timelabel) != 0) strcat(timelabel, "-");
      ptr = (char *)(str_time + 20); //Год
      strncat(timelabel, ptr, 4);
    }
    if((timelabelopt & TTIME)== TTIME)
    {
      if(strlen(timelabel) != 0) strcat(timelabel, "-");
      ptr = (char *)(str_time + 11); //Время
      strncat(timelabel, ptr, 8);
    }
  }
//puts("DebugLevelLog::CreateDateString ---- end");
}

//========================================================================//
void DebugLevelLog::ArchiveLogFile(const char * logpath)
//arch_flag = true -- архив создается вне зависимости от даты создания файла
{
//puts("void DebugLevelLog::ArchiveLogFile(const char * logpath)");
  bool arch_flag = archive_flag;

  int n = access(logpath, F_OK);

  if (n == 0)  {

    char * mod_date_str = (char *) calloc(30, sizeof(char));
     //Формирование строки с датой последней модификации файла
    struct stat buffer;
    if(stat(logpath, &buffer) == 0)
    {       
      char * modify_time = (char *) calloc(30, sizeof(char));
      strcpy(modify_time, ctime(&buffer.st_mtime));
      CreateDateString(modify_time, mod_date_str);
      free(modify_time);
    }

    // Надо сравнить дату последнего изменения существующего файла и текущую дату
    if(!arch_flag) 
    {
      char * cur_date_str = (char *) calloc(30, sizeof(char));
      char * current_time = (char *) calloc(30, sizeof(char));
      time_t timv = time(0);
      strcpy(current_time, ctime(&timv));
      CreateDateString(current_time, cur_date_str);
      free(current_time);
      // Если даты разные, то архивируем
      if(strcmp(cur_date_str, mod_date_str) != 0) 
        arch_flag = true;

      free(cur_date_str);
    }

    if(arch_flag)
    {
      char * archlogpath;
      // Находим послений символ ' / ' и запоминаем указатель на начало имени файла в пути
      const char * ptr_path = strrchr(logpath,'/');
      if(ptr_path == NULL)
        ptr_path = logpath;
      else
        ptr_path = (char *) (ptr_path + 1);
      // Подсчитываем длину пути
      n = strlen(logpath) - strlen(ptr_path);
      if (n != 0)
      {
        archlogpath = (char*) calloc (n  + strlen(ptr_path) + strlen(mod_date_str) + 1, sizeof(char)); 
        strncat(archlogpath, logpath, n);
        strncat(archlogpath, ptr_path, strlen(ptr_path) - 4);
        strcat(archlogpath, mod_date_str);
      }
      else  
      {
        archlogpath = (char*) calloc (strlen(logpath) + strlen(mod_date_str) + 1, sizeof(char));
        strncpy(archlogpath, logpath, strlen(logpath) - 4);
        strcat(archlogpath, mod_date_str);
      }
      strcat(archlogpath,".log");
      rename(logpath, archlogpath);

      free(archlogpath);
    }

    free(mod_date_str);
  }
//puts("void DebugLevelLog::ArchiveLogFile(const char * logpath) --- end");
 // CreateHeader();

}

//========================================================================//
void DebugLevelLog::PrintTime(const DEBUG_LEVEL_ENUM debug_level)
{
//puts("void DebugLevelLog::PrintTime()");
  FILE * log;
  switch(debug_level)    {
    case LOGFILE : {
      log = OpenLog();
      if (log != NULL)        {
            char * str_time = (char *) calloc(30, sizeof(char));
	    time_t timv = time(0);
	    strcpy(str_time, ctime(&timv));
	    char * datestr = (char *) calloc(30, sizeof(char));
	    CreateDateString(str_time, datestr, DDAY | TTIME | MMONTH | YYEAR); // Формируем строку с датой для добавления в начала лога в файле
	    free(str_time);

	    char * str = (char*) calloc (200,sizeof(char));
	    strcpy(str, "\r\n");
	    strcat(str, datestr);
	    strcat(str, "\t");
	    fprintf(log, str);
	    free(datestr);
	    free(str);
	    fclose(log);  
      }   
      break;
    }      
    case STDOUT : {
        char * str_time = (char *) calloc(30, sizeof(char));
	time_t timv = time(0);
	strcpy(str_time, ctime(&timv));
	char * datestr = (char *) calloc(30, sizeof(char));
	CreateDateString(str_time, datestr, DDAY | TTIME | MMONTH | YYEAR); // Формируем строку с датой для добавления в начала лога в файле
	free(str_time);

	char * str = (char*) calloc (200,sizeof(char));
	strcpy(str, "\r\n");
	strcat(str, datestr);
	strcat(str, "\t");
	printf(str);
	free(datestr);
	free(str);
	break;	    
  }
 }
}

//========================================================================//

FILE* DebugLevelLog::OpenLog()
{

if (logpath == NULL) return NULL;
  FILE* log;
  log = fopen(logpath, "a");
  /*if (log == NULL)  {
    openlog(process_ident, LOG_CONS | LOG_PID | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_LOCAL1);
    char * sys_er_str = (char *) calloc(strlen(ERSTR) + strlen(logpath) + 1, sizeof(char));
    strcpy(sys_er_str,  ERSTR);
    strcat(sys_er_str, logpath);
    syslog (LOG_INFO, sys_er_str);
    free(sys_er_str);
    closelog();
  }  */
  return log;
}
//========================================================================//

