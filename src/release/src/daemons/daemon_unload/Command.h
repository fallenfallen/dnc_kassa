#ifndef COMMAND
#define COMMAND

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>  
#include <unistd.h>  

#include "unload_pgsql.h"
#include "trans_data.h"
#include "reader_config.h"
//#include "const_main.h"

extern int num_last_trans;
extern int n_protocol;

const int LEN_MAX = 50;

class Command 
{                         
   public:
    int command_count;
    int er_con;
    Command();
    ~Command();
    int DoCommand(char* command_str, char* param_str, int num_WS, FILE* f_unload);
    int GetCommandResultCount(char* command_str, char* param_str, int num_WS);
    void SetHUMarkTrans(bool param);
  private:
    bool hu_mark_last_trans;
    unload_pgsql* db;
    int ConvertCommand(char* command, char* param, char* result);  
    int ConvertParam(char* param, char** param_mas);
    
};

#endif
