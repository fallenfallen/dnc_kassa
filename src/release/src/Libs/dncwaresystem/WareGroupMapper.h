/* 
 * File:   WareGroupMapper.h
 * Author: shonda
 *
 * Created on 11 Январь 2011 г., 16:50
 */

#ifndef WAREGROUPMAPPER_H
#define	WAREGROUPMAPPER_H

#include "RDBMapper.h"
#include "WareGroup.h"

#include <string>

using namespace std;

// field name in DB
const string GROUP_ID_FIELD = "group_id";
const string GROUP_CODE = "group_code";
const string GROUP_TITLE = "longtitle";
const string GROUP_TEXT = "shortcut";
const string GROUP_PARENT_ID = "parent_group_id";

class WareGroupMapper : public RDBMapper {
public:
    WareGroupMapper();
    virtual string getIdFieldName();
    virtual string getCodeFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};






#endif	/* WAREGROUPMAPPER_H */

