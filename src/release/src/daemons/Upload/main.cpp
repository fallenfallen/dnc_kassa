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

#include <stdio.h>
#include <malloc.h>
#include <signal.h>

#include "debug_level_log.h"
#include "load_file.h"
#include "upload_const.h"
#include "reader_config_load_unload.h"
#include "addon_func.h"
#include "const.h"
#include "const_conf.h"
#include "error_strings_log.h"
#include <qtextcodec.h>
#include "shtrih_load.h"
#include "dnc_load.h"

DebugLevelLog userlog("dancy_upload_daemon", "/tmp/dancy/upload_log/user_upload.log", true);
DebugLevelLog logfile("dancy_upload_daemon", "/tmp/dancy/upload_log/upload.log", true);
//  log - ошибки для пользователя выводятся в .log в папке где хранится файл флаг

QTextCodec * WinCodec = QTextCodec::codecForName("CP1251");
QTextCodec * SystemCodec = QTextCodec::codecForName(DBCODEC);

int protocol = 0;

const int replay_time = 2;
const char* daemon_exist  = "#";
const char* exist_request = "@";

#define USER_LOG_NAME "upload.log"

char * file_flag_path = NULL;
char * data_file_path = NULL;
//char * user_log_path = NULL; 

char* dir_path = NULL;

char log_str[2000];

bool stop_daemon = false;

bool log_flag_empty = false;
bool log_file_empty = false; 

void PrintTime();
void GetFilesPaths();
int TestPath(char* path);
int GetDirPath(char* file_path, char*& dir_path, char*& file_name);
int UploadDataFile();
//int CheckPaths();
int testLoad();
void SetUserLog();
void catch_sig(int sig_num);

/********************************************************************************************************/
// в качествве входящего параметра принимается полное имя - каталог + файл, пустая строка считается не правильным путем
// имя файла не может быть пустым
// 0 - файл существует и доступен
// 1 - существует каталог и доступен, файла нет = NULL !!!
// 2 - файл существует, но не доступен
// 3 - каталог существует, но не доступен? 
// -1 - неверный путь

int TestPath(char* path)
{
    if ( path == NULL ) return -1;
    if ( strlen(path) == 0 ) return -1;
    
    if ( access(path, 06) == 0 ) return 0;
    if ( access(path, 00) == 0 ) return 2;
    
    char * dir_path;
    
    char* ptr = strrchr(path,'/');
    if (ptr == NULL)    {
	// пустое имя каталога, // путь правильный
	dir_path = (char*) calloc( 2, sizeof(char) );
	strcpy(dir_path, "");
    }
    
    if (ptr != NULL) {  
	if ( strlen(ptr) < 2 ) return -1;		// имя файла пустое
	int n =  strlen(path) - strlen(ptr) + 1 ;
	dir_path = (char*) calloc(n+1, sizeof(char));
	strncpy( dir_path, path, n );	
    } 
    
    int ret = -1;
    
    if ( access(dir_path, 00) == 0 ) ret = 3;
    if ( access(dir_path, 06) == 0 ) ret = 1;
    
    free(dir_path);
    
    return ret;    
    
}

