/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2010, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@Dnc-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2010, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@Dnc-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include "dnc_load.h"
#include "stringconvert.h"
#include "DncCommand.h"
#include "WareSystem.h"
#include "Error.h"
#include "UsefulFunctions.h"

#include "Decoder.h"

#include <string>
#include <vector>
#include <fstream>


/******************************* DncLoad ***********************************/

DncLoad::DncLoad()
{
    record_count = 0;
}



int DncLoad::load(const char* file_load_path) 
{
    
    int record_count = 0;
    err_num = 0;

    loadfile.open(file_load_path);
    
    if ( ! loadfile.is_open() )  {
        ErrorRead();
	return -1;
    }
    
    string line;
    getline(loadfile,line);
    if ( line.find("!!!DNCUPLOAD") == string::npos ) {
        ErrorFormat();
	return -1;
    }

    getline(loadfile,line);
    if ( line.find("#UPLOADED_STATE") == string::npos ) {
        ErrorFormat();
	return -1;
    }
    
    getline(loadfile, line);
    if (line.find("uploaded") != string::npos) {
        ErrorDoubleLoad();
	return -1;
    }
    
    getline(loadfile, line);
    if ( line.find("#RECORD_COUNT") == string::npos ) {
        ErrorFormat();
	return -1;
    }

    getline(loadfile, line);
    record_count = fromString<int>(line);
    cout << record_count << endl;
    
    setLoadedState(file_load_path, "processing");
    
    //-----(+)-----Zuskin-----14/07/2011-----
    userlog.PrintString(user_log_type, INFO,WinCodec->fromUnicode(SystemCodec->toUnicode( "Загрузка товаров стартовала... \r\n")) );
    logfile.PrintString(log_type, INFO, "Load of wares started... \n");
    //---------------------------------------
    
    try {
        WareSystem::startInstance();
        
        DncCommand * command = DncCommand::getDefaulCommand();
        int str_num = 0;

        while ( !loadfile.eof() && command->noEnd() ) {
            getline(loadfile, line);
            
            //printf("got line...%s\n", line.c_str());
            
            if ( line[line.length() - 1] == 13 ) {
                line.erase(line.length() - 1, 1);
            }
            
            if ( line.substr(0, 3) == "!!!" ) {
                delete command;
                command = DncCommand::findCommand(line);
            } else {
                if (command->execute(line) != 0)
                    ErrorInStr(str_num);
            }
            str_num++;
        }

        delete command;

        WareSystem::finishInstance();
    } catch(const Error & err) {
        //cout << "Ошибка " << err.getErrorCode() << " " << err.getErrorUserText() << endl;
	//cout << err.getErrorLogText() << endl;
	string user_err_text = "Ошибка " + toString<int>(err.getErrorCode()) + " " + err.getErrorUserText();
	string log_err_text = user_err_text + " " + err.getErrorLogText();
	
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode(user_err_text.c_str())) );
	logfile.PrintString(log_type, ERROR, log_err_text.c_str());   
	
    }

    loadfile.close();
    
    setLoadedState(file_load_path, "uploaded  ");
    
    //cout << "loading succesfull complete"<< endl;
    //cout << "Ошибок - " << err_num << endl;

    userlog.SetMode(true);
    logfile.SetMode(true);	
    userlog.PrintString(user_log_type, INFO,WinCodec->fromUnicode(SystemCodec->toUnicode( "Загрузка товаров успешно завершена \r\n")) );
    logfile.PrintString(log_type, INFO, "Ware load successfully complete \n");
    userlog.SetMode(false);
    logfile.SetMode(false);		      
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Всего ошибок : %d\r\n")), err_num  );
    logfile.PrintString(log_type, INFO, "Errors : %d \n", err_num );    
    userlog.SetMode(true);	
    logfile.SetMode(true);
    
    return 0;
    
}

void DncLoad::setLoadedState(const char* file_path, const char* state) {
    FILE * upload_file = fopen(file_path, "rw+");
    if ( upload_file == NULL ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось перезаписать состояние загрузки файла")) );
	logfile.PrintString(log_type, ERROR, "can't open file path = <%s>\n", file_path);
	return;    
    }
    char str[500];
    fgets(str, 500, upload_file);
    fgets(str, 500, upload_file);

    strcpy(str, state);
    strcat(str, "\r\n");
    
    fwrite( str, sizeof(char), strlen(str), upload_file );
    fclose(upload_file);   
}


void DncLoad::ErrorFormat() {
    loadfile.close();
    //cout << "Файл не соответствует протоколу" << endl;
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Формат файла не соответствует протоколу \r\n")) );
    logfile.PrintString(log_type, INFO, "Incorrect file format \n");
}

void DncLoad::ErrorRead() {
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось открыть файл загрузки \r\n")) );
    logfile.PrintString(log_type, ERROR, "Cannot open data file \n");
    //cout << "Не удалось открыть файл" << endl;
}

void DncLoad::ErrorDoubleLoad() {
    loadfile.close();
    //cout << "Файл уже был обработан" << endl;
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Файл уже был обработан \r\n")) );
    logfile.PrintString(log_type, ERROR, " Seems like data file already processed earlier \n");   
}

void DncLoad::ErrorInStr(int str_num) {
    err_num++;
    //cout << "ошибка в строке - " << str_num << endl;
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка в строке - %d \r\n")), str_num + 6 );
}





