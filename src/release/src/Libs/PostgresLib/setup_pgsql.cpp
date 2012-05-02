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

#include "setup_pgsql.h"
#include "addon_func.h"

//==================================================================================//
SetupParameters::SetupParameters()
{
    Set_Db("LinCash");
    log->PrintString(log_type, INFO, "SetupParameters::SetupParameters - Connect to db parameters DB name = Setup \n");
}

//==================================================================================//
SetupParameters::SetupParameters(const char * conf_path) : PGSQL(conf_path)
{
    //printf("SetupParameters::SetupParameters(conf_path = %s)\n", conf_path);
    log->PrintString(log_type, INFO, "SetupParameters::SetupParameters - Connect to db parameters form path = %s \n", conf_path);
}

//==================================================================================//
SetupParameters::SetupParameters(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    //printf("AccessRights::AccessRights(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
    log->PrintString(log_type, INFO, "SetupParameters::SetupParameters - Connect to db with  parameters : dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
}

//==================================================================================//
SetupParameters::~SetupParameters()
{
    //puts(" SetupParameters::~SetupParameters() ");
    //close(log);
    //Close_Connection();
}

//==================================================================================//
// определение количества записей о схемах в БД
#define GET_COUNT_SCHEMES "SELECT COUNT(*) FROM schemes"
int SetupParameters::GetCountSchemes(int * countschemes)
{
     int state;
     char *str;
    
     //printf("SetupParameters::GetCountSchemes()\n");
     
     str = (char *) calloc(strlen(GET_COUNT_SCHEMES) + 2, sizeof(char));
     if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
     strcpy(str, GET_COUNT_SCHEMES);
    
     state = doSQL(myconnection, str);
     free(str);
     int n = 0;
     if (state == 2) n = PQntuples(result); else return state;
     if (n>0) (*countschemes) = atoi(PQgetvalue(result,0,0));   
     
     PQclear(result);

     return state;
}

//==================================================================================//
int SetupParameters::GetSchemesNames( char** schemes_names, int len_schemes_names)
{
   //printf("SetupParameters::GetSchemesNames()\n");
  
   int res = query_GetSchemesNames();
   int n = 0;   
   if (res == 2) n = PQntuples(result); else return res;
   
   if (n>0)  {
       // вдруг результаты запросов не совпадают 
       if (len_schemes_names != n) { 
	   //printf("Error: definition schmes count!!"); 
	   log->PrintString(log_type, ERROR, " SetupParameters::GetSchemesNames - data base request error" );
	   return -4; 
       } 

       for (int i=0; i<n; ++i)       {
	//schemes_names[i] = (char*) realloc(schemes_names[i], sizeof(char)*(PQgetlength(result, i, 0))+1);
	strcpy(schemes_names[i], PQgetvalue(result, i, 0));       }  
   }
   PQclear(result);
   return res;
}

//==================================================================================//
#define GET_SCHEMES_NAMES "SELECT scheme_name FROM schemes"
int SetupParameters::query_GetSchemesNames()
{
    int state;
    char *str;
    //printf("SetupParameters::query_GetSchemesNames( )\n");
    str = (char *) calloc(strlen(GET_SCHEMES_NAMES) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcpy(str, GET_SCHEMES_NAMES);
   
    state = doSQL(myconnection, str);
    free(str);
    return state;     
}

//==================================================================================//
int SetupParameters::GetActiveScheme(char* act_scheme)
{
     //printf("SetupParameters::GetActiveScheme(%s)\n", act_scheme);
     int res = query_GetActiveScheme();
     if (res != 2) return res;
     int n = PQntuples(result);
     if ( n == 1 ) {
          strcpy(act_scheme, PQgetvalue(result,0,0));
     }
     PQclear(result);
     //printf("active scheme = %s \n", act_scheme);	
     return res;
}

//==================================================================================//
#define GET_ACTIVE_SCHEME "SELECT  s.scheme_name FROM activeScheme a, schemes s WHERE s.scheme_id = a.activeScheme_id"
//#define GET_ACTIVE_SCHEME "SELECT  scheme_name FROM schemes WHERE scheme_id = 1"
int SetupParameters::query_GetActiveScheme()
{
    //printf("SetupParameters::query_GetActiveScheme()\n");
     int state;
     char *str;
       
     str = (char *) calloc(strlen(GET_ACTIVE_SCHEME) + 2, sizeof(char));
     if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
     strcpy(str, GET_ACTIVE_SCHEME);
    
     state = doSQL(myconnection, str);
     free(str);
     return state;
}
//==================================================================================//
#define GET_COUNT_PARAM "SELECT COUNT(*) FROM parameters"
int SetupParameters::GetCountParameters(int * count_param)
{
      int state;
      char *str;
      int n = 0;
      
     // printf("SetupParameters::GetCountParameters()\n");
      
      str = (char *) calloc(strlen(GET_COUNT_PARAM ) + 2, sizeof(char));
      if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
      strcpy(str, GET_COUNT_PARAM);
      state = doSQL(myconnection, str);
      free(str);
      
      if ( state != 2) return state;
      n = PQntuples(result);
      
      if ( n!=1 ) return -1;      
      (*count_param) = atoi(PQgetvalue(result,0,0));
      
      PQclear(result);
      
      state = query_GetCountStringParam();
      if (state != 2) return state;
      n = PQntuples(result);
      if ( n != 1) return -1;
      (*count_param) = (*count_param) + atoi(PQgetvalue(result,0,0));
      
      PQclear(result);
      
      return state;
}
//==================================================================================//
#define GET_COUNT_STRING_PARAM "SELECT COUNT(*) FROM stringParameters"
int SetupParameters::query_GetCountStringParam()
{
    int state;
    char* str;
    str = (char*) calloc(strlen(GET_COUNT_STRING_PARAM) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str,GET_COUNT_STRING_PARAM);
    //printf("SQL str = %s\n", str);
    state = doSQL(myconnection, str);
    free(str);
    return state;    
}
//==================================================================================//
// возвращает 0 - если все хорошо, или res если не очень
int SetupParameters::GetParamID(int * param_id, char** param_names, int param_count)
{
    //printf("SetupParameters::GetParamID(...)\n");
    int k = 0;
    int res;
    int n = 0;
    
    for (int i=0; i<param_count; i++)   {
	if ( param_names[i] == NULL ) { param_id[i] = -1;  continue; }
	if ( strlen(param_names[i]) == 0 ) { param_id[i] = -1;  continue; }
	param_id[i] = -1;
	res = query_GetParamID( param_names[i] ); 
	n = 0;
        //printf("+++++++++++2.1+++++++++ res = %s \n", res);
	//printf("Get id param_names[%d] = %s , OK. res = %d\n", i, param_names[i], res);
	if ( res != 2 ) { 
	    param_id[i] = -1; 
	    //puts("res != 2");  
	    continue; 
	}
	n = PQntuples(result);
	if ( n ==1 ) {
	    param_id[i] = atoi( PQgetvalue( result, 0, 0 ) ); 
	    k++;  
	    //printf("param_name = %s, param_id = %d\n", param_names[i] , param_id[i] );
	    PQclear(result);
	    continue; }
	
	if ( n == 0)  res = query_GetStringParamId( param_names[i] );	
	if ( res != 2 ) { param_id[i] = -1;  continue; }
	//printf("param_names[%d] = %s  - string param \n", i, param_names[i]);
	n = PQntuples(result);
	if ( n == 1 )  {
	    param_id[i] = atoi( PQgetvalue( result, 0, 0 ) ); 
	    k++; 
	}
	//printf("param_name = %s, param_id = %d\n", param_names[i] , param_id[i] );
	PQclear(result);
    }
    
    
    
    //printf("was init %d parameters\n", k);
    log->PrintString(log_type, INFO, "SetupParameters::GetParamID. was init %d parameters \n", param_count - k);
    if (param_count != k)  log->PrintString(log_type, ERROR, "SetupParameters::GetParamID. %d parametrs have not id !!! \n", param_count - k);
    //printf("%d parameters have not id !!!! \n", param_count - k); 
    
    return 0;
}

//==================================================================================//
#define GET_PARAM_ID "SELECT param_id FROM parameters WHERE param_name = '"
int SetupParameters::query_GetParamID(char* param_name)
{
    //printf("SetupParameters::query_GetParamID(%s)\n", param_name);
    int state;
    char* str;
    str = (char*) calloc(strlen(GET_PARAM_ID) + strlen(param_name) + 4, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str,GET_PARAM_ID);
    strcat(str, param_name);
    strcat(str,"'");
    //printf("SQL str = %s\n", str);
    state = doSQL(myconnection, str);
    free(str);
    //printf("query get param id return = %d \n", state);
    return state;    
}
//==================================================================================//
#define GET_STRING_PARAM_ID "SELECT stringParam_id FROM stringParameters WHERE stringParam_name ='"
int SetupParameters::query_GetStringParamId(char* param_name)
{
    int state;
    char* str;
    str = (char*) calloc(strlen(GET_STRING_PARAM_ID) + strlen(param_name) + 4, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str,GET_STRING_PARAM_ID);
    strcat(str, param_name);
    strcat(str,"'");
    state = doSQL(myconnection, str);
    free(str);
    return state;    
}
//==================================================================================//
int SetupParameters::GetDefaultParam(char** param_value, int * param_id, int param_count)
{
     //printf("SetupParameters::GetDefaultParam()\n");
     
     int k = 0;
     int n = 0;
     for (int i=0; i < param_count; i++)  {
	 if (param_id[i] == -1) continue;
	 if (param_id[i] < 500)  {
	     int res = query_GetDefaultParam( itoa(param_id[i]) );
	     
	     if (res <0)   { continue; }
	     if (res == 2) {n = PQntuples(result);}
	     if ( n != 1 ) {continue;}
	     
	     //param_value[i] = (char*) realloc(param_value[i], sizeof(char)*(PQgetlength(result, 0, 0) + 1));
	     strcpy(param_value[i], PQgetvalue(result, 0, 0));
	     k++;
	     PQclear(result);  }
	 if (param_id[i] >500) {
	     int res = query_GetDefaultStringParam( itoa(param_id[i]) );
	     
	     if (res <0)   { continue; }
	     if (res == 2) {n = PQntuples(result);}
	     if (n < 0) {continue;}
	     
	     param_value[i] = (char*) realloc(param_value[i], sizeof(char)*(PQgetlength(result, 0, 0)*2 + 2));
	     strcpy(param_value[i], PQgetvalue(result, 0, 0));
	     k++;
	     PQclear(result);
	 }
     }
     
     log->PrintString(log_type, INFO, "SetupParameters::GetDefaultParam. %d parameters was got \n", k);
     //printf("%d parameters was got \n", k);
     return 0;
}

//==================================================================================//
#define GET_DEFAULT_PARAM "SELECT default_value FROM parameters WHERE param_id = '"
int SetupParameters::query_GetDefaultParam(char* param_id)
{
    int state;
    char *str;
    //printf("SetupParameters::query_GetDefaultParam( )\n");
    str = (char *) calloc(strlen(GET_DEFAULT_PARAM)  + strlen(param_id) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_DEFAULT_PARAM);
    strcat(str, param_id);
    strcat(str, "'");
   
    state = doSQL(myconnection, str);
    free(str);
    return state;     
}

//==================================================================================//
#define GET_DEFAULT_STRING_PARAM "SELECT default_value FROM stringParameters WHERE stringParam_id = '"
int SetupParameters::query_GetDefaultStringParam(char* param_id)
{
    int state;
    char *str;
    
    str = (char *) calloc(strlen(GET_DEFAULT_STRING_PARAM)  + strlen(param_id) + 4, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_DEFAULT_STRING_PARAM);
    strcat(str, param_id);
    strcat(str, "'");
   
    state = doSQL(myconnection, str);
    free(str);
    return state;     
}
//==================================================================================//
int SetupParameters::GetSchemeParam(char* scheme, char** param_values, int* param_id, int param_count)
{    
     //printf("SetupParameters::GetSchemeParam(%s)\n",scheme);
     log->PrintString(log_type, INFO, "SetupParameters::GetSchemeParam(%s).\n", scheme);
     int k = 0;
     int n = 0;
     for (int i = 0; i < param_count; i++)    {
	 if ( param_id[i] == -1 ) continue; 
	 //printf("param_id [%d] = %d \n", i, param_id[i]);
	 if ( param_id[i] < 500 ) {
	     int res = query_GetSchemeParam(scheme, itoa(param_id[i]));
	 
	     if ( res < 0 ) {return res;}
	     if ( res == 2 ) n = PQntuples(result);
	     if ( n != 1 ) { 
		 //printf("the param num %d have not id \n", param_id[i]); 
		 log->PrintString(log_type, INFO, "SetupParameters::GetSchemeParam(%s). the parameter number %d have not id \n", scheme, param_id[i]);
		 continue; }
	     
	     //param_values[i] = (char*) realloc(param_values[i], sizeof(char)*(PQgetlength(result, 0, 0)*2 + 1));
	     strcpy(param_values[i], PQgetvalue(result, 0, 0));
	     k++;
	     PQclear(result);  }
	 
	 if ( param_id[i] > 500 ) {
	     
	     int res = query_GetSchemeStringParam(scheme, itoa(param_id[i]));
	     if ( res < 0 ) {return res;}
	     if ( res == 2 ) n = PQntuples(result);
	     if ( n != 1 ) { continue; }
	     
	     param_values[i] = (char*) realloc(param_values[i], sizeof(char)*(PQgetlength(result, 0, 0)*2 + 1));
	     strcpy(param_values[i], PQgetvalue(result, 0, 0));
	     k++;
	     //printf("param[%d] is string. id = %d value = %s \n", i, param_id[i], param_values[i] );
	     PQclear(result);  }
     }
     
     //printf("%d parameters was got \n", k);
     log->PrintString(log_type, INFO, "SetupParameters::GetSchemeParam(%s). %d parameters was got\n", scheme, k);
     return 0;
}
//==================================================================================//
#define GET_SCHEME_STRING_PARAM1 "SELECT v.value FROM stringParametersValues v, schemes s WHERE v.scheme_id = s.scheme_id AND s.scheme_name = '"
#define GET_SCHEME_STRING_PARAM2 "' AND v.stringParam_id = '"
int SetupParameters::query_GetSchemeStringParam(char* scheme, char* param_id)
{
    int state;
    char *str;
    str = (char *) calloc(strlen(GET_SCHEME_STRING_PARAM1) +strlen(scheme)+ strlen(GET_SCHEME_STRING_PARAM2) + strlen(param_id)  + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_SCHEME_STRING_PARAM1);
    strcat(str, scheme);
    strcat(str,GET_SCHEME_STRING_PARAM2);
    strcat(str, param_id);
    strcat(str, "'");
    state = doSQL(myconnection, str);
    free(str);
    return state;    
}
//==================================================================================//
#define GET_SCHEME_PARAM1 "SELECT v.value FROM parametersValues v, schemes s WHERE v.scheme_id = s.scheme_id AND s.scheme_name = '"
#define GET_SCHEME_PARAM2 "' AND v.param_id = '"
int SetupParameters::query_GetSchemeParam(char* scheme, char* param_id)
{
    int state;
    char *str;
    str = (char *) calloc(strlen(GET_SCHEME_PARAM1) +strlen(scheme)+ strlen(GET_SCHEME_PARAM2) + strlen(param_id)  + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_SCHEME_PARAM1);
    strcat(str, scheme);
    strcat(str,GET_SCHEME_PARAM2);
    strcat(str, param_id);
    strcat(str, "'");
    state = doSQL(myconnection, str);
    free(str);
    return state;    
}
//==================================================================================//
int SetupParameters::SetSchemeParam(const char* scheme_name, int * param_id, char** param_value, int param_count)
{
    //printf("itoa(2) %s\n",itoa(2));
    //printf("SetupParameters::SetSchemeParam(%s)\n",scheme_name);
    int res = 0;
    res = doSQL(myconnection, "BEGIN WORK" );
    if ( res < 0) return res;
    
    for (int i=0; i<param_count; ++i)     {
	//printf("param_values[%d] = %s \n", i, param_value[i]);
	if  ( param_id[i] == -1) continue;
	if  ( param_id[i] < 500 ) {
	    res = query_UpdateSchemeParam(scheme_name, itoa(param_id[i]), param_value[i]);
	    if ( atoi(PQcmdTuples(result)) == 0 ) res = query_InsertSchemeParam(scheme_name, itoa(param_id[i]), param_value[i]);
	}
	if ( param_id[i] > 500 ) {
	    res = query_UpdateSchemeStringParam(scheme_name, itoa(param_id[i]), param_value[i]);
	    if ( atoi(PQcmdTuples(result)) == 0 ) res = query_InsertSchemeStringParam(scheme_name, itoa(param_id[i]), param_value[i]);
	}
	//printf("result = %d\n", res);
	if (res<0) {
	    res = doSQL(myconnection, "ROLLBACK WORK" );
	    if (res != 1) return res;
	    return -1;	}
    }
    res = doSQL(myconnection, "COMMIT WORK" );
    if (res < 0) return res;
    return res;
}

//==================================================================================//
#define UPDATE_PARAM_SET "UPDATE parametersValues SET value = '"
#define UPDATE_PARAM_FROM "' FROM schemes s WHERE parametersValues.scheme_id = s.scheme_id AND s.scheme_name = '"
#define UPDATE_PARAM_PARAM_ID "' AND parametersValues.param_id = "
int SetupParameters::query_UpdateSchemeParam(const char* scheme_name, char* param_id, char* param_value)
{
    //printf("SetupParameters::query_UpdateSchemeParam(%s,%s, %s)\n", scheme_name, param_id, param_value);
    int state;
    char* str;
    
    int l = strlen(UPDATE_PARAM_SET) + strlen(UPDATE_PARAM_FROM) + strlen(UPDATE_PARAM_PARAM_ID);
    l += strlen(param_value) + strlen(scheme_name) + strlen(param_id)+4;
    str = (char*) calloc(l, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, UPDATE_PARAM_SET);
    strcat(str, param_value);
    strcat(str, UPDATE_PARAM_FROM);
    //strcat(str, UPDATE_PARAM_SCHEME);
    strcat(str, scheme_name);
    strcat(str, UPDATE_PARAM_PARAM_ID);
    strcat(str, param_id);
    //printf("str = %s\n", str);
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}
//==================================================================================//
#define INSERT_SCHEME_PARAM "INSERT INTO parametersValues (param_id , scheme_id, value) SELECT  "
#define INSERT_SCHEME_PARAM_2 ", scheme_id, '"
#define INSERT_SCHEME_PARAM_3 "' FROM schemes WHERE scheme_name = '"
int SetupParameters::query_InsertSchemeParam(const char* scheme_name, char* param_id, char* param_value)
{
    //printf("SetupParameters::query_InsertSchemeParam(%s,%s, %s)\n", scheme_name, param_id, param_value);
    int state;
    char* str;
    
    int l = strlen(INSERT_SCHEME_PARAM) + strlen(INSERT_SCHEME_PARAM_2) + strlen(INSERT_SCHEME_PARAM_3) +  strlen(scheme_name) + strlen(param_id) + strlen(param_value) + 2;
    
    str = (char*) calloc(l, sizeof(char));
    
    strcat(str, INSERT_SCHEME_PARAM);
    strcat(str, param_id);
    strcat(str, INSERT_SCHEME_PARAM_2);
    strcat(str, param_value);
    strcat(str, INSERT_SCHEME_PARAM_3);
    strcat(str, scheme_name);    
    strcat(str, "'");
    
    
    //printf("str = %s\n", str);
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}
//==================================================================================//

//==================================================================================//
#define UPDATE_STRING_PARAM_SET "UPDATE stringParametersValues SET value = '"
#define UPDATE_STRING_PARAM_FROM "' FROM schemes s WHERE stringParametersValues.scheme_id = s.scheme_id AND s.scheme_name = '"
#define UPDATE_STRING_PARAM_AND "' AND stringParametersValues.stringParam_id = "
int SetupParameters::query_UpdateSchemeStringParam(const char* scheme_name, char* param_id, char* param_value)
{
    printf("SetupParameters::query_SetSchemeStringParam(%s,%s, %s)\n", scheme_name, param_id, param_value);
    int state;
    char* str;
    
    int l = strlen(UPDATE_STRING_PARAM_SET) + strlen(UPDATE_STRING_PARAM_FROM) + strlen(UPDATE_STRING_PARAM_AND);
    l += strlen(param_value) + strlen(scheme_name) + strlen(param_id)+4;
    str = (char*) calloc(l, sizeof(char));

    strcat(str, UPDATE_STRING_PARAM_SET);
    strcat(str, param_value);
    strcat(str, UPDATE_STRING_PARAM_FROM);
    //strcat(str, UPDATE_PARAM_SCHEME);
    strcat(str, scheme_name);
    strcat(str, UPDATE_STRING_PARAM_AND);
    strcat(str, param_id);
    //printf("str = %s\n", str);
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}
//==================================================================================//
#define INSERT_SCHEME_STRING_PARAM "INSERT INTO parametersValues (param_id , scheme_id, value) SELECT  "
#define INSERT_SCHEME_STRING_PARAM_2 ", scheme_id, '"
#define INSERT_SCHEME_STRING_PARAM_3 "' FROM schemes WHERE scheme_name = '"
int SetupParameters::query_InsertSchemeStringParam(const char* scheme_name, char* param_id, char* param_value)
{
    //printf("SetupParameters::query_InsertSchemeParam(%s,%s, %s)\n", scheme_name, param_id, param_value);
    int state;
    char* str;
    
    int l = strlen(INSERT_SCHEME_STRING_PARAM) + strlen(INSERT_SCHEME_STRING_PARAM_2) + strlen(INSERT_SCHEME_STRING_PARAM_3) +  strlen(scheme_name) + strlen(param_id) + strlen(param_value) + 2;
    
    str = (char*) calloc(l, sizeof(char));
    
    strcat(str, INSERT_SCHEME_STRING_PARAM);
    strcat(str, param_id);
    strcat(str, INSERT_SCHEME_STRING_PARAM_2);
    strcat(str, param_value);
    strcat(str, INSERT_SCHEME_STRING_PARAM_3);
    strcat(str, scheme_name);    
    strcat(str, "'");
    
    
    //printf("str = %s\n", str);
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}
//==================================================================================//
#define MAX_SCHEME_ID "SELECT MAX(scheme_id) FROM schemes"
int SetupParameters::GetNewSchemeName(char* sname)
{
    //printf("SetupParameters::GetNewSchemeName()\n");

    char* str;
    int state;
    
    str = (char*) calloc(strlen(MAX_SCHEME_ID) + 3, sizeof (char));
    strcat(str, MAX_SCHEME_ID);
    state = doSQL(myconnection, str);
    
    if (state ==2)  {
	int n = PQntuples(result);
	if (n>0) {
	    //sname = (char*) realloc(sname, sizeof(char) * (strlen("Scheme") + PQgetlength(result, 0,0) + 1));
	    strcat(sname,"Scheme");
	    strcat(sname, itoa(atoi(PQgetvalue(result,0,0)) + 1));	}
        PQclear(result);    
    }
    free(str);
    return state;
}
//==================================================================================//
// return 0 - OK
// return 1 - такое имя уже есть
//return -1 - error
int SetupParameters::AddNewScheme(char* name)
{
    //printf("SetupParameters::AddNewScheme(%s)\n", name);
    int n = -1;
    int res;
    
    //проверить отсутствие схемы с таким именем
    res = query_CheckSchemeName(name);
    if (res!=2) {
	log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). error db request  \n", name);
	//printf("Scheme name was not check\n"); 
	return -1; 
    }
    if (PQntuples(result)>0)  {
	log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). error db request  \n", name);
	n = atoi(PQgetvalue(result,0,0)); 
    }
    if (n!=0) {
	log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). scheme name was exist  \n", name);
	//printf("the scheme name was exist\n"); 
	return 1;  
    } 
    
    int k = doSQL(myconnection, "BEGIN WORK" );
    if (k  < 0) return k;
    
    //если такой схемы нет добавить
    res = query_AddScheme(name);
    if ( res < 0 )  {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if ( k< 0) return k;
		log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). error db request  \n", name);
		//printf("Scheme was not add!!!\n"); 
		return -1;
    }
    
    //добавить в таблицу со значениями значения параметров по умолчанию
    res = query_InsertParamValues(name);
    if ( res < 0 )  {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		//printf("O no!! The parameters for %s was not add!!!\n", name); 
		log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). error add paramters to scheme \n", name );
		return -1;
    }
    res = query_InsertStringParamValues(name);
    if (res<0)  {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		//printf("O no!! The parameters for %s was not add!!!\n", name); 
		log->PrintString(log_type, ERROR, "SetupParameters::AddNewScheme(%s). error add paramters to scheme \n", name );
		return -1;
    }
    
    k = doSQL(myconnection, "COMMIT WORK" );
    if (k < 0) return k;
    return 0;
}

