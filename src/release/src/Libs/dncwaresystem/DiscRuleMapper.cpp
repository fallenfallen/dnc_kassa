/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Алекасндр aka Zuskin
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include <iostream>

#include "DiscRuleMapper.h"

DiscRuleMapper::DiscRuleMapper() {
    //table_name = "ware_barcode";
    from_db_table_name = "auto_reduction";
    to_db_table_name = "auto_reduction";
}

string DiscRuleMapper::getIdFieldName() {
    return DISCRULE_ID_FIELD;
}

Object * DiscRuleMapper::createObjectByDefault() {
    DiscRule * dsc = new DiscRule();
    return dsc;
}

Object * DiscRuleMapper::createObjectFromResultRecord(const ResultRecord& record) {
    DiscRule * dsc = new DiscRule();
    fromPersistent(dsc, record);
    return dsc;
}

void DiscRuleMapper::fromPersistent(Object* object, const ResultRecord& record) {
//printf("void DiscRuleMapper::fromPersistent(Object* object, const ResultRecord& record) {\n");
    DiscRule * dsc = (DiscRule*) object;
    dsc->setCode(fromString<int>(record.getValue(DISCRULE_CODE)));
    dsc->setSchemeId(fromString<int>(record.getValue(DISCRULE_SCHEME_ID)));
    dsc->setLongTitle(record.getValue(DISCRULE_LONGTITLE));
    dsc->setShortcut(record.getValue(DISCRULE_SHORTCUT));
    dsc->setValue(fromString<double>(record.getValue(DISCRULE_VALUE)));
    dsc->setType(fromString<int>(record.getValue(DISCRULE_TYPE)));
    dsc->setBDate(record.getValue(DISCRULE_BDATE));
    dsc->setEDate(record.getValue(DISCRULE_EDATE));
    dsc->setBTime(record.getValue(DISCRULE_BTIME));
    dsc->setETime(record.getValue(DISCRULE_ETIME));    
    dsc->setWeekdays(fromString<int>(record.getValue(DISCRULE_WDAYS))); 
    dsc->setSumCheck(fromString<double>(record.getValue(DISCRULE_SUMCHECK)));    
    dsc->setAccumulation(fromString<double>(record.getValue(DISCRULE_ACCUMULATION)));
}

void DiscRuleMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
//printf("void DiscRuleMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {\n");
    DiscRule * dsc = (DiscRule*) object;
    rdb_expression.addField(DISCRULE_CODE, toString<int>(dsc->getOID()));
    rdb_expression.addField(DISCRULE_SCHEME_ID, toString<int>(dsc->getSchemeId()));
    rdb_expression.addField(DISCRULE_LONGTITLE, dsc->getLongTitle());
    rdb_expression.addField(DISCRULE_SHORTCUT, dsc->getShortcut());
    rdb_expression.addField(DISCRULE_VALUE, toString<double>(dsc->getValue()));
    rdb_expression.addField(DISCRULE_TYPE, toString<int>(dsc->getType()));
    rdb_expression.addField(DISCRULE_BDATE, dsc->getBDate());
    rdb_expression.addField(DISCRULE_EDATE, dsc->getEDate());
    rdb_expression.addField(DISCRULE_BTIME, dsc->getBTime());
    rdb_expression.addField(DISCRULE_ETIME, dsc->getETime());
    rdb_expression.addField(DISCRULE_WDAYS, dsc->getWeekdays());
    rdb_expression.addField(DISCRULE_SUMCHECK, dsc->getSumCheck());
    rdb_expression.addField(DISCRULE_ACCUMULATION, dsc->getAccumulation());
}
