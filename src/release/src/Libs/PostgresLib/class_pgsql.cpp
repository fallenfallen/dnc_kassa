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
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "class_pgsql.h"
#include "reader_config.h"

using namespace std;

//==================================================================================//
PGSQL::PGSQL()
{
 
  er_con = 1;
  
  db = NULL;
  us = NULL;
  passwd = NULL;
  hostname = NULL;
  hostaddrname = NULL;
  portnum = NULL;

  // лог по умолчанию
  strcpy(logpath, DEFAULT_LOG_PATH);
  log = new DebugLevelLog("dancy_pgsqllib", logpath, true);
   
}

//==================================================================================//
//-----(*)-----Zuskin-----20/12/2011-----
//PGSQL::PGSQL(const char * conf_path)
PGSQL::PGSQL(const char * conf_path, bool try_home)
//---------------------------------------
{
    
  er_con = 0;

  db = NULL;
  us = NULL;
  passwd = NULL;
  hostname = NULL;
  hostaddrname = NULL;
  portnum = NULL;
  logpath = NULL;
  
  confpath = (char*) calloc( strlen(conf_path) + 1, sizeof(char));
  
  strcpy(confpath, conf_path);
  
  //log = NULL;
  
  //-----(*)-----Zuskin-----20/12/2011-----
  //ReaderConfig * rc = new ReaderConfig(conf_path);
  ReaderConfig * rc = new ReaderConfig(conf_path, try_home);
  //---------------------------------------

  int len = 0;

  len = rc->GetDBLength();
  if ( len > 0 ) {	
  	db = (char *) calloc(len + 1, sizeof(char));	
  	rc->GetDB(db);
  }

  len = rc->GetUserLength();
  if ( len >0 ) {
  	us = (char *) calloc(len + 1, sizeof(char));
	rc->GetUser(us);
  }

  len = rc->GetPassLength();
  if (len > 0 ) {
  	passwd = (char *) calloc(len + 1, sizeof(char));
	rc->GetPass(passwd);
  }

  len = rc->GetHostLength();
  if ( len > 0 ) {
  	hostname = (char *) calloc(len + 1, sizeof(char));
	rc->GetHost(hostname);
  }

  len = rc->GetHostAddrLength();
  if ( len > 0 ) {
	hostaddrname = (char*) calloc(len + 1, sizeof(char));
	rc->GetHostAddr(hostaddrname);
  }
  
  len = rc->GetPortLength();
  if ( len > 0 ) {
	portnum = (char *) calloc(len + 1, sizeof(char));
	rc->GetPort(portnum);
  }
  
  len = rc->GetLogLength();
  if ( len > 0 ) {
  	logpath = (char *) calloc(len + 1, sizeof(char));
	rc->GetLog(logpath);
	log = new DebugLevelLog("dancy_pgsqllib", logpath, true);
  } else {
	log = new DebugLevelLog("dancy_pgsqllib", DEFAULT_LOG_PATH, true);
  }
  
  delete rc;

  if ( Init_Connection() != 0 )    er_con = 1;
  else    Set_Datestyle();

}

//==================================================================================//
PGSQL::PGSQL(const char *dbname, const char *user, const char *password, const char *host, const char *hostaddr, const char *port, const char* log_path)
{
  //pgsql_onscreanlog.CreateLog("pgsqllib_onsrean", "dancy_pgsqllib", false);
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGSQL::PGSQL(const char *dbname, const char *user, const char *password, const char *host, const char *hostaddr, const char *port, const char* log_path)\n");
  er_con = 0;
 
  db = NULL;
  us = NULL;
  passwd = NULL;
  hostname = NULL;
  hostaddrname = NULL;
  portnum = NULL;
  logpath = NULL;

  log = NULL;
  
  Set_Log_Path(log_path);
  Set_Db(dbname);
  Set_User(user);
  Set_Password(password);
  Set_Host(host);
  Set_Hostaddr(hostaddr); 
  Set_Port(port);
  
  if ( logpath != NULL ) {
  	if ( strlen(logpath) > 0 )  log = new DebugLevelLog("dancy_pgsqllib", logpath, false);
  } else {
	log = new DebugLevelLog("dancy_pgsqllib", DEFAULT_LOG_PATH, true);
  }
  
  if ( Init_Connection() != 0 ) er_con = 1;
  else Set_Datestyle();
}

//==================================================================================//
PGSQL::~PGSQL()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGSQL::~PGSQL()\n");
  Close_Connection();

  if ( log != NULL ) delete(log);
  if (confpath != NULL) free(confpath);

}

