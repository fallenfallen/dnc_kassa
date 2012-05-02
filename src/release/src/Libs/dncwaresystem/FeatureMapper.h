/* 
 * File:   FeatureMapper.h
 * Author: shonda
 *
 * Created on 31 Август 2010 г., 11:41
 */

#ifndef FEATUREMAPPER_H
#define	FEATUREMAPPER_H

#include "RDBMapper.h"

using namespace std;

const string FEATURE_ID_FIELD = "feature_id";
const string FEATURE_NAME = "feature_name";
const string FEATURE_SET_ID = "set_id";
const string FEATURE_CODE = "code";

class FeatureMapper : public RDBMapper{
public:
    FeatureMapper();
    string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);

};



const string FIS_ID_FIELD = "fis_id";
const string FIS_FEATURE_ID = "feature_id";
const string FIS_SET_ID = "set_id";

class FeatureTieWithSetMapper : public  RDBMapper {
public:
    FeatureTieWithSetMapper();
    string getIdFieldName();
    Object * createObjectByDefault();
    
private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);

};

#endif	/* FEATUREMAPPER_H */

