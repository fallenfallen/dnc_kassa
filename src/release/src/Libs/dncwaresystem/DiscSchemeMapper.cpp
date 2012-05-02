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

#include "DiscSchemeMapper.h"

DiscSchemeMapper::DiscSchemeMapper() {
    //table_name = "ware_barcode";
    from_db_table_name = "auto_scheme";
    to_db_table_name = "auto_scheme";
}

string DiscSchemeMapper::getIdFieldName() {
    return DISCSCHEME_ID_FIELD;
}

Object * DiscSchemeMapper::createObjectByDefault() {
    DiscScheme * dsc = new DiscScheme();
    return dsc;
}

Object * DiscSchemeMapper::createObjectFromResultRecord(const ResultRecord& record) {
    DiscScheme * dsc = new DiscScheme();
    fromPersistent(dsc, record);
    return dsc;
}

void DiscSchemeMapper::fromPersistent(Object* object, const ResultRecord& record) {
    DiscScheme * dsc = (DiscScheme*) object;
    dsc->setCode(fromString<int>(record.getValue(DISCSCHEME_CODE)));
    dsc->setLongTitle(record.getValue(DISCSCHEME_LONGTITLE));
    dsc->setShortcut(record.getValue(DISCSCHEME_SHORTCUT));
    //-----(+)-----Zuskin-----14/12/2011-----
    dsc->setIsDefault(fromString<bool>(record.getValue(DISCSCHEME_IS_DEFAULT)));
    //---------------------------------------
}

void DiscSchemeMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    DiscScheme * dsc = (DiscScheme*) object;
    rdb_expression.addField(DISCSCHEME_CODE, dsc->getOID());
    rdb_expression.addField(DISCSCHEME_LONGTITLE, dsc->getLongTitle());
    rdb_expression.addField(DISCSCHEME_SHORTCUT, dsc->getShortcut());
    //-----(+)-----Zuskin-----14/12/2011-----
    rdb_expression.addField(DISCSCHEME_IS_DEFAULT, (dsc->getIsDefault() ? "t" : "f"));
    //---------------------------------------
}