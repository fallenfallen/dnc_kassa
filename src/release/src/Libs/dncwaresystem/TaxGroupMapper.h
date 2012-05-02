#ifndef TAXGROUPMAPPER_H
#define	TAXGROUPMAPPER_H

#include "RDBMapper.h"
#include "TaxGroup.h"

#include <string>

using namespace std;

// field name in DB
const string TAXGROUP_ID_FIELD = "tax_group_id";
const string TAXGROUP_SHORTNAME = "shortcut";
const string TAXGROUP_LONGNAME = "longtitle";

class TaxGroupMapper : public RDBMapper  {
public:
    TaxGroupMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* TAXGROUPMAPPER_H */

