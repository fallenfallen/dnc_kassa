/* 
 * File:   DbManager.h
 * Author: shonda
 *
 * Created on 20 Январь 2010 г., 15:30
 */

#ifndef _DBMANAGER_H
#define	_DBMANAGER_H

#include <string>
#include <libpq-fe.h>
#include "ResultSet.h"

using namespace std;


class DbManager {
public:
    DbManager();
    DbManager(const DbManager& orig);
    virtual ~DbManager();

    DbManager(const char* conf_path);
    void setConnectionParameters(
                            const char* db, const char* us,
                            const char* pass, const char* host, 
                            const char* host_addr, const char* port,
                            const char* log_path );

    int connect();
    int disconnect();

    int doSQL( const char* command, ResultSet & rs );

    
private:
    string dbname;
    string user;
    string password;
    string hostname;
    string hostaddr;
    string dbport;
    
    string log;

    PGconn * myconnection;
};

#endif	/* _DBMANAGER_H */

