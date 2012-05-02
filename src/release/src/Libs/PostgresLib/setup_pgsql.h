
#include "class_pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#ifndef SETUP_PARAM
#define SETUP_PARAM
class SetupParameters : public PGSQL
{
  public:
    SetupParameters();
    SetupParameters(const char * conf_path);
    SetupParameters(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path);
    ~SetupParameters();   
   
   int GetCountSchemes(int * countschemes);
   int GetSchemesNames( char** schemes_names, int len_schemes_names);
   int GetActiveScheme(char* act_scheme);
   int GetCountParameters(int * count_param);
   int GetParamID(int * param_id, char** param_name, int param_count);
   int GetDefaultParam(char** param_value, int* param_id, int param_count);
   int GetSchemeParam(char* scheme_name,  char** param_vlaue, int* param_id, int param_count);
   int SetSchemeParam(const char* scheme_name, int * param_id, char**param_value, int param_count);
   int GetNewSchemeName(char* sname);    
   int AddNewScheme(char * name);
   int DeleteScheme(char* name);
   int SetActiveScheme(char* sname);
   int CheckSchemeName(const char *sname, bool* name_exist);
   int RenameScheme(const char* oldName, const char* newName);
   //int DeleteActiveScheme();
   
private:
    int query_GetScheme(char* schemeName);
    int query_GetSchemesNames();
    int query_GetActiveScheme();
    
    int query_GetCountStringParam();

    int query_GetParamID(char* param_name);
    int query_GetStringParamId(char* param_name);

    int query_GetDefaultParam(char* param_id);
    int query_GetDefaultStringParam(char* param_id);
    
    int query_GetSchemeParam(char* scheme, char * param_id);
    int query_GetSchemeStringParam(char* scheme, char * param_id);
    
    int query_UpdateSchemeParam(const char* scheme_name, char* param_id, char* param_value);
    int query_UpdateSchemeStringParam(const char* scheme_name, char* param_id, char* param_value);
    int query_InsertSchemeParam(const char* scheme_name, char* param_id, char* param_value);
    int query_InsertSchemeStringParam(const char* scheme_name, char* param_id, char* param_value);
    
    //char* itoa(int d);
    int query_CheckSchemeName(const char* name);
    int query_AddScheme(char* name);
    int query_InsertParamValues(char* schemename);
    int query_InsertStringParamValues(char* schemename);
    
    int query_CompareActiveScheme(char* schemename);
    int query_DeleteParam(char* schemename);
    int query_DeleteStringParam(char* schemename);
    int query_DeleteDis(char* schemename);
    
    int query_DeleteScheme(char* sname);
    
};

#endif

















