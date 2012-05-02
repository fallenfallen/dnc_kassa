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

#include "ResultSet.h"
#include <iostream>

ResultSet::ResultSet() {
    records = NULL;
}

ResultSet::ResultSet(PGresult * result) {
    records = result;
}

ResultSet::~ResultSet() {
    clear();
}

void ResultSet::setResult(PGresult* pg_result) {
    if ( records != NULL )
        clear();
    records = pg_result;
}

void ResultSet::clear() {
    if (records != NULL)
        PQclear(records);
    records = NULL;
}


int ResultSet::getCountRecord() const {
    if ( records == NULL )
        return -1;
    return PQntuples(records);
}

string ResultSet::getValue(int i, int j) const {
    if ( records == NULL )
        return string();
    if ( j >= PQnfields(records)  )
        return string();
    if ( i >= PQntuples(records) )
        return string();
    return string(PQgetvalue(records, i, j));
}

string ResultSet::getValue(int i, const string &field_name) const {
    if (records == NULL)
        return string();
    if ( i >= PQntuples(records) )
        return string();
    int f = PQfnumber(records, field_name.c_str());
    if ( f == - 1 )
        return string();
    return string(PQgetvalue(records, i, f));
}

ResultRecord ResultSet::getRecord(int i) {
    ResultRecord result_record(i, this);
    return result_record;
}


/********************************************************************/

ResultRecord::ResultRecord(int num, ResultSet* rs) {
    record_num = num;
    result_set = rs;
}

string ResultRecord::getValue(int j) const {
    return result_set->getValue(record_num, j);
}

string ResultRecord::getValue(const string& field_name) const {
    return result_set->getValue(record_num, field_name);
}
