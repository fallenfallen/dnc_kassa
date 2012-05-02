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

#include "error_strings_log.h"

//===================================================================================//
ErStrLog::ErStrLog()
{
  //er_log = NULL;   
  //er_log = new DebugLevelLog("download", "/tmp/upload_err_log", false);
  er_count = 0;
}

//===================================================================================//
ErStrLog::~ErStrLog()
{
    //er_log->PrintString(LOGFILE, INFO, "Всего ошибок : %d\n", er_count);
  
    
    //if (er_log != NULL) delete(er_log);
}
//===================================================================================//
int ErStrLog::GetErrCount(){  return er_count;   }
//===================================================================================//
void ErStrLog::AddError(int error_type, int command_type, int n_str, int n_param)
{
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "ErStrLog::AddError(int error_type, int command_type, int n_str, int n_param)\n");
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "AddError(%d, %d, %d, %d)\n", error_type, command_type, n_str, n_param);
    char * er_str;
    int er_type = 0;
    
    char * ostr = NULL;
    ostr = (char *) calloc(200, sizeof(char));
    if ( ostr  !=  NULL )    CommandTypeStr(command_type, ostr);
    char * tstr = NULL;
    tstr = (char *) calloc(200, sizeof(char));
    if ( tstr != NULL )	{
	//pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "***************************\n");
	ErrorTypeStr(error_type, tstr, &er_type);
	//pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "***************************\n");
    }
    int len = 0;
    char * strnum = NULL;
    
    if ( n_str != 0 )  {
	strnum = itoa(n_str);
	len += strlen(strnum);  
    }
    char * parnum = NULL;
    if ( n_param != 0 )  {
	parnum = itoa(n_param);
	len += strlen(parnum);
    }
    char * pstr = (char *) calloc(len + 50, sizeof(char));
//    char * pstr = (char *) calloc(len + 100, sizeof(char));
    
    if ( n_str != 0 )  {
	strcpy(pstr, "Строка ");
	strcat(pstr, strnum);
	free(strnum);
    }
    if( n_param != 0 )  {
	strcat(pstr, ", параметр ");
	strcat(pstr, parnum);
	free(parnum);
    }
//  er_str = (char *) calloc(strlen(tstr) + strlen(ostr) + strlen(pstr) + 2, sizeof(char));
  er_str = (char *) calloc(strlen(tstr) + strlen(ostr) + strlen(pstr) + 3, sizeof(char));
    strcpy(er_str, tstr);
    strcat(er_str, ostr);
    strcat(er_str, pstr);
    strcat(er_str, "\r\n");
    free(ostr);
    free(tstr);
    free(pstr);
    
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL,	INFO, "%s\n", er_str);
    //er_log->PrintString(LOGFILE, (MESSAGE_TYPES_ENUM) er_type, er_str);
    logfile.PrintString(log_type, (MESSAGE_TYPES_ENUM) er_type, er_str);	        
    userlog.PrintString(user_log_type, (MESSAGE_TYPES_ENUM) er_type, WinCodec->fromUnicode(SystemCodec->toUnicode(er_str)) );
    
    er_count ++;
}

//===================================================================================//
// Под строку должна быть выделенна память не менее 50 символов
// ВНИМАНИЕ!!! ПАМЯТЬ ПОД СТРОКУ ДОЛЖНА БЫТЬ ВЫДЕЛЕНА ДО ВЫЗОВА
void ErStrLog::CommandTypeStr(int command_type, char * str)
{
    if ( str ==NULL )	return;
    switch(command_type)    {
      case 0 : // Ошибка не связанная с конкретной командой.
        strcpy(str, " ");
        break;
      case 1 : // ADDWAREGROUP COMMAND 
        strcpy(str, " Добавление товара/группы. ");
        break;
      case 2 : // ADDUSER COMMAND 
        strcpy(str, "Добавление пользователя. ");
        break;
      case 3 : // ADDFIXRED COMMAND 
        strcpy(str, "Добавление фиксированной скидки. ");
        break;
      case 4 : // ADD AUTOSHCEMECOMMAND 
        strcpy(str, "Добавление схемы автоматических скидок. ");
        break;
      case 5 : // ADDAUTORED COMMAND 
        strcpy(str, "Добавление автоматической скидки. ");
        break;
      case 6 : // ADDTAXRATE COMMAND 
        strcpy(str, "Добавление налоговой ставки. ");
        break;
      case 7 : // ADDTAXGROUP COMMAND 
        strcpy(str, "Добавление налоговой группы. ");
        break;
      case 8 : // ADDTAXRATEGROUP COMMAND 
        strcpy(str, "Добавление налоговой ставки/группы. ");    //?????
        break;
      case 9 : // DELWAREGROUP COMMAND 
        strcpy(str, " Удаление товара/группы. ");
        break;
      case 10 : // DELUSER COMMAND 
        strcpy(str, "Удаление пользователя. ");
        break;
      case 11 : // DELFIXRED COMMAND 
        strcpy(str, "Удаление фиксированной скидки. ");
        break;
      case 12 : // DEL AUTOSHCEME COMMAND 
        strcpy(str, "Удаление схемы автоматических скидок. ");
        break;
      case 13 : // DELAUTORED COMMAND 
        strcpy(str, "Удаление автоматической скидки. ");
        break;
      case 14 : // DELTAXRATE COMMAND 
        strcpy(str, "Удаление налоговой ставки. ");
        break;
      case 15 : // DELTAXGROUP COMMAND 
        strcpy(str, "Удаление налоговой группы. ");
        break;
      case 16 : // DELTAXRATEGROUP COMMAND 
        strcpy(str, "Удаление налоговой ставоки/группы. ");
        break;
      case 17 : 
        strcpy(str, "Добавление штрих кода. ");
        break;	
      case 18 : 
        strcpy(str, "Добавление товара. Неверный формат строки");
        break;	
    }
}

//===================================================================================//
// Под строку должна быть выделенна память не менее 200 символов
// ВНИМАНИЕ!!! ПАМЯТЬ ПОД СТРОКУ ДОЛЖНА БЫТЬ ВЫДЕЛЕНА ДО ВЫЗОВА
void ErStrLog::ErrorTypeStr(int error_type, char * str, int * ert)
{
  if ( str == NULL ) return;
  switch(error_type)    {
      case 1 : // ERROR FROM ANALIZ
        *ert = ERROR;
        strcpy(str, "Неправильный идентификатор. ");
        break;
      case 2 : // ERROR FROM ANALIZ
        *ert = ERROR;
        strcpy(str, "Нет поддержки суммовых налогов. ");
        break;
      case 3 : // WARNING FROM ANALIZ
        *ert = WARN;
        strcpy(str, "Ошибка данных. Полученная строка файла загрузки не соответствуют протоколу. Строка пропущена. ");
        break;
      case 4 : // ERROR FROM SQL
        *ert = ERROR;
        strcpy(str, "Ошибка при добавлении в БД. ");
        break;
      case 5 : // ERROR FORM READ STRING
        *ert = ERROR;
        strcpy(str, "Неправильный формат строки. ");
        break;
      case 6 : // ERROR FROM READ FILE FATAL!!!
        *ert = ERROR;
        strcpy(str, "Ошибка чтения файла загрузки. ");
        break;
      case 7 : //ERROR DATA BASE CONNECTION
        *ert = ERROR;
        strcpy(str, "Не удалось установить соединение с базой данных. ");
        break;
    }
}