/********************************************************************************************************/
int GetDirPath(char* path, char* & dir_path, char* & file_name) 
{
    if ( path == NULL ) return -1;
    if ( strlen(path) == 0 ) return -1;
    
    char* ptr = strrchr(path,'/');
    if (ptr == NULL)    {
	// пустое имя каталога, // путь правильный
	file_name = (char*) calloc( strlen(path) + 2, sizeof(char));
	strcpy(file_name, path);
	dir_path = (char*) calloc( 2, sizeof(char) );
	strcpy(dir_path, "");
    }
    
    if (ptr != NULL) {  
	if ( strlen(ptr) < 2 ) return -1;		// имя файла пустое
	int n =  strlen(path) - strlen(ptr) + 1 ;
	dir_path = (char*) calloc(n+1, sizeof(char));
	strncpy( dir_path, path, n );	
	file_name = (char*) calloc( strlen(ptr) +1, sizeof(char) );
	strncpy(file_name, ptr + 1, strlen(ptr) - 1);
    } 
    
    if ( access(path, 06) == 0 ) return 0;
    if ( access(path, 00) == 0 ) return 2;
    
    if ( access(dir_path, 00) == 0 ) return 3;
    if ( access(dir_path, 06) == 0 ) return 1;
    
    return -1;   
}
/********************************************************************************************************/
void GetFilesPaths() {
    
    ReaderConf* rc = new ReaderConf();  
    
    char* tmp = (char*) calloc( rc->getLen("[UPLOAD_FLAG]") +1, sizeof(char) );
    rc->GetUploadFlagPath(tmp);
    
    //printf("file_flag_path = <%s> \n", file_flag_path);
    //printf("tmp = <%s>\n", tmp );
    
    if (  strcmp(tmp, file_flag_path) != 0  ) {
	
	if ( file_flag_path != NULL ) free(file_flag_path);
	
	file_flag_path = (char*) calloc(strlen(tmp)+1, sizeof(char));
	
	strcpy(file_flag_path, tmp);
	
	if ( TestPath(file_flag_path) < 0 ) {
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Для демона загрузки задан не верный путь к файлу флагу\r\n")) );
	    logfile.PrintString(log_type, ERROR, "the path upload flag uncorrect path = %s \n", file_flag_path);
	} else {
	    strcpy(log_str, "Путь к файлу флагу : " );
	    strcat(log_str, file_flag_path);
	    strcat(log_str, "\r\n");
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );	    
	    logfile.PrintString(log_type, INFO, "new path upload flag = %s \n", file_flag_path);		
	}
	    
	//printf("file flag path = %s \n", file_flag_path);
    } 
    
    free(tmp);    
    
    tmp = (char*) calloc( rc->getLen("[UPLOAD_FILE]") +1, sizeof(char) );
    rc->GetUploadDataPath(tmp);


    //printf("data_file_path = <%s> \n", data_file_path);
    //printf("tmp = <%s>\n", tmp );
    
    if (  strcmp(tmp, data_file_path) !=0 ) {    
	
	if ( data_file_path != NULL ) free(data_file_path);
	data_file_path = (char*) calloc(strlen(tmp)+1, sizeof(char));
	strcpy(data_file_path, tmp);
	
	if ( TestPath(data_file_path) < 0 ) {
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Для демона загрузки задан не верный путь к файлу с товарами\r\n")) );
	    logfile.PrintString(log_type, ERROR, "the path upload data file uncorrect path = %s \n", data_file_path);
	} else {
	    strcpy(log_str, "Путь к файлу загрузки" );
	    strcat(log_str, data_file_path);
	    strcat(log_str, "\r\n");
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );	  
	    //userlog.PrintString(user_log_type, INFO, "Путь у файлу загрузки = %s\r\n", data_file_path);
	    logfile.PrintString(log_type, INFO, "new path upload data = %s \n", data_file_path);
	    SetUserLog();
	}
	//printf("data_file_path = %s \n", data_file_path);    
    }
    
    free(tmp);    
    
    
    tmp = (char*) calloc( rc->getLen("[PROTOCOL]") + 1, sizeof(char) );
    rc->GetProtocol(tmp);
    
    protocol = atoi(tmp) ; 
    
    free(tmp);
    
    delete rc;
}	

/********************************************************************************************************/
void PrintTime()
{
  char * str = (char *) calloc(30, sizeof(char));
  time_t timv;
  timv = time(0);
  strcpy(str, ctime(&timv));
  free(str);
}
/********************************************************************************************************/
void catch_sig(int sig_num) {
     //signal(sig_num, catch_sigterm);
    switch (sig_num) {
	  case SIGSEGV :     
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon upload catch signal sigsegv. daemon stoped\n");		
	      break;
	  case SIGINT : 
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon upload catch signal sigint. daemon stoped\n");		
	      break;
	  case SIGTERM : 
	      userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Завершение работы\r\n")) );
	      logfile.PrintString(log_type, INFO, "daemon upload catch signal sigterm. daemon stoped\n");
	      break;
	  default: 
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon upload catch unknow signal. daemon stoped\n");
      }
     
     remove(ISLOAD_UPLOAD);
     if (file_flag_path!=NULL) free(file_flag_path);
     if (data_file_path!=NULL) free(data_file_path);
     
     kill(getpid()+1, SIGTERM);
     sigset(sig_num, SIG_DFL);    
     
     sigset(sig_num, SIG_DFL);
     kill(getpid(), sig_num);
}
/********************************************************************************************************/
int testLoad()
{
    //if (  access(ISLOAD_UPLOAD, 00) != 0 )  return -1;
    FILE * tmp = fopen(ISLOAD_UPLOAD, "w");
    if ( tmp == NULL ) return -1;
    fwrite(exist_request, sizeof(char), 1, tmp);
    fclose(tmp);
    sleep(replay_time);
    
    tmp = fopen(ISLOAD_UPLOAD, "r");
    char str[3];
    fgets(str,2, tmp);
    fclose(tmp);
    if ( strcmp(str, "#") == 0 )  return 1; else return 0;
}
/********************************************************************************************************/
void SetUserLog()
{
    //  вычисление пути для логов для юзера
    
    char * dir_path = NULL;
    char * file_name = NULL;
    
    int state = GetDirPath(data_file_path, dir_path, file_name);
        
    //if (dir_path != NULL) printf("dir_path = <%s>\n", dir_path);
    
    if ( state >= 0 ) {	
	char* user_log_path = (char*) calloc( strlen(dir_path) + strlen(USER_LOG_NAME) + 2, sizeof(char) );
	strcat(user_log_path, dir_path);
	strcat(user_log_path, USER_LOG_NAME);
	//userlog.SetLogPath(user_log_path);
	userlog.CreateLog(user_log_path, "dancy_upload_daemon \r\n", true);
	logfile.PrintString(log_type, INFO, "path of user log file = <%s> \n", user_log_path);
	free(user_log_path);
    }
    
    if ( dir_path != NULL) free(dir_path);
    if ( file_name != NULL )  free(file_name);
    
    
}

