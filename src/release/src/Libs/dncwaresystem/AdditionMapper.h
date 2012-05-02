/* 
 * File:   AdditionMapper.h
 * Author: shonda
 *
 * Created on 19 Август 2010 г., 17:22
 */

#ifndef ADDITIONMAPPER_H
#define	ADDITIONMAPPER_H

#include "RDBMapper.h"

using namespace std;

const string ADDITION_ID_FIELD = "addition_id";
const string ADDITION_WARE_ID = "ware_id";
const string ADDITION_CODE = "addition_code";
const string ADDITION_ARTICLE = "addition_article";
const string ADDITION_PRICE = "addition_price";
const string ADDITION_QUANTITY = "addition_quantity";


class AdditionMapper : public RDBMapper {
public:
    AdditionMapper();
    string getIdFieldName();
    string getCodeFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* ADDITIONMAPPER_H */

