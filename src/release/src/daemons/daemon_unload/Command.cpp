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
#include "Command.h"
#include "const_command.h"
#include "Writer.h"
#include "addon_func.h"
#include "const_conf.h"
#include <qtextcodec.h>
#include <qstring.h>

extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;



/******************************************************************/
Command::Command()
{
   er_con = 0;
   //-----(*)-----Zuskin-----20/12/2011-----
   //db = new unload_pgsql(TRANSACTION_CONF_PATH);
   db = new unload_pgsql(TRANSACTION_CONF_PATH, false);
   //---------------------------------------
   er_con = db->er_con;

   command_count = 0;   
   
   hu_mark_last_trans = false;

}
/******************************************************************/
Command::~Command()
{
   delete db;
}
/******************************************************************/
int Command::ConvertParam(char* param, char** param_mas)
{
   char* str = (char*) calloc(PARAM_STR_LEN_MAX, sizeof(char));
   int i = 0;
   str = strtok(param, ";");
   while (str != NULL && i < COMMAND_PARAM_COUNT_MAX ) {
      if (strlen(str) <= PARAM_LEN_MAX) {  
         strcpy(param_mas[i], str); 
         if ( strlen(param_mas[i]) == 1 ) strcat(param_mas[i], " ");
         i++;   }
      
      str = strtok(NULL, ";");
   }
   free(str);
   return i;
}

/******************************************************************/
void Command::SetHUMarkTrans(bool param)
{
    hu_mark_last_trans = param;
}
//====================================================//
// 0 - все ок
// 2 - неправильная строка параметров
// 1 - команда отсутствует 
int Command::ConvertCommand(char* command, char* param, char* result)
{
   int ret = 0;
   char** param_mas = NULL;
   if (strlen(param) > 1) {
      param_mas = (char**) calloc(COMMAND_PARAM_COUNT_MAX, sizeof(char*)); 
      for (int i=0; i < COMMAND_PARAM_COUNT_MAX; i++) {     param_mas[i] = (char*) calloc(PARAM_LEN_MAX, sizeof(char));   }
      ConvertParam(param, param_mas);   }
   int num_command = 0;
   while (num_command < COMMAND_COUNT+1 && strcmp(command, COMMANDS[num_command]) !=0 ) {
	num_command++; 
	
   }
   
   if (num_command == COMMAND_COUNT+1) {
      if (strlen(param) > 1) {
         int i = COMMAND_PARAM_COUNT_MAX;
         while (--i >=0)    { free(param_mas[i]); }
         free(param_mas);   }
      return 1;      }
    
   if (n_protocol == 2) { // DNC protocol
	switch(num_command) {
	    case 9: {
		num_command = 3;
		break;
	    }
	    case 10: {
	        num_command = 6;
		break;
	    }	
	    case 11: {
		num_command = 0;
		break;
	    }
	    case 12: {
		num_command = 8;
		break;
	    }
	    case 13: {
		num_command = 1;
		break;
	    }
	    case 14: {
		num_command = 5;
		break;
	    }
	}
   }      
   int len = strlen(COMMANDS_SQLTEXT[num_command]);
   strcpy(result, SQLTEXT);
   //strcat(result, SQLTEXT);
   int i = 0;
   int num_param = 0;
   while ( i<len ) {
       if (*(COMMANDS_SQLTEXT[num_command] + i) == SYMBOL_BEFORE_PARAM) {
	   num_param = *(COMMANDS_SQLTEXT[num_command] + i + 1) - 49;
	   if (strlen(param)<1 || num_param >= COMMAND_PARAM_COUNT_MAX || strlen(param_mas[num_param]) < 1) { 	       
	       
	       userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Заданы неверные параметры для команды :%s \r\n")), command);
	       logfile.PrintString(log_type, ERROR, "Incorect param of %s command\n", command);
	       
	       ret = 2; 
	       break; 
	   }
	   uint k =0;
	   while ( k < strlen(param_mas[num_param]) ) { *(result + strlen(SQLTEXT) + i) = *(param_mas[num_param] + k) ;   i++; k++;  }
	   }
       *(result + strlen(SQLTEXT) + i) = *(COMMANDS_SQLTEXT[num_command] + i);
       i++;      
   }

   if (strlen(param) > 1) {
      int i = COMMAND_PARAM_COUNT_MAX;
      while (--i >=0)    { free(param_mas[i]); }
      free(param_mas);   }

   return ret;
}

