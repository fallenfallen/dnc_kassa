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

#include "RDBOperation.h"
#include "stringconvert.h"
#include "Error.h"
#include <iostream>

const char* LINCASH_CONFPATH = "/etc/dancy/LinCash_db.conf";

RDBOperation * RDBOperation::instance = NULL;
int RDBOperation::ref_count = 0;

RDBOperation::RDBOperation() {
    dbm = new DbManager(LINCASH_CONFPATH);
    dbm->connect();
}

RDBOperation::~RDBOperation() {
    dbm->disconnect();
    delete dbm;
}

RDBOperation * RDBOperation::getInstance() {
    if ( instance == NULL )
        throw Error(213, "Сервис работы с базой данных не запущен", "RDBOperation is NULL");
    return instance;
}

void RDBOperation::startInstance() {
    if ( instance == NULL )
        instance = new RDBOperation();
    ref_count++;
}

void RDBOperation::finishInstance() {
    ref_count--;
    if ( ref_count == 0 ) {
        delete instance;
        instance = NULL;
    }
}

ResultSet RDBOperation::getFromTable(const string& table_name, const ObjectFilter &filter) {
    ResultSet rs;
    string str = "SELECT * FROM " + table_name + filter.getFilterExpression();
    dbm->doSQL(str.c_str(), rs);
    return rs;
}

ResultSet RDBOperation::insertToDB(const string& table_name, const RDBInsertExpression& insert_exp) {
    string sql_request;

    sql_request += "INSERT INTO " + table_name + " (" + insert_exp.getFieldExpression() + ") ";
    sql_request += "VALUES (" + insert_exp.getValueExpression() + ") ";
    sql_request += insert_exp.getReturningExpression();

    ResultSet rs;
    dbm->doSQL(sql_request.c_str(), rs);

    return rs;
}

void RDBOperation::updateToDB(const string& table_name, const RDBUpdateExpression& update_exp){
    string sql_request;
    sql_request += "UPDATE " + table_name + " SET " + update_exp.getSettingExpression();
    sql_request += update_exp.getWhereExpression();

    ResultSet rs;
    dbm->doSQL(sql_request.c_str(), rs);
//    printf("%s\n", sql_request.c_str());
}

void RDBOperation::dropFromTable(const string& table_name, const ObjectFilter& filter) {
    string sql_request;
    sql_request += "DELETE FROM " + table_name + filter.getFilterExpression();

    ResultSet rs;
    dbm->doSQL(sql_request.c_str(), rs);
}

ResultSet RDBOperation::getMaximum(const string& table_name, const MaximumFilter& filter) {
    string sql_request;
    sql_request += "SELECT max("  +  filter.getMaxFieldName()  +  ") FROM " + table_name + filter.getFilterExpression();
    
    ResultSet rs;
    dbm->doSQL(sql_request.c_str(), rs);
    return rs;
}


