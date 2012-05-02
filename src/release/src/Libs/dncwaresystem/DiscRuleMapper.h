#ifndef DISCRULEMAPPER_H
#define	DISCRULEMAPPER_H

#include "RDBMapper.h"
#include "DiscRule.h"

using namespace std;

const string DISCRULE_ID_FIELD = "auto_reduction_id";
const string DISCRULE_CODE = "auto_reduction_id";
const string DISCRULE_SCHEME_ID = "auto_scheme_id";
const string DISCRULE_LONGTITLE = "longtitle";
const string DISCRULE_SHORTCUT = "shortcut";
const string DISCRULE_VALUE = "value";
const string DISCRULE_TYPE = "type";
const string DISCRULE_BDATE = "begindate";
const string DISCRULE_EDATE = "enddate";
const string DISCRULE_BTIME = "begintime";
const string DISCRULE_ETIME = "endtime";
const string DISCRULE_WDAYS = "weekdays";
const string DISCRULE_SUMCHECK = "check_sum";
const string DISCRULE_ACCUMULATION = "accumulation";
//----------not in use yet ------------
const string DISCRULE_QTTY = "quantity";
const string DISCRULE_POSSUM = "position_sum";

class DiscRuleMapper : public RDBMapper {
public:
    DiscRuleMapper();
    string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* DISCRULEMAPPER_H */

