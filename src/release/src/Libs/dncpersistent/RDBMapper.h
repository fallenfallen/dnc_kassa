/* 
 * File:   RDBMapper.h
 * Author: shonda
 *
 * Created on 26 Май 2010 г., 17:51
 */

#ifndef _RDBMAPPER_H
#define	_RDBMAPPER_H

#include <string>

#include "PersistentMapper.h"
#include "ResultSet.h"
#include "ObjectFilter.h"
#include "stringconvert.h"
#include "ObjectsCollection.h"
#include "RDBInsertExpression.h"
#include "RDBUpdateExpression.h"

using namespace std;

class RDBMapper : public PersistentMapper{
protected:
    string to_db_table_name;
    string from_db_table_name;

    ObjectsCollection* getObjectsCollectionFromStorage(const ObjectFilter &filter);
    virtual ObjectsCollection* getObjectsCollectionFormResult(ResultSet& rs);

    
    void addNewObjectToStorage(Object * new_object);
    RDBInsertExpression getInsertExpression(Object * object);
    void setIdInsertedObject(Object * object, ResultSet& rs);

    void updateObjectToStorage(Object * object);
    RDBUpdateExpression getUpdateExpression(Object * object);

    void reloadObjectFromStorage(Object * object);    

    virtual Object * createObjectFromResultRecord(const ResultRecord & record) = 0;
    virtual void fromPersistent(Object* object, const ResultRecord & record) = 0;
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression) = 0;

    void dropObjects(const ObjectFilter &filter);
    
    double getMaximum(const MaximumFilter& filter);

    Object * createObjectByDefault() = 0;
    
private:
    ResultSet getDBResult(const ObjectFilter& filter);
    ResultSet insertToDB(const RDBInsertExpression & insert_exp);
    void updateToDB(const RDBUpdateExpression & update_exp);
};

#endif	/* _RDBMAPPER_H */

