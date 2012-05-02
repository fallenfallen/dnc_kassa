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

#include "RDBMapper.h"
#include "RDBOperation.h"
#include "stringconvert.h"


ObjectsCollection * RDBMapper::getObjectsCollectionFromStorage(const ObjectFilter& filter) {
    ResultSet rs = getDBResult(filter);
    ObjectsCollection * o = getObjectsCollectionFormResult(rs);
    return o;
}

ResultSet RDBMapper::getDBResult(const ObjectFilter& filter) {
    return RDBOperation::getInstance()->getFromTable(from_db_table_name, filter);
}

ObjectsCollection * RDBMapper::getObjectsCollectionFormResult(ResultSet& rs) {
    ObjectsCollection * collection = new ObjectsCollection();
    for ( int i = 0; i < rs.getCountRecord(); i++ ) {
        Object * object = createObjectFromResultRecord(rs.getRecord(i));
        int id = fromString<int>(rs.getValue(i, getIdFieldName()));
        object->setOID(id);
        collection->addObject(object);
    }
    return collection;
}

void RDBMapper::addNewObjectToStorage(Object* new_object) {
    RDBInsertExpression insert_exp = getInsertExpression(new_object);
    ResultSet rs = insertToDB(insert_exp);
    setIdInsertedObject(new_object, rs);
}

RDBInsertExpression RDBMapper::getInsertExpression(Object* object) {
    RDBInsertExpression insert_exp;
    toPersistent(object, insert_exp);
    insert_exp.addReturningField(getIdFieldName());
    return insert_exp;
}

ResultSet RDBMapper::insertToDB(const RDBInsertExpression& insert_exp) {
    return RDBOperation::getInstance()->insertToDB(to_db_table_name, insert_exp);
}

void RDBMapper::setIdInsertedObject(Object* object, ResultSet& rs) {
    if ( rs.getCountRecord() == 0 ) return;
    int id = fromString<int>(rs.getValue(0, getIdFieldName()));
    if ( id > 0 )
        object->setOID(id);
}

void RDBMapper::updateObjectToStorage(Object* object) {
    RDBUpdateExpression update_exp = getUpdateExpression(object);
    updateToDB(update_exp);
}

RDBUpdateExpression RDBMapper::getUpdateExpression(Object* object) {
    RDBUpdateExpression update_exp;
    toPersistent(object, update_exp);
    update_exp.where_filter.addWhereCondition(getIdFieldName(), EQUAL, object->getOID());
    return update_exp;
}

void RDBMapper::updateToDB(const RDBUpdateExpression& update_exp) {
    RDBOperation::getInstance()->updateToDB(to_db_table_name, update_exp);
}

void RDBMapper::reloadObjectFromStorage(Object* object) {
    ResultSet rs = getDBResult(ObjectFilterById(object->getOID(), object->getClassName()));
    if ( rs.getCountRecord() < 1 )
        throw(218, "Перезагрузка объекта не возможна", "Object by id not found");
    fromPersistent(object, rs.getRecord(0));
}

void RDBMapper::dropObjects(const ObjectFilter& filter) {
    RDBOperation::getInstance()->dropFromTable(to_db_table_name, filter);
}

double RDBMapper::getMaximum(const MaximumFilter& filter) {
    ResultSet rs = RDBOperation::getInstance()->getMaximum(from_db_table_name, filter);
    double max = fromString<double>(rs.getValue(0, 0));
    return max;
}
