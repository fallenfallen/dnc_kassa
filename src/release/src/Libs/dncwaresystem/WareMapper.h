/* 
 * File:   WareMapper.h
 * Author: shonda
 *
 * Created on 12 Июль 2010 г., 13:50
 */

#ifndef WAREMAPPER_H
#define	WAREMAPPER_H

#include "RDBMapper.h"
#include "FeaturesSet.h"
#include "Ware.h"

#include <string>

using namespace std;

// field name in DB
const string WARE_ID_FIELD = "ware_id";
const string WARE_INTERNAL_CODE = "internal_id";
const string WARE_TITLE = "longtitle";
const string WARE_TEXT = "shortcut";
const string WARE_ARTICLE = "article";
const string WARE_PRICE = "price";
const string WARE_ALL_QUANTITY = "quantity";
const string WARE_FLAGS = "flags";
const string WARE_MIN_PRICE = "min_price";
const string WARE_BEST_BEFORE = "best_before";
const string WARE_GROUP_ID = "group_id";
const string WARE_TAXGR_ID = "tax_group_id";
const string WARE_FSET_ID = "features_set_id";
const string WARE_SECTION = "section";
const string WARE_SALE_LIMIT_BY_TIME = "sale_limit_by_time";


class WareMapper : public RDBMapper{
public:
    WareMapper();
    virtual string getIdFieldName();
    virtual string getCodeFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* WAREMAPPER_H */