//==================================================================================//
#define CHECK_SCHEME_NAME "SELECT COUNT(*) FROM schemes WHERE scheme_name = '"
int SetupParameters::query_CheckSchemeName(const char* name)
{
    //printf("SetupParameters::query_CheckSchemeName(%s)\n", name);
    char* str;
    int state;
    
    str = (char*) calloc(strlen(CHECK_SCHEME_NAME) + strlen(name) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, CHECK_SCHEME_NAME);
    strcat(str, name);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}

//==================================================================================//
#define ADD_SCHEME "INSERT INTO Schemes (scheme_name) VALUES('"
int SetupParameters::query_AddScheme(char* name)
{
    int state;
    char* str;
    //printf("SetupParameters::AddScheme(%s )\n",name);
    str = (char *) calloc(strlen(ADD_SCHEME) + strlen(name) + 4, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcpy(str, ADD_SCHEME);
    strcat(str, name);
    strcat(str, "')");
  
   state = doSQL(myconnection, str);
   
   free(str);
   return state;
}

//==================================================================================//
#define INSERT_PARAM_VALUE " INSERT INTO parametersValues (param_id, scheme_id, value) SELECT p.param_id, s.scheme_id, p.default_value  FROM parameters p, schemes s WHERE s.scheme_name = '"
int SetupParameters::query_InsertParamValues(char* schemename)
{
    //printf("SetupParameters::query_InsertParamValues(%s)\n", schemename);
    
    int state;
    char* str;
    
    str = (char*) calloc(strlen(INSERT_PARAM_VALUE) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, INSERT_PARAM_VALUE);
    strcat(str, schemename);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    free(str);  
    return state;
}

//==================================================================================//
#define INSERT_STRING_PARAM_VALUE " INSERT INTO stringParametersValues (stringParam_id, scheme_id, value) SELECT sp.stringParam_id, s.scheme_id, sp.default_value  FROM stringParameters sp, schemes s WHERE s.scheme_name = '"
int SetupParameters::query_InsertStringParamValues(char* schemename)
{
    //printf("SetupParameters::query_InsertParamValues(%s)\n", schemename);
    
    int state;
    char* str;
    
    str = (char*) calloc(strlen(INSERT_STRING_PARAM_VALUE) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, INSERT_STRING_PARAM_VALUE);
    strcat(str, schemename);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    free(str);  
    return state;
}
//==================================================================================//
int SetupParameters::DeleteScheme(char* name)
{
    //printf("SetupParameters::DeleteSchemes(%s)\n", name);
    int res;
    
    // елси удаляемая схема не активна!!!!! то ее можно удалить
    res = query_CompareActiveScheme(name);
    if (res!=2) {
	log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). error sql request \n", name );
	//printf("Cannot compare with active scheme!!!!\n"); 
	return res;
    }
    
    //printf("Compare return %d strok\n", PQntuples(result));
    if (PQntuples(result) > 0) {
	log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). Active scheme cannot be delete \n", name );
	//printf("Active scheme cannot be delete!!\n");
	return res;  }
    PQclear(result);
    
    int k = doSQL(myconnection, "BEGIN WORK" );
    if (k < 0) return k;
    
    //удаление папраметров схемы
    res = query_DeleteParam(name);
    if (res<0) {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). error of delete parameters in scheme \n", name );
		//printf("Parameters chemes(%s) was not delete\n", name);  
		return res;   }
    
    res = query_DeleteStringParam(name);
    if (res<0) {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). error of delete parameters in scheme \n", name );
		//printf("Parameters chemes(%s) was not delete\n", name);  
		return res;  }
    
    //удаление фиксированных скидок схемы
    res = query_DeleteDis(name);
    if (res<0) {
	  k = doSQL(myconnection, "ROLLBACK WORK" );
	if (k < 0) return k;
	log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). error sql request \n", name );
	//printf("Discounts chemes(%s) was not delete\n", name);  
	return res;  }
    
    //удаление самой схемы 
    res = query_DeleteScheme(name);
    if (res<0) {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		log->PrintString(log_type, ERROR, "SetupParameters::DeleteScheme(%s). error sql request \n", name );
		return res; 
    }
    //сохраненике измненений
    k = doSQL(myconnection, "COMMIT WORK" );
    if (k < 0) return k;
    return res;
}
//==================================================================================//
#define COMPARE_ACTIVE_SCHEME "SELECT a.activeScheme_id FROM activeScheme a, schemes s WHERE a.activeScheme_id = s.scheme_id AND s.scheme_name = '"
int SetupParameters::query_CompareActiveScheme(char* schemename)
{
    //printf("SetupParameters::query_CompareAtiveScheme(%s)\n", schemename);
    int state;
    
    char* str;
    str = (char*) calloc(strlen(COMPARE_ACTIVE_SCHEME) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, COMPARE_ACTIVE_SCHEME);
    strcat(str, schemename);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}

