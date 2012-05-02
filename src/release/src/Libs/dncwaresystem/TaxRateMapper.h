#ifndef TAXRATEMAPPER_H
#define	TAXRATEMAPPER_H

#include "RDBMapper.h"
#include "TaxRates.h"

#include <string>

using namespace std;

// field name in DB
const string TAXRATE_ID_FIELD = "tax_rate_id";
const string TAXRATE_SHORTNAME = "shortcut";
const string TAXRATE_LONGNAME = "longtitle";
const string TAXRATE_VALUE = "value";

class TaxRateMapper : public RDBMapper  {
public:
    TaxRateMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* TAXRATEMAPPER_H */

