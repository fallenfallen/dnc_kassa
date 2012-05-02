/* 
 * File:   RDBOperation.h
 * Author: shonda
 *
 * Created on 26 Май 2010 г., 19:09
 */

#ifndef _RDBOPERATION_H
#define	_RDBOPERATION_H

#include <string>

#include "ResultSet.h"
#include "DbManager.h"
#include "ObjectFilter.h"
#include "RDBInsertExpression.h"
#include "RDBUpdateExpression.h"

using namespace std;

class RDBOperation {

protected:
    static int ref_count;

    RDBOperation();
    ~RDBOperation();

public:
    static RDBOperation * getInstance();
    static void startInstance();
    static void finishInstance();

    ResultSet getFromTable( const string & table_name,  const ObjectFilter &filter );
    ResultSet insertToDB(const string & table_name, const RDBInsertExpression & insert_exp);
    void updateToDB(const string & table_name, const RDBUpdateExpression & update_exp);

    void dropFromTable(const string & table_name, const ObjectFilter & filter);

    ResultSet getMaximum(const string & table_name, const MaximumFilter & filter);
    
private:
    static RDBOperation * instance;
    
    DbManager* dbm;
};

#endif	/* _RDBOPERATION_H */