//==================================================================================//
#define DELETE_PARAM "DELETE FROM parametersValues WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
int SetupParameters::query_DeleteParam(char* schemename)
{
    //printf("SetupParameters::query_DeleteParam(%s)\n", schemename);
    int state;
    char* str;
    
    str = (char*) calloc(strlen(DELETE_PARAM) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, DELETE_PARAM);
    strcat(str, schemename);
    strcat(str, "')");
    
    state = doSQL(myconnection, str);
    
    free(str);  
    return state;
}

//==================================================================================//
#define DELETE_STRING_PARAM "DELETE FROM stringParametersValues WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
int SetupParameters::query_DeleteStringParam(char* schemename)
{
    //printf("SetupParameters::query_DeleteParam(%s)\n", schemename);
    int state;
    char* str;
    
    str = (char*) calloc(strlen(DELETE_STRING_PARAM) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, DELETE_STRING_PARAM);
    strcat(str, schemename);
    strcat(str, "')");
    
    state = doSQL(myconnection, str);
    
    free(str);  
    return state;
}
//==================================================================================//
#define DELETE_DIS "DELETE FROM Fix_reduction WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
int SetupParameters::query_DeleteDis(char* schemename)
{
    //printf("SetupParameters::query_DeleteParam(%s)\n", schemename);
    int state;
    char* str;
    
    str = (char*) calloc(strlen(DELETE_DIS) + strlen(schemename) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, DELETE_DIS);
    strcat(str, schemename);
    strcat(str, "')");
    
    state = doSQL(myconnection, str);
    
    free(str);  
    return state;
}
//==================================================================================//
#define DELETE_SCHEMES "DELETE FROM schemes WHERE scheme_name = '"
int SetupParameters::query_DeleteScheme(char* sname)
{
    //printf("SetupParameters::query_DeleteScheme(%s)\n", sname);
    int state;
    char* str;
    
    str = (char*) calloc(strlen(DELETE_SCHEMES) + strlen(sname) + 3, sizeof(char));
    if (str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, DELETE_SCHEMES);
    strcat(str, sname);
    strcat(str, "'");
    
    //printf("str = %s", str);
    state = doSQL(myconnection, str);
    
    free(str);
    return state;
}

