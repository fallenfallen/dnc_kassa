#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "class_pgsql.h"

#ifndef GET_PARAM
#define GET_PARAM



class GetParam : public PGSQL
{
  public:
    GetParam();
    GetParam(const char * conf_path);
    GetParam(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path);
    ~GetParam();   
    int TakeParam(const char* name_param, bool* param_value);
    int TakeParam(const char* name_param, int* param_value);
    int TakeParam(const char* name_param, char*& param_value);
    int TakeParam(const char* name_param, double* param_value);
    //---(+)---Zuskin---28/02/2012---
    double GetSectionCash(int section_num);
    //-------------------------------
  private:
    int query_TakeParam(const char* param_name);
    int query_TakeLongParam(const char* param_name);
};

#endif

















