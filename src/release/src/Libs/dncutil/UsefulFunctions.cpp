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

#include <iostream>
#include <math.h>
#include "UsefulFunctions.h"

vector<string> parseStringWithSeparator(const string& orig_string, const string& separator) {
    vector<string> results;

    string param;
    int pos_start = 0;
    int pos_separator = 0;

    pos_separator = orig_string.find(separator, pos_start);
    
    while (pos_separator != -1) {
	param = orig_string.substr(pos_start, pos_separator - pos_start);
	//cout << "param = " << param << endl;
	results.push_back(param);
	pos_start = pos_separator + 1;
	pos_separator = orig_string.find(separator, pos_start);
    }

    int len = orig_string.length();
    if ( pos_start < len ) {
	param = orig_string.substr(pos_start, len - pos_start);
	//cout << " last param = " << param << endl;
	results.push_back(param);
    }
    
    return results;
}

int convertStringOfZeros(string str) {
    if (str.length() == 0) 
	return -1;
    vector<string> vstr = parseStringWithSeparator(str, ",");
    
    int result = 0;
    for (int i = 0; i < vstr.size(); i++) {
	if (vstr[i] == "1") {
	    result += (int) pow(2, i);
	}
    }
        
    return result;	

}

string convertDate(string str) {
    if (str.length() == 0) 
	return "";
    vector<string> vstr = parseStringWithSeparator(str, ".");
    
    if ( vstr.size() != 3 ) 
	return "";
	
    return vstr[2] + "-" + vstr[1] + "-" + vstr[0];	

}