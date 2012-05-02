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


#include "RDBInsertExpression.h"
#include "stringconvert.h"

void RDBInsertExpression::concatExpression(const string& field_name, const string& value) {
    if (!fields_expression.empty())
        fields_expression += ", ";
    fields_expression += field_name;

    if (!values_expression.empty())
        values_expression += ", ";
    values_expression += (value.empty() ? "null" : value);
    
}

void RDBInsertExpression::addReturningField(const string& field_name) {
    if ( !returning_expression.empty() )
        returning_expression += ", ";
    else
        returning_expression += "RETURNING ";
    returning_expression += field_name;
}

string RDBInsertExpression::getFieldExpression() const {
    return fields_expression;
}

string RDBInsertExpression::getValueExpression() const {
    return values_expression;
}

string RDBInsertExpression::getReturningExpression() const {
    return returning_expression;
}
