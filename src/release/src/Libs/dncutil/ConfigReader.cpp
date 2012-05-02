/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include "ConfigReader.h"
#include <iostream>
#include <fstream>
#include "Error.h"

void ConfigReader::loadFromFile(const char* path) {
    string line;
    ifstream loadfile(path);
    
    if (!loadfile.is_open())
        throw Error(215, "Could't open config file");
    
    parameters.clear();

    int pos, pos_end;

    while ( !loadfile.eof() ) {
	getline (loadfile, line);

        pos = line.find("[");
        if (pos == string::npos) continue;

        pos_end = line.find("]", pos + 1);
        if (pos_end == string::npos) continue;

        parameters[line.substr(pos + 1, pos_end - pos - 1)] = line.substr(pos_end + 1);
    }

    loadfile.close();

    /*for (map<string, string>::iterator it = parameters.begin(); it != parameters.end(); it++) {
        cout << "param_name = " <<it->first << " value = " << it->second << endl;
    }*/

}

void ConfigReader::saveToFile(const char* path) {

    ofstream savefile(path);
    if (!savefile.is_open()) 
        throw Error(215, "Could't open config file");

    for (map<string, string>::iterator it = parameters.begin(); it != parameters.end(); it++ ) {
        savefile << "[" << it->first << "] " << it->second << endl;
    }

    savefile.close();

}

string ConfigReader::getParamValue(const char* param_name) {
     return parameters[param_name];
}

void ConfigReader::setParamValue(const char* param_name, const char* param_value) {
    parameters[param_name] = param_value;
}
