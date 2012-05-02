/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include <iostream>
#include "FixedDiscountMapper.h"
#include "SectionMapper.h"
#include "RDBOperation.h"


FixedDiscountMapper::FixedDiscountMapper() {
    to_db_table_name = "fix_reduction";
    from_db_table_name = "fix_reduction";
}

string FixedDiscountMapper::getIdFieldName() {
    return FDISCOUNT_ID_FIELD;
}

string FixedDiscountMapper::getCodeFieldName() {
    return FDISCOUNT_CODE;
}

Object * FixedDiscountMapper::createObjectByDefault() {
    FixedDiscount * fixed_discount = new FixedDiscount();
    return fixed_discount;
}

Object * FixedDiscountMapper::createObjectFromResultRecord(const ResultRecord& record) {
    FixedDiscount * fixed_discount = new FixedDiscount();
    fromPersistent(fixed_discount, record);
    return fixed_discount;
}

void FixedDiscountMapper::fromPersistent(Object* object, const ResultRecord& record) {
    FixedDiscount* fdiscount = (FixedDiscount*)object;
    FixedDiscountInfo fdiscount_info;

    fdiscount_info.code = record.getValue(FDISCOUNT_CODE);
    fdiscount_info.title = record.getValue(FDISCOUNT_TITLE);
    fdiscount_info.text = record.getValue(FDISCOUNT_TEXT);
    fdiscount_info.barcode = record.getValue(FDISCOUNT_BARCODE);
    fdiscount_info.card_code = record.getValue(FDISCOUNT_CARDCODE);
    fdiscount_info.visible = record.getValue(FDISCOUNT_VISIBLE) == "t" ? true : false;
    fdiscount_info.rate = fromString<double>(record.getValue(FDISCOUNT_RATE));
    fdiscount_info.range = record.getValue(FDISCOUNT_AREA) == "t" ? CHECK_DISCOUNT : POSITION_DISCOUNT;
    fdiscount_info.type = (FixedDiscountType)fromString<int>(record.getValue(FDISCOUNT_TYPE));
    //-----(+)-----Zuskin-----19/07/2011-----
    fdiscount_info.scheme_id = fromString<int>(record.getValue(FDISCOUNT_SCHEMEID));
    fdiscount_info.summ = fromString<double>(record.getValue(FDISCOUNT_SUMM));
    fdiscount_info.maxbonuspercent = fromString<double>(record.getValue(FDISCOUNT_MAXPERCENT));
    //---------------------------------------
    
    fdiscount->setInfo(fdiscount_info);

}

void FixedDiscountMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    FixedDiscount * fdiscount = (FixedDiscount*)object;

    FixedDiscountInfo info = fdiscount->getInfo();
    
    rdb_expression.addField(FDISCOUNT_CODE, info.code);
    rdb_expression.addField(FDISCOUNT_TITLE, info.title);
    rdb_expression.addField(FDISCOUNT_TEXT, info.text);
    rdb_expression.addField(FDISCOUNT_BARCODE, info.barcode);
    rdb_expression.addField(FDISCOUNT_CARDCODE, info.card_code);
    rdb_expression.addField(FDISCOUNT_RATE, info.rate);
    rdb_expression.addField(FDISCOUNT_VISIBLE, info.visible ? "t" : "f");
    
    if ( info.range == CHECK_DISCOUNT )
        rdb_expression.addField(FDISCOUNT_AREA, "t");
    else 
        rdb_expression.addField(FDISCOUNT_AREA, "f");
    
    rdb_expression.addField(FDISCOUNT_TYPE, info.type);

    //-----(*)-----Zuskin-----19/07/2011-----
    // Заменить потом на WareSystem::getInstance()->getSystemPreferenceActiveSchemeId()
    //ResultSet rs = RDBOperation::getInstance()->getFromTable("activescheme", EmptyObjectFilter("activescheme"));
    //int active_scheme_id = fromString<int>(rs.getRecord(0).getValue("activescheme_id"));
    //

    //rdb_expression.addField(FDISCOUNT_SCHEMEID, active_scheme_id);
    rdb_expression.addField(FDISCOUNT_SCHEMEID, info.scheme_id);
    rdb_expression.addField(FDISCOUNT_SUMM, info.summ);
    rdb_expression.addField(FDISCOUNT_MAXPERCENT, info.maxbonuspercent);
    
    //----------------------------------------
}

