
#ifndef READER_CONFIG
#define READER_CONFIG

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "addon_func.h"

const int  LEN_BUF = 500;
const int COUNT_PARAM = 7;

class ReaderConfig {
   
   public:
      //-----(*)-----Zuskin-----20/12/2011-----
      //ReaderConfig(const char* aconfig_path);
      ReaderConfig(const char* aconfig_path, bool try_home = true);
      //---------------------------------------
      ~ReaderConfig();
      void GetDB(char* a_nameDB);   
      void GetUser(char* a_user);
      void GetPass(char* a_pass);
      void GetHost(char* a_host);
      void GetHostAddr(char* a_hostaddr);
      void GetPort(char* a_port);
      void GetLog(char* a_log_path);
      
      int GetDBLength();
      int GetUserLength();
      int GetPassLength();
      int GetHostLength();
      int GetHostAddrLength();
      int GetPortLength();
      int GetLogLength();
      
      
   private:
      char* config_path;
      char* nameDB;
      char* user;
      char* pass;
      char* host;
      char* hostaddr;
      char* port;
      char* log_path;
      FILE* f_config;
      void rstr(char* str);
      int FillParam();
      void TakeParam(int n, char* str);
      
};

#endif
