#ifndef MANAGERMAPPER_H
#define	MANAGERMAPPER_H

#include "RDBMapper.h"
#include "Manager.h"

#include <string>

using namespace std;

// field name in DB
const string MANAGER_ID_FIELD = "manager_id";
const string MANAGER_SHORTNAME = "manager_name";
const string MANAGER_LONGNAME = "name_to_print";
const string MANAGER_BARCODE = "barcode";
const string MANAGER_CARDCODE = "cardcode";

class ManagerMapper : public RDBMapper  {
public:
    ManagerMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* MANAGERMAPPER_H */

