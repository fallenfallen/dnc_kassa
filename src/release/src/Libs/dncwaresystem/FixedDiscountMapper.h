/* 
 * File:   FixedDiscountMapper.h
 * Author: shonda
 *
 * Created on 4 Март 2011 г., 15:30
 */

#ifndef FIXEDDISCOUNTMAPPER_H
#define	FIXEDDISCOUNTMAPPER_H

#include "RDBMapper.h"
#include "FixedDiscount.h"

#include <string>

using namespace std;

// field name in DB
const string FDISCOUNT_ID_FIELD = "fix_reduction_id";
const string FDISCOUNT_CODE = "external_id";
const string FDISCOUNT_TITLE = "longtitle";
const string FDISCOUNT_TEXT = "shortcut";
const string FDISCOUNT_RATE = "value";
const string FDISCOUNT_TYPE = "type";
const string FDISCOUNT_AREA = "area";
const string FDISCOUNT_BARCODE = "barcode";
const string FDISCOUNT_CARDCODE = "cardcode";
const string FDISCOUNT_VISIBLE = "show";
const string FDISCOUNT_SCHEMEID = "scheme_id";
//-----(+)-----Zuskin-----19/07/2011-----
const string FDISCOUNT_SUMM = "summ";
const string FDISCOUNT_MAXPERCENT = "maxbonuspercent";
//---------------------------------------

class FixedDiscountMapper : public RDBMapper {
public:
    FixedDiscountMapper();
    virtual string getIdFieldName();
    virtual string getCodeFieldName();
    Object * createObjectByDefault();
    
private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* FIXEDDISCOUNTMAPPER_H */

