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

#include "RDBUpdateExpression.h"
#include "stringconvert.h"


void RDBUpdateExpression::concatExpression(const string& field_name, const string& value) {
    
    if ( !setting_expression.empty() )
        setting_expression += ", ";
    setting_expression += field_name + " = ";

    setting_expression += (value.empty() ? "null" : value);
    
}

string RDBUpdateExpression::getSettingExpression() const {
    return setting_expression;
}

string RDBUpdateExpression::getWhereExpression() const {
    return where_filter.getWhereExpression();
}