//==================================================================================//
int PGSQL::Init_Connection()
{

  if ( db == NULL ) {
      log->PrintString(log_type, FATAL, "db string empty. config file = %s \n", confpath);
      return 1;
  }
  
  if ( strlen(db) == 0 ) {
      log->PrintString(log_type, FATAL, "db string empty. config file = %s \n", confpath);
      return 1;
  }

  int len = 0;
  
  len += strlen("dbname = ");
  len += strlen(db);
  
  if ( us != NULL ) { len += strlen(" user = "); len += strlen(us);  }
  if ( passwd != NULL ) {  len += strlen(" password = "); len += strlen(passwd);  }
  if ( hostname != NULL ) { len += strlen(" host = "); len += strlen(hostname);  }
  if ( hostaddrname != NULL ) {  len += strlen(" hostaddr = "); len += strlen(hostaddrname);   }
  if ( portnum != NULL )  { len += strlen(" port = ");  len += strlen(portnum);  }
  
  char* conninfo = (char *) calloc ( len + 1, sizeof(char));

  strcpy(conninfo, "dbname = ");
  strcat(conninfo, db);
  
  if ( us != NULL )  {   strcat(conninfo, " user = ");    strcat(conninfo,us); } 
  if ( passwd != NULL )  {    strcat(conninfo, " password = ");    strcat(conninfo,passwd);  } 
  if ( hostname != NULL )   {    strcat(conninfo, " host = ");    strcat(conninfo,hostname);  }
  if ( hostaddrname != NULL )   {    strcat(conninfo, " hostaddr = ");    strcat(conninfo,hostaddrname);  }
  if ( portnum != NULL )  {    strcat(conninfo, " port=");    strcat(conninfo,portnum);  }

  myconnection = PQconnectdb(conninfo);

  if(PQstatus(myconnection) == CONNECTION_OK)  {
    free(conninfo);
    return 0;
  }  else  {
    log->PrintString(log_type, FATAL, "connection failed,\n config file = %s \n db name = <%s>\n us name = <%s>\n host = <%s>\n hostaddr = <%s>\n port = <%s>\n", confpath,db, us, hostname, hostaddrname, portnum);
    
    //PQfinish(myconnection);
    free(conninfo);
    return 1;
  }
}

//==================================================================================//
void PGSQL::Close_Connection()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGSQL::Close_Connection()\n");

  if(logpath != NULL) free(logpath);

  if(db != NULL) free(db);

  if(us != NULL) free(us);

  if(passwd != NULL) free(passwd);

  if(hostname != NULL) free(hostname);

  if(hostaddrname != NULL) free(hostaddrname);

  if(portnum != NULL) free(portnum);

  if(er_con == 0) PQfinish(myconnection);

}

//==================================================================================//
void PGSQL::Set_Log_Path(const char* log_path)
{
  if ( log_path == NULL ) return;
  if ( strlen(log_path) == 0 ) return;

  logpath = (char*) calloc(strlen(log_path) + 1, sizeof(char));
  strcpy(logpath, log_path);
  
  log = new DebugLevelLog("dancy_pgsqllib", logpath, false);
}

//==================================================================================//
void PGSQL::Set_Db(const char * dbname)
{
  if ( dbname == NULL ) return;
  if ( strlen(dbname) == 0 ) return;
  if (db != NULL ) free(db);
  db = (char *) calloc(strlen(dbname) + 1, sizeof(char));
  strcpy(db, dbname);
}

//==================================================================================//
void PGSQL::Set_User(const char * user)
{
  if ( user == NULL ) return;
  if ( strlen(user) == 0 ) return;
  if (us != NULL) free(us);
  us = (char *) calloc(strlen(user) + 1, sizeof(char));
  strcpy(us, user); 
}

//==================================================================================//
void PGSQL::Set_Password(const char * password)
{
  if ( password == NULL ) return;
  if ( strlen(password) == 0 ) return;
  if ( passwd != NULL ) free(passwd);
  passwd = (char *) calloc(strlen(password), sizeof(char));
  strcpy(passwd, password);
}

//==================================================================================//
void PGSQL::Set_Host(const char * host)
{
  if ( host == NULL ) return;
  if ( strlen(host) == 0 ) return;
  if ( hostname != NULL ) free(hostname);
  hostname = (char *) calloc(strlen(host) + 1, sizeof(char));
  strcpy(hostname, host);
}

//==================================================================================//
void PGSQL::Set_Hostaddr(const char *hostaddr)
{
  if ( hostaddr == NULL ) return;
  if ( strlen(hostaddr) == 0 ) return;
  if ( hostaddrname != NULL ) free(hostaddrname);
  hostaddrname = (char *) calloc(strlen(hostaddr) + 1, sizeof(char));
  strcpy(hostaddrname, hostaddr);
}

//==================================================================================//
void PGSQL::Set_Port(const char *port)
{
  if ( port == NULL ) return;
  if ( strlen(port) == 0 ) return;
  if ( portnum != NULL ) free(portnum);
  portnum = (char *) calloc(strlen(port) + 1, sizeof(char));
  strcpy(portnum, port);
}

//==================================================================================//
char* PGSQL::Get_Db() {  return db;   }
//==================================================================================//
char* PGSQL::Get_User() {    return us;  }
//==================================================================================//
char* PGSQL::Get_Password()  {  return passwd;  }
//==================================================================================//
char* PGSQL::Get_Host()  {  return hostname;	}
//==================================================================================//
char* PGSQL::Get_Hostaddr()  {  return hostaddrname;  }
//==================================================================================//
PGconn* PGSQL::Get_Connection()  {  return myconnection; }
//==================================================================================//
void PGSQL::pause(int Nsec)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PGSQL::pause(int Nsec)\n");
  struct timespec R1, R2;
  if (Nsec < 1 || Nsec > 199)
    return;
  R1.tv_sec = 0;
  R1.tv_nsec = Nsec * 100000000;
  if (nanosleep(&R1,&R2)!=0)
    nanosleep(&R2,&R1);
  return;
}

