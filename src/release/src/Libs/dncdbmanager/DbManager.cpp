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

#include "DbManager.h"
#include "ConfigReader.h"
#include "Error.h"
#include <iostream>

DbManager::DbManager() {
    myconnection = NULL;
}

DbManager::DbManager(const DbManager& orig) {
}

DbManager::~DbManager() {
}

DbManager::DbManager(const char* conf_path) {

    myconnection = NULL;

    ConfigReader rc;
    rc.loadFromFile(conf_path);
    
    dbname = rc.getParamValue("DB");
    user = rc.getParamValue("USER");
    password = rc.getParamValue("PASSWORD");
    hostname = rc.getParamValue("HOST");
    hostaddr = rc.getParamValue("HOSTADDR");
    dbport = rc.getParamValue("PORT");
    log = rc.getParamValue("LOG");

    myconnection = NULL;

}

void DbManager::setConnectionParameters(
                            const char* db, const char* us,
                            const char* pass, const char* host, 
                            const char* host_addr, const char* port,
                            const char* log_path ) {
    dbname = db;
    user = us;
    password = pass;
    hostname = host;
    hostaddr = host_addr;
    dbport = port;
    log = log_path;
}

int DbManager::connect() {

    if ( myconnection != NULL ) if ( PQstatus(myconnection) == CONNECTION_OK )
        return CONNECTION_OK;

    if ( dbname.empty() ) 
        throw Error(201, "Ошибка соединения с базой", "Не задано имя базы", MSG_FATAL);

    string con_info;
    con_info += "dbname = " + dbname;
    if ( ! user.empty() )  con_info += " user = " + user;
    if ( ! password.empty() ) con_info += " password = " + password;
    if ( ! hostname.empty() ) con_info += " host = " + hostname;
    if ( ! hostaddr.empty() ) con_info += " hostaddr = " + hostaddr;
    if ( ! dbport.empty() ) con_info += " port = " + dbport;

    
    myconnection = PQconnectdb(con_info.c_str());

    if ( PQstatus(myconnection) != CONNECTION_OK ) 
        throw Error(202, "Ошибка при соединении с базой", string(PQerrorMessage(myconnection)), MSG_FATAL);
    
    return CONNECTION_OK;
    
}


int DbManager::disconnect() {
    PQfinish(myconnection);
}


int DbManager::doSQL(const char* command, ResultSet & rs ) {

     if ( myconnection == NULL ) 
         throw Error(203, "Ошибка при выполнении запроса к бд", "Отсутствует соединение с базой");
     if ( PQstatus(myconnection) != CONNECTION_OK ) 
         throw Error(204, "Отсутствует соединение с базой");

     PGresult * result;

     result = PQexec(myconnection, command);
     
     if ( PQresultStatus(result) == PGRES_BAD_RESPONSE ) {
         throw Error(205, "Ошибка при обращении к серверу базы данных \nОтвет сервера непонятен");
     }

     if ( PQresultStatus(result) == PGRES_NONFATAL_ERROR ) {
         throw Error(206, "Нефатальная ошибка, можно попробовать еще раз");
     }

     if ( PQresultStatus(result) == PGRES_FATAL_ERROR ) 
        throw Error(207, "Ошибка запроса к базе данных\n", string(PQresultErrorMessage(result)));

     if ( PQresultStatus(result) == PGRES_TUPLES_OK  )  rs.setResult(result);

     return PQresultStatus(result);
}