/********************************************************************************************************/
// -1 - не доступен файл загрузки
// 1
int UploadDataFile()  {
    int res = TestPath(data_file_path);
    if ( TestPath(data_file_path) != 0 ) {
	
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Невозможно открыть файл с товарами!\r\n")) );
	logfile.PrintString(log_type, ERROR, "the upload data file incorrect or absent or has status [access denied]. path  = %s \n", data_file_path);
	remove(file_flag_path);
	return -1;
    }
    
    if (protocol == 0) {
	
	LoadFile loaded_file(data_file_path);
	
	if  ( loaded_file.er_con != 0 ) {	
	    remove(file_flag_path);
	    return -1;    
	}
	
	loaded_file.Load();  // return 1 - файл уже был обработан  // -1 ошибка чтения файла
	remove(file_flag_path);
	
    } 
    if ( protocol == 1 ) {	
	//printf("start load for shtrih \n");
	ShtrihLoad shl;
	int state = shl.load(data_file_path);
	//printf("state = %d \n", state);
	remove(file_flag_path);	
	return state;
	
    }
    
    if ( protocol == 2 ) {
	printf("start load for dnc \n");
	DncLoad dnc_load;
	int state = dnc_load.load(data_file_path);
	//printf("state = %d \n", state);
	remove(file_flag_path);	
	return state;
    }
    
    return 0;
}
/********************************************************************************************************/
int main( int argc, char ** argv )
{
    puts("-------------------");
    puts("daemon start");
    puts("-------------------");
    
    file_flag_path = (char*) calloc(2, sizeof(char));
    data_file_path = (char*) calloc(2, sizeof(char)); 
    
   logfile.PrintString(log_type, INFO, "path of user log file = <%s> \n", userlog.GetLogPath());
    
    GetFilesPaths();
    
    int result_testLoad = testLoad();
    
    if ( result_testLoad == -1 ) {
	userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Отсутствует рабочий каталог. Запуск демона невозможен\r\n")) );
	logfile.PrintString(log_type, FATAL, "not found ISLOAD_UPLOAD dir path. daemon not run!!! path = %s \n ", ISLOAD_UPLOAD);
	exit(0);
    }
    
    if ( result_testLoad == 1 ) {
	userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Демон загрузки уже запущен, повторный запуск не требуется\r\n")) );
	logfile.PrintString(log_type, FATAL, "daemon exist load.\n ");
	exit(0);
    }   
   
    
    switch(fork())    {
      case (pid_t) -1 :
	  userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode( "Системная ошибка. Запуск демона не возможен!\r\n")) );
	  logfile.PrintString(log_type, FATAL, "fork 1 error..");	
	  break;
      case (pid_t) 0 :
	  switch ( fork() )  {
	    case (pid_t) -1 :  
		userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Запуск демона не возможен!\r\n")) );
		logfile.PrintString(log_type, FATAL, "fork 2 error..");
		break; 
	    case (pid_t) 0 :
		//puts("isload_upload process start");
		while( true ) {
		    FILE * tmp = fopen(ISLOAD_UPLOAD, "w");
		    if ( tmp == NULL ) { 
			userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Для демона загрузки отсутствует рабочий каталог. Запуск демона невозможен\r\n")) );
			logfile.PrintString(log_type, FATAL, "not found ISLOAD_UNLOAD path. path = %s \n ", ISLOAD_UPLOAD);
			exit(0);
		    }		    		    
		    fwrite(daemon_exist, sizeof(char), 1, tmp);
		    
		    fclose(tmp);   
		    chmod(ISLOAD_UPLOAD, 511);
		    sleep(replay_time);
		}
		break;
	    default :  
	                sigset(SIGTERM, catch_sig);
	                sigset(SIGSEGV, catch_sig);
		sigset(SIGINT, catch_sig);   
		
		setsid();	    
		chdir("/");
		chmod(ISLOAD_UPLOAD, 511); 
		
		//close(STDIN_FILENO);
		//close(STDOUT_FILENO);    
		//close(STDERR_FILENO);
//		puts("daemon process start");
		
		userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Демон загрузки запущен\r\n")) );
		while(true) {
		    GetFilesPaths();
		    if ( TestPath(file_flag_path) == 0 ) UploadDataFile();
		    sleep(5);
		}
		exit(0);  
	    }
	  break;  	
      default :
                if (file_flag_path!=NULL) free(file_flag_path);
                if (data_file_path!=NULL) free(data_file_path);
    }
    exit(0);
}
/********************************************************************************************************/

