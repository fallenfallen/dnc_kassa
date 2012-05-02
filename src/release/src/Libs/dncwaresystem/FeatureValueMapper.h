/* 
 * File:   FeatureMapper.h
 * Author: shonda
 *
 * Created on 31 Август 2010 г., 11:41
 */

#ifndef FEATUREVALUEMAPPER_H
#define	FEATUREVALUEMAPPER_H

#include "RDBMapper.h"

using namespace std;

const string FVALUE_ID_FIELD = "value_id";
const string FVALUE_MEANING = "value_value";
const string FVALUE_CODE = "code";
const string FVALUE_FEATURE_ID = "feature_id";

class FeatureValueMapper : public RDBMapper{
public:
    FeatureValueMapper();
    string getIdFieldName();
    Object * createObjectByDefault();
private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};


const string SFVALUE_ID_FIELD = "features_value_id";
const string SFVALUE_FEATURE_ID = "feature_id";
const string SFVALUE_ADDITION_ID = "addition_id";
const string SFVALUE_VALUE_ID = "value_id";
const string SFVALUE_VALUE = "value_value";
const string SFVALUE_VALUE_CODE = "code";

class SelectedValueMapper : public RDBMapper {
public:
    SelectedValueMapper();
    string getIdFieldName();
    Object * createObjectByDefault();
private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* FEATUREVALUEMAPPER_H */

