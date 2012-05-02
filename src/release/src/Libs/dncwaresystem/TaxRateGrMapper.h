#ifndef TAXRATEGRMAPPER_H
#define	TAXRATEGRMAPPER_H

#include "RDBMapper.h"
#include "TaxRateGr.h"

#include <string>

using namespace std;

// field name in DB
const string TAXRATEGR_ID_FIELD = "id";
const string TAXRATEGR_RATE_ID = "tax_rate_id";
const string TAXRATEGR_GROUP_ID = "tax_group_id";
const string TAXRATEGR_BASE = "base_flag";

class TaxRateGrMapper : public RDBMapper  {
public:
    TaxRateGrMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* TAXRATEGRMAPPER_H */

