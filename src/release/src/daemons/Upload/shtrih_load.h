/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2010, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2010, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#ifndef LOAD_SHTRIH_H
#define LOAD_SHTRIH_H

#include <string>
#include <vector>
#include <qtextcodec.h>
#include "debug_level_log.h"
#include "upload_const.h"
#include "upload_pgsql.h"

extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;


//const DEBUG_LEVEL_ENUM log_type = LOGFILE;
//const DEBUG_LEVEL_ENUM user_log_type = LOGFILE;

using namespace std;


class Command {
public:
    Command(string line, int string_num = -1);
    void print();
    //CommandType getType();
    int execute(PgUpload *db, bool add_quantity);
private:
    string line_str;
    int str_num;
    //CommandType type;
    vector<string> parameters;
    void process(string line);	 // разбирает строку на параметры
    int correctWareGroup();
    int correctBarcode();
    int correctFixDiscount();
    int wareLoad(PgUpload * db, bool add_quantity);
    int groupLoad(PgUpload * db);
    int barcodeLoad(PgUpload * db);
    int fixdiscountLoad(PgUpload * db);
    
    string convertDouble(double value);
};

class ShtrihLoad {
public:
    ShtrihLoad();
    int load(const char* file_load_path);
    ~ShtrihLoad();
private:
    bool command_ware_clear;
    bool command_fix_clear;
    bool command_add_quantity;  // replace = true
    int err_count;
    void testCommand(string str);
    void executeCommand();
    string file_path;
    vector<Command> commands;
    PgUpload *pg_upload;
};


#endif
