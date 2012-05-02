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


#include "Error.h"
#include <iostream>

Error::Error(const string& err_text, ErrorType err_type) {
    log_text = err_text;
    user_text = err_text;
    type = err_type;
    //cout << err_type << " " << err_text << endl;
}

Error::Error(int err_code, const string& err_user_text, const string& err_log_text, ErrorType err_type) {
    code = err_code;
    log_text = err_log_text;
    user_text = err_user_text;
    type = err_type;
    /*cout << "Ошибка № " << err_code << " " << type << endl;
    cout << user_text << endl;
    cout << log_text << endl;*/
}

Error::Error(int err_code, const string& err_text, ErrorType err_type) {
    code = err_code;
    log_text = err_text;
    user_text = err_text;
    type = err_type;
    /*cout << "Ошибка № " << err_code << " " << type << endl;
    cout << log_text << endl;*/
}

string Error::getErrorUserText() const {
    return user_text;
}

string Error::getErrorLogText() const {
    return log_text;
}

ErrorType Error::getErrorType() const{
    return type;
}

int Error::getErrorCode() const{
    return code;
}


