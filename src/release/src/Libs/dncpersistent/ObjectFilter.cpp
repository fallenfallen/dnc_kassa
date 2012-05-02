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

#include "ObjectFilter.h"
#include "PersistentFacade.h"
#include "RDBWhereFilter.h"

bool ObjectFilter::compareObject(Object* obj) const {
    return false;
}

ObjectFilterById::ObjectFilterById(int oid, const string& object_class_name) {
    this->compared_id = oid;
    class_name = object_class_name;
}
bool ObjectFilterById::compareObject(Object* obj) const {
    return obj->getOID() == compared_id;
}
string ObjectFilterById::getClassName() const {
    return class_name;
}
string ObjectFilterById::getFilterExpression() const {
    RDBWhereFilter rdb_filter;
    string id_field_name = PersistentFacade::getInstance()->getIdFieldName(class_name);
    rdb_filter.addWhereCondition(id_field_name, EQUAL, compared_id);
    return rdb_filter.getWhereExpression();
}


ObjectFilterByCode::ObjectFilterByCode(const string& object_internal_code, const string& object_class_name) {
    compared_code = object_internal_code;
    class_name = object_class_name;
}
bool ObjectFilterByCode::compareObject(Object* obj) const {
    return obj->getCode() == compared_code;
}
string ObjectFilterByCode::getClassName() const {
    return class_name;
}
string ObjectFilterByCode::getFilterExpression() const {
    RDBWhereFilter rdb_filter;
    string code_field_name = PersistentFacade::getInstance()->getCodeFieldName(class_name);
    rdb_filter.addWhereCondition(code_field_name, EQUAL, compared_code);
    return rdb_filter.getWhereExpression();
}


EmptyObjectFilter::EmptyObjectFilter(const string& object_class_name) {
    this->class_name = object_class_name;
}
bool EmptyObjectFilter::compareObject(Object* obj) const {
    return true;
}
string EmptyObjectFilter::getClassName() const {
    return class_name;
}
string EmptyObjectFilter::getFilterExpression() const {
    return "";
}



MaximumFilter::MaximumFilter(const string& max_field_name, const string& object_class_name) {
    this->field_name = max_field_name;
    this->class_name = object_class_name;
}
string MaximumFilter::getClassName() const {
    return class_name;
}
string MaximumFilter::getFilterExpression() const {
    return "";
}
string MaximumFilter::getMaxFieldName() const {
    return field_name;
}

