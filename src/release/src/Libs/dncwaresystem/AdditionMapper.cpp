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

#include "AdditionMapper.h"
#include "WareAddition.h"

AdditionMapper::AdditionMapper() {
    //table_name = "ware_additions";
    from_db_table_name = "ware_additions";
    to_db_table_name = "ware_additions";
}

string AdditionMapper::getIdFieldName() {
    return ADDITION_ID_FIELD;
}
string AdditionMapper::getCodeFieldName() {
    return ADDITION_CODE;
}

Object * AdditionMapper::createObjectByDefault() {
    WareAddition * ware_addition = new WareAddition;
    return ware_addition;
}

Object* AdditionMapper::createObjectFromResultRecord(const ResultRecord& record) {
    WareAddition * ware_addition = new WareAddition;
    fromPersistent(ware_addition, record);
    return ware_addition;
}

void AdditionMapper::fromPersistent(Object* object, const ResultRecord& record) {
    WareAddition * wa = (WareAddition*) object;
    WareAdditionInfo wa_info;
    wa_info.code = record.getValue(ADDITION_CODE);
    wa_info.article = record.getValue(ADDITION_ARTICLE);
    wa_info.price = fromString<double>(record.getValue(ADDITION_PRICE));
    wa_info.quantity = fromString<double>(record.getValue(ADDITION_QUANTITY));
    wa_info.ware_id = fromString<int>(record.getValue(ADDITION_WARE_ID));

    wa->replaceInfo(wa_info);
}

void AdditionMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    WareAddition * addition = (WareAddition*)object;
    rdb_expression.addField(ADDITION_CODE, addition->getCode());
    rdb_expression.addField(ADDITION_ARTICLE, addition->getArticle());
    rdb_expression.addField(ADDITION_PRICE, addition->getPrice());
    rdb_expression.addField(ADDITION_QUANTITY, addition->getQuantity());
    rdb_expression.addField(ADDITION_WARE_ID, addition->getInfo().ware_id);
}