//==================================================================================//
int PGSQL::ReConnect()
{  
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PGSQL::ReConnect()\n");
  int k = 0;
  while (er_con != 0)
  { 
    k++;
    pause(9);
    er_con = Init_Connection();
    if (k >= 10) break;
  /*  {

      pause(30);
      //er_con = Init_Connection();
      if (er_con == 0)
        break;
    }*/
  }
  //puts("END OF Reconnet");
  return k;
}

//==================================================================================//
int PGSQL::doSQL(PGconn * conn, const char * command)
{
  //printf("doSQL(%s) \n", command);
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGSQL::doSQL(PGconn * conn, const char * command)\n");
  int n = 0;
  int res = -3;
  while (1)  { 
    //printf("er_con = %d \n", er_con);
    if (er_con != 0) {
	int recon = ReConnect();
	//printf("Recon = %d \n", recon);  
      if(recon == 10) break;
    }
    
    //puts("AFTER Reconnect AND break");	

    result = PQexec(conn, command);

    switch (PQresultStatus(result))
    {
      case PGRES_EMPTY_QUERY :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGRES_EMPTY_QUERY - доступ к базе данных не требуется\n");
        res = 0;
        break;
      case PGRES_COMMAND_OK :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGRES_COMMAND_OK - Успешное завершение. Команда не возращает данные\n"); 
        res = 1;
        PQclear(result);
        break;
      case PGRES_TUPLES_OK :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "PGRES_TUPLES_OK - Успешное завершение. Запрос вернул ноль или больше строк\n"); 
	//printf("PQresultStatus(result) = PGRES_tuples_OK = %d \n", PQresultStatus(result));
        res = 2;
        break;
      case PGRES_BAD_RESPONSE :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, ERROR, "PGRES_BAD_RESPONSE - Ошибка. Ответ сервера непонятен\n");
        res = -1;
        break;
      case PGRES_NONFATAL_ERROR :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, ERROR, "PGRES_NONFATAL_ERROR - Нефатальная ошибка, ожно попробовать еще раз\n"); 
        n++;
        break;
      case PGRES_FATAL_ERROR :
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, FATAL, "PGRES_FATAL_ERROR - Фатальная ошибка, повторить попытку нельзя\n");
	printf("PQresultStatus(result) = PGRES_FATAL_ERROR = %d \n", PQresultStatus(result));
	printf("dbname = %s \n", db);
	printf("user name = %s \n", us);
	//printf("password user = %s \n", passwd);
	printf("command = %s \n", command);
	printf("Error : %s \n", PQresultErrorMessage(result));
		
 	log->PrintString(log_type, ERROR, "Error exec command = %s\n PQresultStatus = PGRES_FATAL_ERROR\n dbname = %s\n user = %s\n", command, db, us);

        res = -2;
        break;
      default : 
        //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, FATAL, "default - Непредвиденные проблемы с сервером\n");
	
	log->PrintString(log_type, ERROR, "Error exec command = %s\n PQresultStatus = UNKNOW ERROR\n dbname = %s\n user = %s\n", command, db, us);
	
        res = -3;
        break;
    }
    if (n > 5)
    {    
      res = -3;
      break;
    }
    if (n == 0)
      break;
  }
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "command = %s\n", command);
  //printf("AFTER WHILE. er_con = %d \n", er_con); 
  if(res <= 0)
  {
    //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "res = %d\n", res); 
    //log->PrintString(DEBUG_LEVEL, ERROR, "command : %s\n%s\n", command, PQresultErrorMessage(result));
    if(er_con == 0) PQclear(result);
  }
  //printf("end of doSQL \n");
  return res;
}

//==================================================================================//
//дата в формате ISO, DMY, то есть 01-12-2005 
#define SET_DATESTYLE "SET DATESTYLE TO 'European, ISO'"
int PGSQL::Set_Datestyle()
{
  int state;
  char *str;
  str = (char *) calloc(strlen(SET_DATESTYLE) + 1, sizeof(char));
  
  strcpy(str,SET_DATESTYLE);
  
  state = doSQL(myconnection, str);

  free(str);
  return state;  
}


//==================================================================================//
#define BEGIN_TR "BEGIN WORK"
#define ROLLBACK_TR "ROLLBACK WORK"
#define COMMIT_TR "COMMIT WORK"
int PGSQL::BeginTransaction()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PGSQL::BeginTransaction()\n");
    
    return doSQL(myconnection, "BEGIN WORK");
    
}
//==================================================================================//
int PGSQL::RollbackTransaction()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PGSQL::RollbackTransaction()\n");
    return doSQL(myconnection,"ROLLBACK WORK");
}
//==================================================================================//
int PGSQL::CommitTransaction()
{
    return doSQL(myconnection, "COMMIT WORK");
}
//==================================================================================//