/******************************************************************************/
// 0 - все ок
// 2 - неправильная строка параметров
// 1 - команда отсутствует 
// 3 - данные отсутствуют
// -1 - -3 - error DB
int Command::DoCommand(char* command_str, char* param_str, int num_WS, FILE* f_unload)
{      
    int com = 0;
    
    //-----DNC protocol-----
    if ( strcmp(command_str, "NEWSALES") == 0 ) com = 1;
    if ( strcmp(command_str, "SALESOFTIMERANGE") == 0 ) com = 2;
    //-----ATOL/Frontol-----
    if ( strcmp(command_str, "NEWTRANSACTIONS") == 0 ) com = 1;
    if ( strcmp(command_str, "TRANSACTIONSBYDATETIMERANGE") == 0 ) com = 2;
    //----------------------
    char* strSQL = (char*) calloc(2000, sizeof(char));
    int state = ConvertCommand(command_str, param_str, strSQL);
    if ( state != 0) { 
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка в строке. <%s>: <%s> Команда отсутствует или параметры заданы не верно\r\n")), command_str, param_str);
	logfile.PrintString(log_type, ERROR, "Incorect param or command apsent. command = <%s> param = %s\n", command_str, param_str);
	free(strSQL); 
	return state; 
    }
    
    itoa( num_WS, (strSQL + strlen(strSQL)-1) );
    
    if (com == 1) { 
	strcat(strSQL, " AND n_transaction > ");  
	itoa(num_last_trans, (strSQL + strlen(strSQL)-1));       
    }
    
    //if ( strcmp(command_str, "TRANSACTIONSBYREPORTRANGE") == 0 ) strcat(strSQL, " AND tr_type = 63");
    strcat(strSQL, " ORDER BY n_transaction");
    
    int count_trans = 0;
    
    state = db->DoCommand_GetCount(strSQL, &count_trans);
    
    free(strSQL);
    
    if ( state != 2 ) { 
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка выполнения команды = <%s> \r\n")), command_str);	
	logfile.PrintString(log_type, ERROR, "Data base error in command : <%s>\n", command_str);
	//free(strSQL);  
	return state;  
    }
    
    command_count++;
   
    if (count_trans == 0 ) { 
	userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Для данной команды <%s> отсутствуют данные! \r\n")), command_str);
	logfile.PrintString(log_type, INFO, "Data of %s command apsent\n", command_str);
	return 3;  
    }
    
    Transaction* tr_mas = (Transaction*) calloc(count_trans, sizeof(Transaction));
    for (int i = 0; i < count_trans; i++) {  
	tr_mas[i].data = (char**) calloc(COUNT_ATTRIBUTE, sizeof(char*));
	for (int j = 0; j < COUNT_ATTRIBUTE; j++) {         tr_mas[i].data[j] = (char*) calloc(ATTRIBUTE_LEN_MAX, sizeof(char));      }
    }
    
    db->GetResult(tr_mas, count_trans);
    int count_att_in_format = COUNT_ATTRIBUTE_IN_FORMAT;
    if ( n_protocol == 1 ) count_att_in_format = COUNT_ATTRIBUTE_SHTRIH;
    if ( n_protocol == 2 ) count_att_in_format = COUNT_ATTRIBUTE_DNC;
    
    char* str = (char*) calloc( 2*(COUNT_ATTRIBUTE * ATTRIBUTE_LEN_MAX + count_att_in_format) + 5, sizeof(char) ); 
    for (int i = 0; i < count_trans; i++) {  
	if (n_protocol == 1) tr_mas[i].CreateShtrihRecord(str);
	if (n_protocol == 2) tr_mas[i].CreateRecordDnc(str);
	if (n_protocol == 0) tr_mas[i].CreateRecord(str);
	strcat(str, "\r\n");
	QString wincodec_str = WinCodec->fromUnicode(SystemCodec->toUnicode(str));
	strcpy(str, wincodec_str);
	fwrite( str, sizeof(char), strlen(str), f_unload );
	strcpy(str, "");
    }
    free(str);
    
    if ( com == 2 && hu_mark_last_trans ) {
	if ( count_trans > 0 ) {
	    /*printf("first of unload = %d \n", atoi(tr_mas[0].data[0]));
	    printf("last of unload = %d \n", atoi(tr_mas[count_trans-1].data[0]));
	    printf("num_last_trans = %d \n", num_last_trans);*/
	    if ( (atoi(tr_mas[0].data[0]) - num_last_trans) > 2 ) {
		userlog.PrintString(user_log_type, WARN, WinCodec->fromUnicode(SystemCodec->toUnicode("Некоторые транзакции могут остаться не выгруженными!! - Номера транзакций с %d по %d \r\n")), num_last_trans, atoi(tr_mas[0].data[0]) );
		logfile.PrintString(log_type, INFO, "Some transaction can be not unloaded!! num of transactions : %d - %d \n", num_last_trans, atoi(tr_mas[0].data[0]));
	    }
	    
	    if ( (atoi(tr_mas[count_trans -1].data[0]) - num_last_trans) > 1 ) {
		num_last_trans = atoi( tr_mas[count_trans -1].data[0] );
		Writer::SaveLastTrans(num_last_trans);   
	    }   
	}
    }
    
    
    if ( com == 1 ) {  
	num_last_trans = atoi( tr_mas[count_trans -1].data[0] ); 
	Writer::SaveLastTrans(num_last_trans);   
    }
    
    for (int i = count_trans-1; i >= 0; i--) {  
	for (int j = COUNT_ATTRIBUTE-1; j >= 0; j--) {   free(tr_mas[i].data[j]);      }
	free( tr_mas[i].data );
    }
    free( tr_mas );
        
    return 0;
}
/************************************************************************************/

