/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
/* MY MY MY MY */

#include <signal.h>
#include <qtextcodec.h>

#include "const_main.h"
#include "Command.h"
#include "Writer.h"
#include "reader_config_load_unload.h"
#include "const_conf.h"
#include "const.h"

int num_last_trans = 0;

DebugLevelLog userlog("dancy_unload_daemon", "/tmp/dancy/unload_log/user_unload.log", true);
DebugLevelLog logfile("dancy_unload_daemon", "/tmp/dancy/unload_log/unload.log", true);


QTextCodec * WinCodec = QTextCodec::codecForName("CP1251");
QTextCodec * SystemCodec = QTextCodec::codecForName(DBCODEC);

const int replay_time = 2;
const char* daemon_exist  = "#";
const char* exist_request = "@";

#define USER_LOG_NAME "unload.log"

char log_str[2000];

char * file_flag_path;
char * data_file_path;
int report_number = 0;
int WS_number = 0;
bool hu_mark_trans = false;
int n_protocol = 0;

FILE* f_flag;
FILE* f_unload;
char* temp;
int flag_str_count;

Command* cmd;

int TestPath(char* path);
int GetDirPath(char* path, char* & dir_path, char* & file_name) ;
void GetFilesPaths() ;
void SetUserLog();
int testLoad();


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
    
    //printf("rc->getLen([UNLOAD_FLAG]) = %d \n", rc->getLen("[UNLOAD_FLAG]"));

    char* tmp = (char*) calloc( rc->getLen("[UNLOAD_FLAG]") +1, sizeof(char) );
    rc->GetUnloadFlagPath(tmp);
    
    //printf("file_flag_path = <%s> \n", file_flag_path);
    //printf("tmp = <%s>\n", tmp );
    
    if (  strcmp(tmp, file_flag_path) != 0  ) {
	
	if ( file_flag_path != NULL ) free(file_flag_path);
	
	file_flag_path = (char*) calloc(strlen(tmp)+1, sizeof(char));
	
	strcpy(file_flag_path, tmp);
	
	if ( TestPath(file_flag_path) < 0 ) {
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Для демона выгрузки задан неверный путь к файлу-флагу\r\n")) );
	    logfile.PrintString(log_type, ERROR, "INCORRECT path to unload flag [%s]\n", file_flag_path);
	} else {
	    strcpy(log_str, "Путь к файлу флагу : " );
	    strcat(log_str, file_flag_path);
	    strcat(log_str, "\r\n");
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );	    
	    
	    //userlog.PrintString(user_log_type, INFO, "Путь к файлу флагу = %s\n", file_flag_path);
	    logfile.PrintString(log_type, INFO, "NEW path to unload flag = %s \n", file_flag_path);		
	}
	    
	//printf("file flag path = %s \n", file_flag_path);
    } 
    
    free(tmp);    

    tmp = (char*) calloc( rc->getLen("[UNLOAD_FILE]") +1, sizeof(char) );
    rc->GetUnloadDataPath(tmp);

    //printf("data_file_path = <%s> \n", data_file_path);
    //printf("tmp = <%s>\n", tmp );
    
    if (  strcmp(tmp, data_file_path) !=0 ) {    
	
	if ( data_file_path != NULL ) free(data_file_path);
	data_file_path = (char*) calloc(strlen(tmp)+1, sizeof(char));
	strcpy(data_file_path, tmp);
	
	if ( TestPath(data_file_path) < 0 ) {
	    userlog.PrintString(user_log_type, ERROR,WinCodec->fromUnicode(SystemCodec->toUnicode( "Для демона выгрузки задан неверный путь к файлу выгрузки\r\n")) );
	    logfile.PrintString(log_type, ERROR, "INCORRECT path to unload data file [%s] \n", data_file_path);
	} else {
	    strcpy(log_str, "Путь к файлу выгрузки : " );
	    strcat(log_str,  data_file_path);
	    strcat(log_str, "\r\n");
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );
	    //userlog.PrintString(user_log_type, INFO, "Путь у файлу выгрузки = %s\n", data_file_path);
	    logfile.PrintString(log_type, INFO, "NEW path unload data file = %s \n", data_file_path);
	    SetUserLog();
	}
	
	//printf("data_file_path = %s \n", data_file_path);    
    }

    free(tmp);   
    
    tmp = (char*) calloc( rc->getLen("[N_PC]") +1, sizeof(char) );
    rc->GetNPC(tmp);
    
    WS_number = atoi(tmp);
    
    free(tmp);
    
    tmp = (char*) calloc( rc->getLen("[HU_MARK_TRANS]") + 1, sizeof(char) );
    rc->GetHUParam(tmp);
    
    hu_mark_trans = ( strcmp( tmp, " t" ) == 0) || ( strcmp( tmp, "t" ) == 0 ) ; 
    
    free(tmp);
    
    tmp = (char*) calloc( rc->getLen("[PROTOCOL]") + 1, sizeof(char) );
    rc->GetProtocol(tmp);
    
    n_protocol = atoi(tmp);
    
    free(tmp);
    
    delete rc;
}
/******************************************************************************/
void catch_sig(int sig_num) {
    //userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Демон остановлен\r\n")) );
    switch (sig_num) {
	  case SIGSEGV :     
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка SIGSEGV. Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon unload caught SIGSEGV. daemon stopped");		
	      break;
	  case SIGINT : 
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка SIGINT. Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon unload caught SIGINT. daemon stopped");		
	      break;
	  case SIGTERM : 
	      userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Завершение работы\r\n")) );	      
	      logfile.PrintString(log_type, INFO, "daemon unload caught SIGTERM. daemon stopped");
	      break;
	  default: 
	      userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Неизвестная системная ошибка . Демон остановлен\r\n")) );
	      logfile.PrintString(log_type, FATAL, "daemon unload caught UNKNOWN signal. daemon stopped");
      }
     
     remove(ISLOAD_UPLOAD);
     if (file_flag_path!=NULL) free(file_flag_path);
     if (data_file_path!=NULL) free(data_file_path);
     
     kill(getpid()+1, SIGTERM);
     sigset(sig_num, SIG_DFL);    
     
     sigset(sig_num, SIG_DFL);
     kill(getpid(), sig_num);
     
}
/******************************************************************************/
int testLoad()
{
    //if (  access(ISLOAD_UPLOAD, 00) != 0 )  return -1;
    FILE * tmp = fopen(ISLOAD_UNLOAD, "w");
    if ( tmp == NULL ) return -1;
    fwrite(exist_request, sizeof(char), 1, tmp);
    fclose(tmp);
    sleep(replay_time);
    
    tmp = fopen(ISLOAD_UNLOAD, "r");
    char str[3];
    fgets(str,2, tmp);
    fclose(tmp);
    if ( strcmp(str, "#") == 0 )  return 1; else return 0;
}
/******************************************************************************/
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
	userlog.CreateLog(user_log_path, "dancy_unload_daemon", true);
	logfile.PrintString(log_type, INFO, "path to user log file = <%s> \n", user_log_path);
	free(user_log_path);
    }
    
    if ( dir_path != NULL) free(dir_path);
    if ( file_name != NULL )  free(file_name);
    
}
/******************************************************************************/
int ReadConfig()
{
    FILE * f_conf;
    
     f_conf = fopen( UNLOAD_CONF_PATH,"r" );
     
     if ( f_conf != NULL ) { 
	 char* str = (char*) calloc(CONFIG_FILE_STR_LEN_MAX, sizeof(char));
    
	 fgets(str, CONFIG_FILE_STR_LEN_MAX, f_conf);
	 report_number = atoi(str);
	 
	 fgets(str, CONFIG_FILE_STR_LEN_MAX, f_conf);
	 num_last_trans = atoi(str);
	 
	 free(str);
	 fclose(f_conf);
	// printf("report_number = %d \n", report_number);
	 //printf("num_last_trans = %d \n", num_last_trans);	 
	 return 0;
     }
     
     f_conf = fopen(UNLOAD_CONF_PATH,"a+");
     if ( f_conf != NULL )  {
	 fwrite("0\n", sizeof(char), 2, f_conf);
	 fwrite("0\n", sizeof(char), 1, f_conf);
	 num_last_trans = 0;
	 report_number = 0;
	 fclose(f_conf);
	 return 0;
     }
     
     userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Невозможно открыть конфигурационный файл для демона!!!\r\n")) );
     logfile.PrintString(log_type, ERROR, "Cannot open config file [%s] \n", UNLOAD_CONF_PATH);
     return -1; 
}
/******************************************************************************/
// 0 - хорошая строка 
// 1 - строка не подходит ни к параметрам ни к командам или пустая
int GetCommand(char* command_str, char* param_str)
{
    char* str = (char*) calloc(FLAG_FILE_STR_LEN_MAX, sizeof(char));
    int len;
    
    fgets(str, FLAG_FILE_STR_LEN_MAX, f_flag);

    len = strlen(str);
    
    if ( strcmp(str, "") == 0 )  {    free(str);    return 1;        }
    
    // если  строка состоит из одних пробелов это пустая и не правильная строка, вернуть 1 
    int i = 0;
    while ( i < len && *(str+i) == ' ' ) i++;
    if ( i == len ) return 1;
    
    flag_str_count++;
    
    if ( (*(str + len - 2) == CR) || (*(str + len - 2) == LF) ) *(str + len - 2) = '\0';
    if ( (*(str + len - 1) == CR) || (*(str + len - 1) == LF) )  *(str + len - 1) = '\0';
    
   switch (n_protocol) {
    case 2: { // new DNC protocol
	if ( strstr(str, DNC_SYMBOL) == str ) {
	    if (strcmp(command_str, "") != 0) {
		cmd->DoCommand(command_str, param_str, WS_number, f_unload);
	    }
	    strcpy(command_str, str + strlen(DNC_SYMBOL));
	    strcpy(param_str, "\0");
	} else {
	    if (strcmp(param_str, "") != 0) {
		cmd->DoCommand(command_str, param_str, WS_number, f_unload);
	    }
	    strcpy(param_str, str);
	}
        //printf("command = [%s], params = [%s]\n", command_str, param_str);
	break;
    }
    default: {
	if ( strstr(str, COMMAND_SYMBOL) == str ) {
	    if (strcmp(command_str, "") != 0) {
		cmd->DoCommand(command_str, param_str, WS_number, f_unload);
	    }
	    strcpy(command_str, str + strlen(COMMAND_SYMBOL));
	    strcpy(param_str, "\0");
	} else {
	    if (strcmp(param_str, "") != 0) {
		cmd->DoCommand(command_str, param_str, WS_number, f_unload);
	    }
	    strcpy(param_str, str);
	}
    }
   }    
        
    free(str);
    
    return 0;
}

