/* 
 * File:   SectionMapper.h
 * Author: shonda
 *
 * Created on 1 Февраль 2011 г., 16:19
 */

#ifndef SECTIONMAPPER_H
#define	SECTIONMAPPER_H

#include "RDBMapper.h"
#include "Section.h"

#include <string>

using namespace std;

// field name in DB
const string SECTION_ID_FIELD = "section_id";
const string SECTION_NUMBER = "section_number";
const string SECTION_NAME = "section_name";
const string SECTION_HEAD_TEXT = "head_text";
const string SECTION_BOTTOM_TEXT = "bottom_text";
const string SECTION_IS_FISCAL = "fiscal_section";
const string SECTION_CASH = "section_cash";
const string SECTION_AUTO_ENCASHMENT = "auto_encashment";

class SectionMapper : public RDBMapper {
public:
    SectionMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* SECTIONMAPPER_H */

