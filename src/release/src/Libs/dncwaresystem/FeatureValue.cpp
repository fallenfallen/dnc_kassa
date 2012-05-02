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

#include "FeatureValue.h"

FeatureValue::FeatureValue() {
    this->class_name = "FeatureValue";
}

FeatureValue::FeatureValue(const string & meaning) {
    this->class_name = "FeatureValue";
    this->meaning = meaning;
}

string FeatureValue::getMeaning() const {
    return meaning;
}
void FeatureValue::setMeaning(const string& meaning) {
    this->meaning = meaning;
}

string FeatureValue::getCode() const {
    return code;
}
void FeatureValue::setCode(const string& code) {
    this->code = code;
}

int FeatureValue::getFeatureId() {
    return feature_id;
}
void FeatureValue::setFeatureId(int f_id) {
    feature_id = f_id;
}

void FeatureValue::getCopy(FeatureValue* copy_value) {
    copy_value->setMeaning(getMeaning());
    copy_value->setCode(getCode());
    copy_value->setFeatureId(feature_id);
    copy_value->setOID(this->getOID());
}

/**************************************************/

SelectedValue::SelectedValue() {
    class_name = "SelectedValue";
    value = NULL;
}

SelectedValue::SelectedValue(int feature_id) {
    class_name = "SelectedValue";
    this->feature_id = feature_id;
    value = NULL;
}

SelectedValue::~SelectedValue() {
    delete value;
}

void SelectedValue::setValue(FeatureValue* fvalue) {
    if ( value != NULL )
        delete value;
    this->value = fvalue;
}
int SelectedValue::getValueId() const {
    return value->getOID();
}

int SelectedValue::getFeatureId() {
    return feature_id;
}

string SelectedValue::getMeaning() const {
    return value->getMeaning();
}
void SelectedValue::setFeatureId(int f_id) {
    feature_id = f_id;
}

int SelectedValue::getAdditionId() const {
    return addition_id;
}
void SelectedValue::setAdditionId(int additionid) {
    addition_id = additionid;
}
