#ifndef READER_CONF
#define READER_CONF

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <map>


#include "addon_func.h"
#include "const_conf.h"

using namespace std;

//void strrs(char* str);

class ReaderConf {
   
   public:
      //const static int LEN_BUF = 200;
      ReaderConf(const char* aconfig_path);
      ReaderConf();
      ~ReaderConf();
      int getLen(const char* str);
      //void GetLenUploadFilesPath(int* upload_falg_len, int* upload_file_len);
      //void GetLenUnloadFilesPath(int* unload_falg_len, int* unload_file_len);
      int GetUploadFlagPath(char* upload_flag);
      int GetUploadDataPath(char* upload_file);
      int GetUnloadFlagPath(char* unload_flag);
      int GetUnloadDataPath(char* unload_file);
      int GetNPC(char * n_pc);
      int GetWarePrefix(char* prefix);
      int GetHUParam(char* mark_trans);
      int GetProtocol(char * protocol);
		
   private:
      void TakeParam(string str);
      map <string, string> addresses;

};

#endif
