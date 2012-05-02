/* 
 * File:   UserMapper.h
 * Author: shonda
 *
 * Created on 25 Апрель 2011 г., 16:04
 */

#ifndef USERMAPPER_H
#define	USERMAPPER_H

#include "RDBMapper.h"
#include "User.h"

#include <string>

using namespace std;

// field name in DB
const string USER_ID_FIELD = "user_id";
const string USER_LONGNAME = "user_name";
const string USER_SHORTNAME = "shortcut";
const string USER_PROFILE_ID = "profile_id";
const string USER_BARCODE = "bar_code";
const string USER_CARDCODE = "card_code";
const string USER_PASSWORD = "userpassword";


class UserMapper : public RDBMapper  {
public:
    UserMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* USERMAPPER_H */

