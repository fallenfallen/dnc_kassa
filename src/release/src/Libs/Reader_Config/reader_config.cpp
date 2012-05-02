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

#include "reader_config.h"

const char* PARAM_MAS[COUNT_PARAM] = {"[DB]",
                                      "[USER]",
                                      "[PASS]",
                                      "[HOST]",
                                      "[HOSTADDR]",
                                      "[PORT]",
                                      "[LOG]"}; 

/*************************************************************************/
ReaderConfig::ReaderConfig(const char* aconfig_path, bool try_home) {
//-----(*)-----Zuskin-----17/05/2011------
   nameDB = NULL;
   user = NULL;
   pass = NULL;
   host = NULL;
   hostaddr = NULL;
   port = NULL;
   log_path = NULL;
   
   //-----(*)-----Zuskin-----20/12/2011-----
   if (try_home) {
   //---------------------------------------
    char * pch = NULL;
    char * fnm = NULL; 
    char * tstr = (char*) calloc ( strlen(aconfig_path) +1, sizeof(char));
    strcpy(tstr, aconfig_path);
   
    pch = strtok(tstr, "/");
    if (pch != NULL) {
	    fnm = pch;
	}
    while (pch != NULL) {
	pch = strtok( NULL, "/");
	if (pch != NULL) {
	    fnm = pch;
	}
    }
   
    if (fnm != NULL) {
	config_path = (char*) calloc ( strlen(getenv("HOME")) + strlen("/.dnc/") + strlen(fnm) + 1, sizeof(char));
	strcpy(config_path, getenv("HOME"));
	strcat(config_path, "/.dnc/");
	strcat(config_path, fnm);
	printf("trying config file: %s\n", config_path);
    
	if ( FillParam() == -1 ) {
	    //free(config_path);
	    config_path = (char*) calloc( strlen(aconfig_path) + 1, sizeof(char) );
	    strcpy(config_path, aconfig_path);
	    printf("file not found. trying config file: %s\n", config_path);
	    FillParam();
	}
    }
    /*if (pch != NULL) free(pch);
    if (fnm != NULL) free(fnm);
    if (tstr != NULL) free(tstr);*/
        
    return;
   }
       
  //-----(*)-----Zuskin-----20/12/2011----- 
  config_path = (char*) calloc( strlen(aconfig_path) + 1, sizeof(char) );
  strcpy(config_path, aconfig_path);
  FillParam();
  //---------------------------------------          
/*
//   printf("create reader config \n");

   config_path = (char*) calloc( strlen(aconfig_path) + 1, sizeof(char) );
   strcpy(config_path, aconfig_path);
   
   nameDB = NULL;
   user = NULL;
   pass = NULL;
   host = NULL;
   hostaddr = NULL;
   port = NULL;
   log_path = NULL;
   //puts("fill param");
   FillParam();
//   puts("reader config init ok");
*/
//------------------------------------------
}
/*************************************************************************/
ReaderConfig::~ReaderConfig() {
   if (nameDB != NULL) free(nameDB);
   if (user != NULL) free(user);
   if (pass != NULL) free(pass);
   if (host != NULL)  free(host);
   if (hostaddr != NULL) free(hostaddr);
   if (port != NULL) free(port);
   if (log_path != NULL) free(log_path);
}
/*************************************************************************/
void ReaderConfig::GetDB(char* a_nameDB) {
   if (nameDB == NULL || a_nameDB == NULL) { return; }
   strcpy(a_nameDB, nameDB);   }
/*************************************************************************/
void ReaderConfig::GetUser(char* a_user) {
   if (user == NULL || a_user == NULL) { return; }
   strcpy(a_user, user);   }
/*************************************************************************/
void ReaderConfig::GetPass(char* a_pass) {   
   if (pass == NULL || a_pass == NULL) { return; }
   strcpy(a_pass, pass);   }
/*************************************************************************/
void ReaderConfig::GetHost(char* a_host) {   
   if (host == NULL || a_host == NULL) { return; }
   strcpy(a_host, host);   }
/*************************************************************************/
void ReaderConfig::GetHostAddr(char* a_hostaddr) {
   if (hostaddr == NULL || a_hostaddr == NULL) { return; }
   strcpy(a_hostaddr, hostaddr);   }
/*************************************************************************/
void ReaderConfig::GetPort(char* a_port)  { 
   if (port == NULL || a_port == NULL) { return; }
   strcpy(a_port, port);   }
/*************************************************************************/
void ReaderConfig::GetLog(char* a_log_path) {  
   if (log_path == NULL || a_log_path == NULL) { return; }
   strcpy(a_log_path, log_path);   }
