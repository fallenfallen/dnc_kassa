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

#include "FeatureValueMapper.h"
#include "FeatureValue.h"

FeatureValueMapper::FeatureValueMapper() {
    //table_name = "wa_fv_view";
    from_db_table_name = "features_values";
    to_db_table_name = "features_values";
}

string FeatureValueMapper::getIdFieldName() {
    return FVALUE_ID_FIELD;
}

Object * FeatureValueMapper::createObjectByDefault() {
    FeatureValue * f_value = new FeatureValue;
    return f_value;
}

Object * FeatureValueMapper::createObjectFromResultRecord(const ResultRecord& record) {
    FeatureValue * f_value = new FeatureValue;
    fromPersistent(f_value, record);
    return f_value;
}

void FeatureValueMapper::fromPersistent(Object* object, const ResultRecord& record) {
    FeatureValue * fvalue = (FeatureValue*)object;
    fvalue->setMeaning(record.getValue(FVALUE_MEANING));
    fvalue->setCode(record.getValue(FVALUE_CODE));
    fvalue->setFeatureId(fromString<int>(record.getValue(FVALUE_FEATURE_ID)));
}

void FeatureValueMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    FeatureValue * fvalue = (FeatureValue*)object;
    rdb_expression.addField(FVALUE_CODE, fvalue->getCode());
    rdb_expression.addField(FVALUE_MEANING, fvalue->getMeaning());
    rdb_expression.addField(FVALUE_FEATURE_ID, fvalue->getFeatureId());
}

/*********************************************************************/

SelectedValueMapper::SelectedValueMapper() {
    //table_name = "wa_fv_view";
    from_db_table_name = "wa_fv_view";
    to_db_table_name = "ware_additions_features_values";
}

string SelectedValueMapper::getIdFieldName() {
    return SFVALUE_ID_FIELD;
}

Object * SelectedValueMapper::createObjectByDefault() {
    SelectedValue * svalue = new SelectedValue();
    return svalue;
}

Object * SelectedValueMapper::createObjectFromResultRecord(const ResultRecord& record) {
    SelectedValue * svalue = new SelectedValue();
    fromPersistent(svalue, record);
    return svalue;
}

void SelectedValueMapper::fromPersistent(Object* object, const ResultRecord& record) {
    SelectedValue * svalue = (SelectedValue*) object;
    
    svalue->setFeatureId(fromString<int>(record.getValue(SFVALUE_FEATURE_ID)));
    svalue->setAdditionId(fromString<int>(record.getValue(SFVALUE_ADDITION_ID)));
    
    FeatureValue * fvalue = new FeatureValue(record.getValue(SFVALUE_VALUE));
    fvalue->setOID(fromString<int>(record.getValue(SFVALUE_VALUE_ID)));
    fvalue->setCode(record.getValue(SFVALUE_VALUE_CODE));
    
    svalue->setValue(fvalue);
}

void SelectedValueMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    SelectedValue * svalue = (SelectedValue*)object;
    rdb_expression.addField(SFVALUE_ADDITION_ID, svalue->getAdditionId());
    rdb_expression.addField(SFVALUE_VALUE_ID, svalue->getValueId());
}

