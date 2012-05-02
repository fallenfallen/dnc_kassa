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
//---(+)---Zuskin---28/02/2012---
using namespace std;
#include <string>
#include "stringconvert.h"
//-------------------------------

#include "get_param_pgsql.h"

//==================================================================================//
GetParam::GetParam()   {
    //puts("GetParam::GetParam()");
    Set_Db("LinCash");
}

//==================================================================================//
GetParam::GetParam(const char * conf_path) : PGSQL(conf_path)
{
    //printf("GetParam::GetParam(conf_path = %s)\n", conf_path);
}

//==================================================================================//
GetParam::GetParam(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)   {
    //puts("GetParam::GetParam(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path)");
}
//==================================================================================//
GetParam::~GetParam()   {
    
    //puts(" GetParam::~GetParam() ");
}
//==================================================================================//
#define TAKE_PARAM "SELECT pv.value FROM parameters p, parametersValues pv, activeScheme act WHERE pv.scheme_id = act.activeScheme_id AND pv.param_id = p.param_id AND p.param_name = '"
int GetParam::query_TakeParam(const char* param_name) {
   int state;
    char * tmp = (char *) calloc(strlen(TAKE_PARAM) + 80 +2, sizeof(char));
    strcat(tmp, TAKE_PARAM);
    strcat(tmp, param_name);
    strcat(tmp, "'");
  // printf("tmp = |%s|\n",tmp);
    state = doSQL(myconnection, tmp);
    free(tmp);
    
    return state;
}
//==================================================================================//
#define TAKE_LONG_PARAM "SELECT pv.value FROM stringParameters p, stringParametersValues pv, activeScheme act WHERE pv.scheme_id = act.activeScheme_id AND pv.stringParam_id = p.stringParam_id AND p.stringParam_name = '"
int GetParam::query_TakeLongParam(const char* param_name) {
   int state;
    char * tmp = (char *) calloc(strlen(TAKE_LONG_PARAM) + 80 +2, sizeof(char));
    strcat(tmp, TAKE_LONG_PARAM);
    strcat(tmp, param_name);
    strcat(tmp, "'");
   // printf("tmp = |%s|\n",tmp);
    state = doSQL(myconnection, tmp);
    free(tmp);
    
    return state;
}
//==================================================================================//
int GetParam::TakeParam(const char* name_param, bool* param_value)   {

    int state = query_TakeParam(name_param);
   // printf("state = |%d|\n",state);
    if ( state != 2) return state;
    int n = PQntuples(result);
   // printf("n = |%d|\n",n);
    if ( n != 1 ) return -3;
    
    if ( strcmp(PQgetvalue(result, 0,0), "1") == 0 ) 
      *(param_value) = true;
    else 
      *(param_value) = false;
    
    PQclear(result);
    return 0;     
}
//==================================================================================//
int GetParam::TakeParam(const char* name_param, int* param_value)   {

    int state = query_TakeParam(name_param);
    if ( state != 2) return state;
    int n = PQntuples(result);
    if ( n != 1 ) return -3;

    *(param_value) = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);
    return 0;
}
//==================================================================================//
int GetParam::TakeParam(const char* name_param, double* param_value)   {
    int state = query_TakeParam(name_param);
    if ( state != 2) return state;
    int n = PQntuples(result);
    if ( n != 1 ) return -3;

    *(param_value) = atof(PQgetvalue(result, 0, 0));

    PQclear(result);
    return 0;
}
//==================================================================================//
int GetParam::TakeParam(const char* name_param, char*& param_value)   {
    //printf("TaqkeParam(%s) \n", name_param);
    int state = query_TakeParam(name_param);
    if ( state != 2) return state;
    int n = PQntuples(result);
    if ( n != 0 && n != 1 )  return -3;
    if (n == 0) {  state = query_TakeLongParam(name_param);    }
    if ( state != 2 ) return state;
    n = PQntuples(result);
    if ( n != 1 )  return -3;
    free(param_value);
    param_value = (char*) calloc(PQgetlength(result, 0, 0)+3, sizeof(char));
    strcpy(param_value, PQgetvalue(result, 0, 0));
    //printf("param v itoge = %s \n", param_value);
    PQclear(result);
    return 0;
}
//==================================================================================//

//---(+)---Zuskin---28/02/2012---
#define GET_SECTION_CASH "SELECT section_cash FROM sections WHERE section_number = "
double GetParam::GetSectionCash(int section_num) {

    string str = GET_SECTION_CASH;
    
    str+= toString<int>(section_num+1);
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    double res = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;

}
//-------------------------------



