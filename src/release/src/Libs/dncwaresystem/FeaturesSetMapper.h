/* 
 * File:   FeaturesSetMapper.h
 * Author: shonda
 *
 * Created on 24 Август 2010 г., 14:58
 */

#ifndef FEATURESSETMAPPER_H
#define	FEATURESSETMAPPER_H

#include "RDBMapper.h"

using namespace std;

const string FSET_ID_FIELD = "set_id";
const string FSET_NAME = "set_name";
const string FSET_CODE = "code";


class FeaturesSetMapper : public RDBMapper{
public:
    FeaturesSetMapper();
    string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* FEATURESSETMAPPER_H */

