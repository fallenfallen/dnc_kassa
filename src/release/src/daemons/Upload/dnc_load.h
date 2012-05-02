
#ifndef LOAD_DNC_H
#define LOAD_DNC_H

#include <string>
#include <vector>
#include "debug_level_log.h"
#include <qtextcodec.h>
#include <iostream>
#include <fstream>
#include "upload_const.h"

extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;



using namespace std;


class DncLoad {
public:
    DncLoad();
    int load(const char* file_load_path);
private:    
    int record_count;
    ifstream loadfile;
    int err_num;
    
    void ErrorFormat();
    void ErrorRead();    
    void ErrorDoubleLoad();
    void ErrorInStr(int str_num);
    void setLoadedState(const char* file_path, const char * state);
};


#endif