/******************************************************************************/
int UnloadData()  {
    
    f_flag = fopen( file_flag_path, "r" );
    if ( f_flag == NULL ) { 
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Невозможно открыть для чтения файл-флаг!!!\r\n")) );
	logfile.PrintString(log_type, ERROR, "cannot open unload flag for reading [%s]\n", file_flag_path);
	remove(file_flag_path);
	return 1;
    }
    
    f_unload = fopen(data_file_path, "a+");
    if ( f_unload == NULL ) { 
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Невозможно открыть файл выгрузки для записи!!!\r\n")) );
	logfile.PrintString(log_type, ERROR, "cannot open unload data file for writing [%s] \n", data_file_path);
	fclose(f_flag); 
	remove(file_flag_path);
	return 1; 
    }
    
    cmd = new Command();
    //проверка на подключение к бД
    if ( cmd->er_con != 0 ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Отсутствует подключение к бд!!!\r\n")) );
	logfile.PrintString(log_type, ERROR, "cannot connect to database!!! \n");
	fclose(f_flag);
	fclose(f_unload);
	remove(file_flag_path);
	delete cmd;
	return 1;
    } 
       
    cmd->SetHUMarkTrans(hu_mark_trans);
    
    
    
    /******************************************************************************************/
    // если файл пустой сделать ему  //"$$$NEWTRANSACTIONS";
    bool rewrite = false;
    char* str = (char*) calloc( 21, sizeof(char) );
    switch (n_protocol) {
    
	case 2: { // new DNC protocol
    	    bool not_loaded = false;
    	    while (!feof(f_unload)) {
    		fgets(str, 20, f_unload);
    		if ( strstr(str, "#UNLOAD_STATE") != NULL ) {
    		    fgets(str, 20, f_unload);
    		    if ((strstr(str, "not loaded") != NULL ) || (strstr(str, "processing") != NULL ))  {
    		        not_loaded = true;
    			break;
    		    }
    		} 
    	    }
    	    if (!not_loaded) {
    		fclose(f_unload);
		f_unload = fopen(data_file_path, "w");
		rewrite = true;	   
	    }	
    	    break;    
    	}
    	default: {    
    	    
	    fgets(str, 20, f_unload);
    	    
	    if ( strcmp(str, "") == 0 ) rewrite = true;
    	    if (strchr(str, '@') != NULL) {    
		// @
		// файл был обработан, исходник можно перезаписать
		fclose(f_unload);
		f_unload = fopen(data_file_path, "w");
		rewrite = true;	
	    } 
	}
    }	
    free(str);    
    /*************************************************************************************/
        
    char* command_str = (char*) calloc(100, sizeof(char));
    char* param_str = (char*) calloc(100, sizeof(char));
    
    strcpy(command_str, "");    
    strcpy(param_str, "");
    
    flag_str_count = 0;
    
    // старт выгрузки
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Выгрузка продаж успешно стартовала \r\n")) );
    logfile.PrintString(log_type, INFO, "unload process started \n");
    
    Transaction::init_type_trans(n_protocol);
    
    while ( !feof(f_flag) ) {    
	int result = GetCommand(command_str, param_str);   
    }    
    if ( strcmp(command_str, "") == 0 ) {
        switch (n_protocol) {
    	    case 2: {
		strcpy(command_str,"NEWSALES");
		break;
	    }
	    default: {
	        strcpy(command_str,"NEWTRANSACTIONS");
	    }
	}    
	strcpy(param_str, "");
    }
    if ( (n_protocol == 2) || (rewrite) ) {
    //-----(*)-----Zuskin-----19/05/2011-----
    //if ( rewrite ) {
        //Writer::WriteHeader(f_unload, report_number, WS_number);   
        int cnt = cmd->GetCommandResultCount(command_str, param_str, WS_number);
        Writer::WriteHeader(f_unload, report_number, WS_number, n_protocol, command_str, param_str, cnt);
	//---------------------------------------
    }
    cmd->DoCommand(command_str, param_str, WS_number, f_unload);
/*    if ( strcmp(command_str, "") != 0 ) 
	cmd->DoCommand(command_str, param_str, WS_number, f_unload);
    if ( flag_str_count == 0 )
	cmd->DoCommand("NEWTRANSACTIONS", "", WS_number, f_unload); */
    
    free(command_str);
    free(param_str);
    Writer::WriteFooter(f_unload, n_protocol);
    fclose(f_flag);
    fclose(f_unload);
        
    /***************************************************************************/
    if ( cmd->command_count > 0) {
	report_number = report_number + cmd->command_count;
	Writer::IncReportNumber( report_number, data_file_path, n_protocol );
    }    
    /*****************************************************************************/
        
    remove(file_flag_path);
    
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Выгрузка продаж успешно завершена \r\n")) );
    logfile.PrintString(log_type, INFO, "unload process successfully complete \n");
    
    ///  выгрузка закончена
    
    delete cmd;
    
    return 0;
}
/******************************************************************************/
int main( int argc, char ** argv )
{
    
    puts("-------------------");
    puts("daemon unload start");
    puts("-------------------");
    
    file_flag_path = (char*) calloc(2, sizeof(char));
    data_file_path = (char*) calloc(2, sizeof(char)); 
    
    logfile.PrintString(log_type, INFO, "path to user log file = <%s> \n", userlog.GetLogPath());
    

    GetFilesPaths();
    ReadConfig();
    
    int result_testLoad = testLoad();    
    
    if ( result_testLoad == -1 ) {
	userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Отсутствует рабочий каталог. Запуск демона невозможен\r\n")) );
	logfile.PrintString(log_type, FATAL, "ISLOAD_UNLOAD path not found. daemon exits immediately!!! path = %s \n ", ISLOAD_UNLOAD);
	exit(0);
    }
    
    if ( result_testLoad == 1 ) {
	userlog.PrintString(user_log_type, INFO,WinCodec->fromUnicode(SystemCodec->toUnicode("Демон выгрузки уже запущен, повторный запуск не требуется\r\n")) );
	logfile.PrintString(log_type, FATAL, "unload daemon already exists\n ");
	exit(0);	
    }	  	 
     
    
    switch( fork() )       {
    case (pid_t) -1 :
	userlog.PrintString(user_log_type, FATAL,WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Запуск демона не возможен!\r\n")) );
	logfile.PrintString(log_type, FATAL, "fork 1 error..");	
	break;
    case (pid_t) 0:   
	switch ( fork() ) {
	case (pid_t) -1 :  
	    userlog.PrintString(user_log_type, FATAL,WinCodec->fromUnicode(SystemCodec->toUnicode("Системная ошибка. Запуск демона не возможен!\r\n")) );
	    logfile.PrintString(log_type, FATAL, "fork 2 error..");
	    break; 	      
	case (pid_t) 0 : 
	    while( true ) {
		FILE * tmp = fopen(ISLOAD_UNLOAD, "w");
		if ( tmp == NULL ) { 
		    userlog.PrintString(user_log_type, FATAL, WinCodec->fromUnicode(SystemCodec->toUnicode("Для демона выгрузки отсутствует рабочий каталог. Запуск демона невозможен\r\n")) );	
		    logfile.PrintString(log_type, FATAL, "ISLOAD_UNLOAD path not found. daemon exits immediately!!! path = %s \n ", ISLOAD_UPLOAD);
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
	     
	    //close(STDIN_FILENO);
	    //close(STDOUT_FILENO);    
	    //close(STDERR_FILENO);     
	    
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Демон выгрузки запущен\r\n")) );
	    while(true) {  
		GetFilesPaths();
		if ( TestPath(file_flag_path) == 0 ) UnloadData();
		sleep(5);
	    }
	    exit(0);  
	    
	    break;
	}			
    default :
                 if (file_flag_path!=NULL) free(file_flag_path);
                 if (data_file_path!=NULL) free(data_file_path);
    }
    exit(0);

}
/******************************************************************************/