//==================================================================================//
#define INSERT_ACTIVE_SCHEME  "INSERT INTO activescheme SELECT scheme_id FROM schemes WHERE scheme_name = '"
#define UPDATE_ACTIVE_SCHEME "UPDATE activeScheme SET activeScheme_id = schemes.scheme_id FROM schemes WHERE schemes.scheme_name = '"
int SetupParameters::SetActiveScheme(char* sname)
{
    printf("SetupParameters::SetActiveScheme(%s)\n", sname);
    
    int res = query_GetActiveScheme();    
    if (res != 2) {
	//printf("Cannot get old active scheme"); 
	log->PrintString(log_type, ERROR, "SetupParameters::SetActiveScheme(%s). error sql request \n", sname );
	return -1;
    }
    int n = PQntuples(result);
    
    
    //printf("res getOldScheme = %d \n", res);
    //printf("return %d strok\n", PQntuples(result));
    char* str;
    if (n > 0) { 
	str = (char*) calloc(strlen(UPDATE_ACTIVE_SCHEME) + strlen(sname) + 3, sizeof(char));
	if (str == NULL) printf("NULL CALLOC!!! parameter str\n");
	strcat(str, UPDATE_ACTIVE_SCHEME);
	strcat(str, sname);
	strcat(str, "'");
    }  else {
	str = (char*) calloc(strlen(INSERT_ACTIVE_SCHEME) + strlen(sname) + 3, sizeof(char));
	if (str == NULL) printf("NULL CALLOC!!! parameter str\n");
	strcat(str, INSERT_ACTIVE_SCHEME);
	strcat(str, sname);
	strcat(str, "'");	
    }
    
    int k = doSQL(myconnection, "BEGIN WORK" );
    if (k  < 0) return k;
    
    res = doSQL(myconnection, str);    
    
    if (res<0)  {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		//printf("Active scheme was not change\n"); 
    }
    k = doSQL(myconnection, "COMMIT WORK" );
    if (k < 0) return k;
    return res;
}
//==================================================================================//
int SetupParameters::CheckSchemeName(const char *sname, bool* name_exist)
{
    int res;
    int n = 0;
    (*name_exist) = false;
    // проверить есть такая схему или еще нет
    res = query_CheckSchemeName(sname);

    if (res!=2) {
	log->PrintString(log_type, ERROR, "SetupParameters::CheckSchemeName(%s). error sql request \n", sname );
	//printf("Scheme name was not check\n"); 
	return -1; 
    }
    if (PQntuples(result)>0)  {n = atoi(PQgetvalue(result,0,0)); }
    if (n > 0) { 
	//printf("the scheme name was exist\n"); 
	log->PrintString(log_type, ERROR, "SetupParameters::CheckSchemeName(%s). scheme name was exist \n", sname );
	(*name_exist) = true; 
    } 
    PQclear(result);
    //(*name_exist) = false;
    return res;
}

