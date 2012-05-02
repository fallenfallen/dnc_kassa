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

#include "Feature.h"
#include <algorithm>
#include <iostream>

#include "ObjectFilter.h"
#include "Error.h"
#include "ObjectFilters.h"

Feature::Feature() {
    this->class_name = "Features";
}

Feature::Feature(const string & fname) {
    name = fname;
    this->class_name = "Features";
}

string Feature::getName() const {
    return name;
}

void Feature::setName(const string  & fname) {
    name = fname;
}

string Feature::getCode() const {
    return code;
}
void Feature::setCode(const string& code) {
    this->code = code;
}

void Feature::addParentSet(int set_id) {
    if ( find(parents.begin(), parents.end(), set_id) == parents.end() )
        parents.push_back(set_id);
}

bool Feature::isParentSet(int set_id) {
    return (find(parents.begin(), parents.end(), set_id) != parents.end());
}

void Feature::lookParallelObject(Object* obj) {
    vector<int> parents_parallel_object = ((Feature*)obj)->getParentsId();
    for (vector<int>::iterator it = parents_parallel_object.begin(); it != parents_parallel_object.end(); it++)
        this->addParentSet(*it);
}

vector<int> Feature::getParentsId() const{
    return parents;
}

void Feature::addPossibleValue(const FeatureValue& pos_value) {
    try {
        ObjectFilterByCode filter(pos_value.getCode(), "FeatureValue");
        FeatureValue * exist_value = (FeatureValue*) possible_values.getObject(filter);
        exist_value->setCode(pos_value.getCode());
        exist_value->setFeatureId(this->getOID());
        exist_value->update();
    } catch (const Error & err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND)
            insertPossibleValue(pos_value);
        else
            throw err;
    }
}

void Feature::insertPossibleValue(const FeatureValue& f_value) {
    FeatureValue * pos_value = new FeatureValue(f_value.getMeaning());
    pos_value->setCode(f_value.getCode());
    pos_value->setFeatureId(this->getOID());

    possible_values.insertObject(pos_value);
}

void Feature::clear() {
    possible_values.dropObjectByFilter(PossibleValueInFeature(this->getOID()));
}


/*FeatureValue * Feature::getAdditionValue(int addition_id) {
    Filter filter("addition_id", addition_id);
    filter.add("feature_id", getOID());
    return (FeatureValue*) possible_values.loadObject("FeatureValue", filter);
}*/


/*
 *
void Feature::editPossibleValue(int index_value, const char* value) {
    if ( index_value < possible_values.size() )
        possible_values[index_value].setValue(value);
}
void Feature::removePossibleValue(int index_value) {
    if ( index_value < possible_values.size() )
        possible_values.erase(possible_values.begin() + index_value);
}

int Feature::getCountPossibleValue() const {
    return possible_values.size();
}

string Feature::getPossibleValue(int index) const {
    if ( index < possible_values.size() && index > -1 )
        return possible_values.at(index).getValue();
    return Value().getValue();
}

Value * Feature::getValuePoint(const string& value) {
    vector<Value>::iterator it = existPossibleValue(value);
    if ( it == possible_values.end() ) {
        possible_values.push_back(Value(value));
        return &(*(possible_values.end() - 1));
    } else {
        return &(*it);
    }
}

 vector<Value>::iterator Feature::existPossibleValue(const string& value) {
    return find_if(possible_values.begin(), possible_values.end(), Value_equal(value));
}*/