/*************************************************************************/
int ReaderConfig::GetDBLength() {  if (nameDB != NULL) return strlen(nameDB);  return -1;  }
/*************************************************************************/
int ReaderConfig::GetUserLength() {  if (user != NULL) return  strlen(user);  return -1;  }
/*************************************************************************/
int ReaderConfig::GetPassLength() {  if (pass != NULL) return  strlen(pass);  return -1;  }
/*************************************************************************/
int ReaderConfig::GetHostLength() {  if (host != NULL) return strlen(host);  return -1;  }
/*************************************************************************/
int ReaderConfig::GetHostAddrLength() {  if (hostaddr != NULL) { return strlen(hostaddr); } return -1;  }
/*************************************************************************/
int ReaderConfig::GetPortLength() {  if (port != NULL) return strlen(port);  return -1;  }
/*************************************************************************/
int ReaderConfig::GetLogLength() {  if (log_path != NULL) return strlen(log_path);  return -1;  }
/*************************************************************************/
void ReaderConfig::TakeParam(int n, char* str) {
   //printf("take param %d from %s \n", n, str);
   int len_str = strlen(str);
   //printf("len of str = %d \n", len_str);
   int len_text = strlen(PARAM_MAS[n]);
   //printf("PARAM_MAS[n] = %s len of PARAM_MAS[n] = %d \n", PARAM_MAS[n], len_text);
   switch (n)  { 
      case 0 : 
         nameDB = (char*) calloc(len_str - len_text + 1, sizeof(char));  
         for (int i = 0; i < len_str - len_text; i++) *(nameDB + i) = *(str + len_text + i);  
         break;
      case 1 : 
         user = (char*) calloc(len_str - len_text + 1, sizeof(char));  
         for (int i = 0; i < len_str - len_text; i++) *(user + i) = *(str + len_text + i); 
         break; 
      case 2 :
         pass = (char*) calloc(len_str - len_text + 1, sizeof(char));   
         for (int i = 0; i < len_str - len_text; i++) *(pass + i) = *(str + len_text + i); 
         break; 
      case 3 :
         host = (char*) calloc(len_str - len_text + 1, sizeof(char));   
         for (int i = 0; i < len_str - len_text; i++) *(host + i) = *(str + len_text + i); 
         break;  
      case 4 :
         hostaddr = (char*) calloc(len_str - len_text + 1, sizeof(char));   
         for (int i = 0; i < len_str - len_text; i++) *(hostaddr + i) = *(str + len_text + i); 
         break;  
      case 5 : 
         port = (char*) calloc(len_str - len_text + 1, sizeof(char));  
         for (int i = 0; i < len_str - len_text; i++) *(port + i) = *(str + len_text + i); 
         break; 
      case 6 :
         log_path = (char*) calloc(len_str - len_text + 1, sizeof(char));   
         for (int i = 0; i < len_str - len_text; i++) *(log_path + i) = *(str + len_text + i); 
         break; 
   }
   //printf("nameDB = %s \n", nameDB);
}
/*************************************************************************/
void ReaderConfig::rstr(char* str) {
   //printf("read str %s \n", str);
   fgets(str, LEN_BUF, f_config);
   int i = 1;

   while ( *(str + strlen(str) -1) != '\n' &&  !feof(f_config) ) {
      //printf("in while \n");
      i++;
      str = (char*) realloc(str, sizeof(char)*(i*LEN_BUF+1));
      fgets( str + strlen(str), LEN_BUF, f_config );
   }
   //printf("str from file  end of read = %s \n",str);
}
/*************************************************************************/
int ReaderConfig::FillParam() {  

//puts("int ReaderConfig::FillParam()");

   f_config = fopen(config_path, "r");
   //-----(+)-----Zuskin-----22/12/2011-----
   free(config_path);
   //---------------------------------------
   if (f_config == NULL) { return -1; }
   
   char* tmp = NULL;
   int i = 0;
//   puts("read file ");
   while ( !feof(f_config) ) {
      tmp = (char*) calloc( LEN_BUF + 1, sizeof(char) );
      rstr(tmp);      
      strrs(tmp);         
      i = 0;
//      printf("tmp = %s \n", tmp);
      while ( ( strstr(tmp, PARAM_MAS[i]) != tmp ) &&  ( i < COUNT_PARAM ) ) i++;
      if ( i < COUNT_PARAM ) TakeParam(i, tmp);
      free(tmp);
   }
   fclose(f_config);

//puts("end of fill param");
   
   return 0;
}
/*************************************************************************/