//==================================================================================//
#define UPDATE_SCHEME1 "UPDATE schemes SET  scheme_name = '"
#define UPDATE_SCHEME2 "' WHERE scheme_name = '"
int SetupParameters::RenameScheme( const char* oldName, const char* newName)
{
    //printf("SetupParameters::RenameScheme(%s, %s)\n", oldName, newName);
    
    int k;
    char* str;
    int res;
    // переименовать
	k = doSQL(myconnection, "BEGIN WORK" );
    if (k < 0) return k;
    
    str = (char*) calloc(strlen(UPDATE_SCHEME1) + strlen(newName) + strlen(UPDATE_SCHEME2) + strlen(oldName) + 3, sizeof(char));
    if (str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, UPDATE_SCHEME1);
    strcat(str, newName);
    strcat(str, UPDATE_SCHEME2);
    strcat(str, oldName);
    strcat(str, "'");
    
    res = doSQL(myconnection, str);
    
    if (res < 0) {
		k = doSQL(myconnection, "ROLLBACK WORK" );
		if (k < 0) return k;
		return res;
    }
    k = doSQL(myconnection, "COMMIT WORK" );
    if (k < 0) return k;
    return res;
}
//==================================================================================//
/*#define DELETE_ACTIVE_SCHEME "DELETE FROM activeScheme"
int SetupParameters::DeleteActiveScheme()
{
    printf("SetupParameters:: DeleteActiveScheme()\n");
    
    return  doSQL(myconnection, DELETE_ACTIVE_SCHEME);;
}*/
	
//==================================================================================//	
