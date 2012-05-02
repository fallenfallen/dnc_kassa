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

#include "RDBSetExpression.h"
#include "stringconvert.h"
#include "addon_func.h"

void RDBSetExpression::addField(const string& field_name, const string& value) {
    concatExpression(field_name, prepareVarchar(value));
}

void RDBSetExpression::addField(const string& field_name, double value) {
    //printf("void RDBSetExpression::addField(%2f)\n", value);
    //printf("toString(%2f) = %s\n", value, toString<double>(value).c_str());
    concatExpression(field_name, toString<double>(value));
        
}

void RDBSetExpression::addField(const string& field_name, int value) {
    if ( value < 0 )
        concatExpression(field_name, "null");
    else
        concatExpression(field_name, toString<int>(value));
}
