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

#include <ostream>
#include "PersistentFacade.h"
#include "MapperContainer.h"
#include "stringconvert.h"
#include "Error.h"
#include "RDBOperation.h"

PersistentFacade * PersistentFacade::instance = NULL;
int PersistentFacade::ref_count = 0;


PersistentFacade * PersistentFacade::getInstance() {
    if ( instance == NULL )
        throw Error(214, "Сервис преобразования данных не запущен", "PersistentFasade is NULL");
    return instance;
}

void PersistentFacade::startInstance() {
    if ( instance == NULL )
        instance = new PersistentFacade();
    ref_count++;
    RDBOperation::startInstance();
}

void PersistentFacade::finishInstance() {
    RDBOperation::finishInstance();
    ref_count--;
    if ( ref_count == 0 ) {
        delete instance;
        instance = NULL;
    }
}

PersistentFacade::PersistentFacade() {
    initMappers();
}

void PersistentFacade::initMappers() {
    mappers = MapperContainer::getAllMappers();
}

void PersistentFacade::findMapper(const string& class_name) {
    if ( mappers.find(class_name) == mappers.end()  )  {
        string err = "Mapper ";
        err += class_name;
        err += " not found";
        throw Error(208, "Невозможно получить/cохранить данные", err);
    }
}

Object* PersistentFacade::getObject(const ObjectFilter &filter) {
    
    ObjectsCollection* collection = getObjectsCollection(filter);
    if ( collection->isEmpty() ) {
        delete collection;
        string err_message = "Объект " + filter.getClassName() + " не найден";
//        puts(err_message.c_str());
        throw Error(209, err_message, "Result is empty");
    }
    
    Object * object = collection->seizeFirstObject();
    delete collection;

    return object;
}

ObjectsCollection * PersistentFacade::getObjectsCollection(const ObjectFilter &filter) {
    findMapper(filter.getClassName());
    return mappers.at(filter.getClassName())->getObjectsCollection(filter);
}

string PersistentFacade::getIdFieldName(const string& class_name) {
    findMapper(class_name);
    return mappers.at(class_name)->getIdFieldName();
}

string PersistentFacade::getCodeFieldName(const string& class_name) {
    findMapper(class_name);
    return mappers.at(class_name)->getCodeFieldName();
}

void PersistentFacade::insertObject(Object* new_object) {
    string class_name = new_object->getClassName();    
    findMapper(class_name);
    mappers.at(class_name)->addNewObject(new_object);
}

void PersistentFacade::updateObject(Object* object) {
    string class_name = object->getClassName();
    findMapper(class_name);
    mappers.at(class_name)->updateObject(object);
}

void PersistentFacade::reloadObject(Object* object) {
    string class_name = object->getClassName();
    findMapper(class_name);
    mappers.at(class_name)->reloadObject(object);
}

void PersistentFacade::dropObjectsByFilter(const ObjectFilter& filter) {
    findMapper(filter.getClassName());
    mappers.at(filter.getClassName())->dropObjects(filter);
}

double PersistentFacade::getMaximum(const MaximumFilter& filter) {
    findMapper(filter.getClassName());
    return mappers.at(filter.getClassName())->getMaximum(filter);
}

Object * PersistentFacade::createObjectByDefault(const string& class_name) {
    findMapper(class_name);
    return mappers.at(class_name)->createObjectByDefault();
}