int Command::GetCommandResultCount(char* command_str, char* param_str, int num_WS)
{      
    int com = 0;
    
    //-----DNC protocol-----
    if ( strcmp(command_str, "NEWSALES") == 0 ) com = 1;
    if ( strcmp(command_str, "SALESOFTIMERANGE") == 0 ) com = 2;
    //-----ATOL/Frontol-----
    if ( strcmp(command_str, "NEWTRANSACTIONS") == 0 ) com = 1;
    if ( strcmp(command_str, "TRANSACTIONSBYDATETIMERANGE") == 0 ) com = 2;
    //----------------------    
    char* strSQL = (char*) calloc(2000, sizeof(char));
    char * tmparam = (char*) calloc (strlen(param_str) + 1, sizeof(char));
    strcpy(tmparam, param_str);
    int state = ConvertCommand(command_str, param_str, strSQL);
    strcpy(param_str, tmparam);
    free(tmparam);
    if ( state != 0) { 
	free(strSQL); 
	return 0; 
    }
    itoa( num_WS, (strSQL + strlen(strSQL)-1) );
    if (com == 1) { 
	strcat(strSQL, " AND n_transaction > ");  
	itoa(num_last_trans, (strSQL + strlen(strSQL)-1));       
    }
    //if ( strcmp(command_str, "TRANSACTIONSBYREPORTRANGE") == 0 ) strcat(strSQL, " AND tr_type = 63");
    strcat(strSQL, " ORDER BY n_transaction");
    int count_trans = 0;
    
    state = db->DoCommand_GetCount(strSQL, &count_trans);

    free(strSQL);
    return count_trans;    
}    