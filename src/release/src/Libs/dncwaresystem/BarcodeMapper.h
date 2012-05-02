/* 
 * File:   BarcodeMapper.h
 * Author: shonda
 *
 * Created on 16 Сентябрь 2010 г., 17:58
 */

#ifndef BARCODEMAPPER_H
#define	BARCODEMAPPER_H

#include "RDBMapper.h"
#include "Barcode.h"

using namespace std;

const string BARCODE_ID_FIELD = "wb_id";
const string BARCODE_ADDITION_CODE = "addition_code";
const string BARCODE_WARE_CODE = "internal_id";
const string BARCODE_TEXT = "barcode";
const string BARCODE_COEFFICIENT = "coefficient";
const string BARCODE_MAIN_BARCODE = "main_barcode";


class BarcodeMapper : public RDBMapper {
public:
    BarcodeMapper();
    string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* BARCODEMAPPER_H */

