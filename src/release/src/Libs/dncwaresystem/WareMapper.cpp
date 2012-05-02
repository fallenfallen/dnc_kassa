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

#include "WareMapper.h"

WareMapper::WareMapper() {
    to_db_table_name = "ware";
    from_db_table_name = "ware";
    //table_name = "ware";
}

string WareMapper::getIdFieldName() {
    return WARE_ID_FIELD;
}

string WareMapper::getCodeFieldName() {
    return WARE_INTERNAL_CODE;
}

Object * WareMapper::createObjectByDefault() {
    Ware * ware = new Ware();
    return ware;
}

Object* WareMapper::createObjectFromResultRecord(const ResultRecord& record) {
    Ware * ware = new Ware();
    fromPersistent(ware, record);
    return ware;
}

void WareMapper::fromPersistent(Object* object, const ResultRecord& record) {
    Ware* ware = (Ware*)object;

    WareInfo ware_info;

    ware_info.internal_code = record.getValue( WARE_INTERNAL_CODE);
    ware_info.title = record.getValue(WARE_TITLE);
    ware_info.text = record.getValue(WARE_TEXT);
    ware_info.article = record.getValue(WARE_ARTICLE);
    ware_info.price = fromString<double>(record.getValue(WARE_PRICE));
    ware_info.quantity = fromString<double>(record.getValue(WARE_ALL_QUANTITY));
    int flags = fromString<int>(record.getValue(WARE_FLAGS));
    ware_info.flags.setFlags(flags);
    ware_info.min_price = fromString<double>(record.getValue(WARE_MIN_PRICE));
    ware_info.expiration = record.getValue(WARE_BEST_BEFORE);

    ware_info.section_num = fromString<int>(record.getValue(WARE_SECTION));
    //---(*)---Zuskin---20/02/2012---
    //ware_info.sale_limit_by_time = fromString<int>(record.getValue(WARE_SECTION)) == 1;
    ware_info.sale_limit_by_time = fromString<int>(record.getValue(WARE_SALE_LIMIT_BY_TIME));
    //-------------------------------

    ware_info.features_set_id = fromString<int>(record.getValue(WARE_FSET_ID));
    ware_info.group_id = fromString<int>(record.getValue(WARE_GROUP_ID));
    ware_info.taxgr_id = fromString<int>(record.getValue(WARE_TAXGR_ID));

    ware->replaceInfo(ware_info);
}

void WareMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    Ware* ware = (Ware*)object;
    WareInfo ware_info = ware->getInfo();

    rdb_expression.addField(WARE_INTERNAL_CODE, ware_info.internal_code);
    rdb_expression.addField(WARE_TITLE, ware_info.title);
    rdb_expression.addField(WARE_TEXT, ware_info.text);
    rdb_expression.addField(WARE_ARTICLE, ware_info.article);
    rdb_expression.addField(WARE_PRICE, ware_info.price);
    rdb_expression.addField(WARE_ALL_QUANTITY, ware->getAllRestQuantity());
    rdb_expression.addField(WARE_FLAGS, ware_info.flags.toInt());
    rdb_expression.addField(WARE_MIN_PRICE, ware_info.min_price);
    rdb_expression.addField(WARE_BEST_BEFORE, ware_info.expiration);
    rdb_expression.addField(WARE_SECTION, ware_info.section_num);
    //---(*)---Zuskin---20/02/2012---
    //rdb_expression.addField(WARE_SALE_LIMIT_BY_TIME, ware_info.sale_limit_by_time ? 1 : 0);
    rdb_expression.addField(WARE_SALE_LIMIT_BY_TIME, ware_info.sale_limit_by_time);
    //-------------------------------
    
    rdb_expression.addField(WARE_FSET_ID, ware_info.features_set_id);
    rdb_expression.addField(WARE_GROUP_ID, ware_info.group_id);
    rdb_expression.addField(WARE_TAXGR_ID, ware->getTaxGroupId());
}

