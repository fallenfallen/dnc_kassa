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
#include "Writer.h"
#include "const_writer.h"
#include "addon_func.h"
#include "const_conf.h"
#include "const.h"


#include <sys/stat.h>

#define DEF_CHMOD S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

/*******************************************************************/
int Writer::FileUnloadSymbolLen() { return FILE_UNLOAD_SYMBOL_LEN; }
/*******************************************************************/
void Writer::FileUnloadSymbolAppend(char* str) {  strcpy(str, FILE_UNLOAD_SYMBOL_APPEND);  }
/*******************************************************************/
void Writer::FileUnloadSymbolRewrite(char* str) {  strcpy(str, FILE_UNLOAD_SYMBOL_REWRITE);  }
/*******************************************************************/
Writer::Writer() {   
    
}
/******************************************************************/
Writer::~Writer()  { 

}
/******************************************************************/
void Writer::get_temp_file_path(const char* base_path, char* temp_path)
{
   if (strlen(base_path) == 0)  return; 
   int n = strlen(base_path)-1;
   while ( n>=0 && base_path[n] != '/' ) {   n--;   }
   
   for (int i = 0; i <= n; i++) { temp_path[i] = base_path[i];   }
   strcat(temp_path, "tmp");
}
/******************************************************************/
int Writer::IncReportNumber(int n, char* unload_path, int protocol)
{     
   FILE * f_config;
   char* str = (char*) calloc(STR_LEN_MAX, sizeof(char));
   char* temp_path;
   int i;

   f_config = fopen(UNLOAD_CONF_PATH, "r");
   if ( f_config == NULL ) { 
       strcpy(log_str, "Не удалось прочитать конфигурационный файл - <" );
       strcat(log_str,  UNLOAD_CONF_PATH);
       strcat(log_str, ">\r\n");
       userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );
       logfile.PrintString(log_type, ERROR, "can't open config file path = <%s>\n", UNLOAD_CONF_PATH);
       return 1;  
   }
   
   // переписать файл конфиг	
   temp_path = (char*) calloc(strlen(UNLOAD_CONF_PATH)+4, sizeof(char)); 
   get_temp_file_path(UNLOAD_CONF_PATH, temp_path);
   
   FILE * temp = fopen(temp_path, "w+");
   
   i = 0;
   while ( !feof(f_config) ) {
       i++;
       fgets(str, STR_LEN_MAX, f_config);
       if (i == NUMBER_STR_REPORT_CONFIG) { itoa(n, str); strcat(str, "\n"); }
       fwrite( str, sizeof(char), strlen(str), temp );   
       strcpy(str, "");   }
   
   fclose(temp);
   fclose(f_config);
   
   remove(UNLOAD_CONF_PATH);
   rename(temp_path, UNLOAD_CONF_PATH);
   remove(temp_path);
   free(temp_path);
   
   switch (protocol) {
     case 2: {
        break;
     }
     default: {
	//изменить номер отчета в выгружаемом файле
	temp_path = (char*) calloc( strlen(unload_path)+4, sizeof(char));
	get_temp_file_path(unload_path, temp_path);
	FILE* f_unload = fopen(unload_path, "r");
	FILE* f_tmp = fopen(temp_path, "w+"); 
   
	i = 0;
	while ( !feof(f_unload) ) {
    	    fgets(str, STR_LEN_MAX, f_unload);
    	    if ( i == 2 ) { 
		itoa(n, str);
		strcat(str, "\r\n"); 
    	    } 
    	    fwrite(str, sizeof(char), strlen(str), f_tmp);
    	    strcpy(str, "");       i++;
	}
   
	fclose(f_tmp);
	fclose(f_unload);
   
	remove (unload_path);
	rename(temp_path, unload_path);
	remove(temp_path);
	free(temp_path);
	}
   }
   chmod(unload_path, DEF_CHMOD);
   free(str);
   return 0;
}
/******************************************************************************/
//-----(*)-----Zuskin-----19/05/2011-----
//int Writer::WriteHeader(FILE * f_unload, int report_number, int WS_number)
int Writer::WriteHeader(FILE * f_unload, int report_number, int WS_number, int protocol, char* comm_str, char* param_str, int count)
//---------------------------------------
{  
switch (protocol) {
    case 2: {  // new 'DNC' protocol
	char* str = (char*) calloc( 100, sizeof(char) );
	strcpy(str, "!!!DNCREPORT\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "#UNLOAD_STATE\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "not loaded\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "#PROGRAMM_VERSION\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "");
        //-----(*)-----Zuskin-----07/06/2011-----
        // чтобы не мучиться с кодировкой, нам нужны только цифры версии
        char* p = strchr(PROGRAMM_VERSION, ' ');
        if (p!= NULL) { 
    	    strcpy(str, p+1);
    	}
    	else {
    	    strcpy(str,PROGRAMM_VERSION);
    	}    
        //strcpy(str,PROGRAMM_VERSION);
        //---------------------------------------
        strcat(str, "\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "#REPORT_NUMBER\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "");
	itoa(report_number, str);
	strcat(str, "\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload );
        strcpy(str, "#REPORT_COMMAND\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        free(str);
        str = (char*) calloc (strlen(comm_str) + strlen(param_str) + 4, sizeof(char));
        strcpy(str, comm_str);
        strcat(str, " ");
        strcat(str, param_str);
        strcat(str, "\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        free(str);
        str = (char*) calloc( 100, sizeof(char) );
        strcpy(str, "#REPORT_DATE_TIME\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        char * tstr = str_current_dtime();
        strcpy(str, tstr);
        free(tstr);
        strcat(str, "\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "#RECORD_COUNT\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        strcpy(str, "");
	itoa(count, str);
	strcat(str, "\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload );

        free(str);
        break;
    }
    default: {    
	fwrite(FILE_UNLOAD_SYMBOL_APPEND, sizeof(char), strlen(FILE_UNLOAD_SYMBOL_APPEND), f_unload);
	char* str = (char*) calloc( 100, sizeof(char) );
	strcat(str, "\r\n");
	fwrite(str, sizeof(char), strlen(str), f_unload);
   
	strcpy(str, "");
	itoa(WS_number, str);
	strcat(str, "\r\n");
	fwrite( str, sizeof(char), strlen(str), f_unload );
   
	strcpy(str, "");
	itoa(report_number, str);
	strcat(str, "\r\n");
	fwrite( str, sizeof(char), strlen(str), f_unload );

	free(str);
    }
}   
   return 0;
}
/******************************************************************/
int Writer::SaveLastTrans(int num)
{
   char* str = (char*) calloc(STR_LEN_MAX, sizeof(char));
   
   FILE * f_config = fopen(UNLOAD_CONF_PATH, "rw+");
   if ( f_config == NULL ) {
       strcpy(log_str, "Не удалось прочитать конфигурационный файл - <" );
       strcat(log_str,  UNLOAD_CONF_PATH);
       strcat(log_str, ">\r\n");
       userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode(log_str)) );
       logfile.PrintString(log_type, ERROR, "can't open config file path = <%s>\n", UNLOAD_CONF_PATH);
       return -1;    
   }
   fgets(str, STR_LEN_MAX, f_config);
   
   itoa(num, str);
   
   fwrite( str, sizeof(char), strlen(str), f_config );
   free(str);
   fclose(f_config);
   
   return 0;
}
/******************************************************************/
int Writer::WriteConfig(int report_num, int num_last_trans)
{
    FILE * f_config = fopen(UNLOAD_CONF_PATH, "rw+");
    if ( f_config == NULL ) {
	strcpy(log_str, "Не удалось прочитать конфигурационный файл - <" );
	strcat(log_str,  UNLOAD_CONF_PATH);
	strcat(log_str, ">\r\n");
	logfile.PrintString(log_type, ERROR, "can't open config file path = <%s>\n", UNLOAD_CONF_PATH);
	return -1;    
    }
    
    char* str = (char*) calloc(STR_LEN_MAX, sizeof(char));
    itoa(report_num, str);
    fwrite( str, sizeof(char), strlen(str), f_config );
    
    itoa(num_last_trans, str);
    fwrite( str, sizeof(char), strlen(str), f_config );
    
    free(str);
    fclose(f_config);
    
    return 0;
}

/******************************************************************/
int Writer::WriteFooter(FILE * f_unload, int protocol) {
 
  switch (protocol) {
    case 2: {  // new 'DNC' protocol
	char* str = (char*) calloc( 100, sizeof(char) );
	strcpy(str, "!!!DNCREPORT_END\r\n");
        fwrite(str, sizeof(char), strlen(str), f_unload);
        free(str);
	break;
    }
    default: {
        break;
    }
  }     	
  return 0;
}