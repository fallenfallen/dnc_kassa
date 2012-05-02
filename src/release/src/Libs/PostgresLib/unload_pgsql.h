#ifndef UNLOAD_PGSQL
#define UNLOAD_PGSQL

#include <stdlib.h>
#include <string.h>     
#include <malloc.h>
#include <stdio.h>
#include "class_pgsql.h"
#include "trans_data.h"


class unload_pgsql : public PGSQL
{
  public:
    unload_pgsql();
    //-----(*)-----Zuskin-----20/12/2011-----
    //unload_pgsql(const char * conf_path);
    unload_pgsql(const char * conf_path, bool try_home = true);
    //---------------------------------------
    unload_pgsql(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path);
    ~unload_pgsql();   
    int DoCommand_GetCount(char* SQLcommand, int *count_transaction);
    int GetResult(Transaction * tr_mas, int n);
    //int GetShtrihResult(Transaction * tr_mas, int n);
     
};

#endif


















