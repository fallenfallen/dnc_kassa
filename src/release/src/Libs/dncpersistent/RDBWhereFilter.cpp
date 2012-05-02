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

#include "RDBWhereFilter.h"
#include "stringconvert.h"

void RDBWhereFilter::addWhereCondition(const string& field_name, const string& logical_operation, const string& value) {
    concatCondition(field_name, logical_operation, prepareVarchar(value));
}

void RDBWhereFilter::addWhereCondition(const string& field_name, const string& logical_operation, double value) {
    concatCondition(field_name, logical_operation, toString<double>(value));
}

void RDBWhereFilter::addWhereCondition(const string& field_name, const string& logical_operation, int value) {
    if ( value < 0 )
        concatCondition(field_name, " IS ", "NULL");
    else
        concatCondition(field_name, logical_operation, toString<int>(value));
}

void RDBWhereFilter::concatCondition(const string& field_name, const string& logical_operation, const string& value) {
    if (where_expression.empty())
        where_expression += " WHERE " +  field_name + logical_operation + value;
    else 
        where_expression += " AND " +  field_name + logical_operation + value;
}

string RDBWhereFilter::getWhereExpression() const {
    return where_expression;
}