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

#include "ObjectCatalog.h"
#include "PersistentFacade.h"
#include "Error.h"
#include "ObjectCreator.h"

#include <iostream>

ObjectCatalog::~ObjectCatalog() {
    deleteObjects();
}

void ObjectCatalog::deleteObjects() {
    for(vector<Object*>::iterator it = objects.begin(); it != objects.end(); it++) {
        delete *it;
    }
    objects.clear();
}

Object * ObjectCatalog::getObject(const ObjectFilter& filter) {
    try {
        return findExistObject(filter);
    } catch (const Error & err) {
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND ) 
            return loadObject(filter);
        else
            throw err;
    }
}

Object * ObjectCatalog::findExistObject(const ObjectFilter& filter) {
    vector<Object*>::reverse_iterator it = objects.rbegin();
    while ( it != objects.rend() && filter.compareObject(*it) != true )
        ++it;   
    if ( it == objects.rend() )
        throw Error(ERROR_OBJECT_NOT_FOUND, "Объект не найден", "Object not found");
    else 
        return (*it);
}

Object * ObjectCatalog::loadObject(const ObjectFilter& filter) {
    Object * loaded_object = PersistentFacade::getInstance()->getObject(filter);
    loaded_object->setloadedState();
    addObjectToCatalog(loaded_object);
    return findExistObject(filter);
}

void ObjectCatalog::addObjectToCatalog(Object* object) {
    Object * finded_object;
    try {
        finded_object = findExistObject(ObjectFilterById(object->getOID(), object->getClassName()));
        finded_object->lookParallelObject(object);
        delete object;
    } catch (const Error& err) {
         if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
             objects.push_back(object);
         else
             throw err;
    }
}

vector<Object*> ObjectCatalog::getAllObjects(const ObjectFilter& filter) {
    deleteObjects();
    loadObjectCollectionToBuffer(filter);
    objects = buffer_collection;
    buffer_collection.clear();
    return objects;
}

vector<Object*> ObjectCatalog::getObjects(const ObjectFilter& filter) {
    loadObjectCollectionToBuffer(filter);
    for (vector<Object*>::iterator it = buffer_collection.begin(); it != buffer_collection.end(); it++)
        addObjectToCatalog(*it);
    buffer_collection.clear();
    return getExistObjects(filter);
}

void ObjectCatalog::loadObjectCollectionToBuffer(const ObjectFilter& filter) {
    ObjectsCollection * collection = PersistentFacade::getInstance()->getObjectsCollection(filter);
    collection->setLoadedState();
    buffer_collection = collection->seizeObjects();
    delete collection;
}

vector<Object*> ObjectCatalog::getExistObjects(const ObjectFilter& filter) {
    vector<Object*> result_object;
    for (vector<Object*>::iterator it = objects.begin(); it != objects.end(); it++)
        if ( filter.compareObject(*it) )
            result_object.push_back(*it);
    return result_object;
}

void ObjectCatalog::insertObject(Object* obj) {
    obj->save();
    objects.push_back(obj);
}

void ObjectCatalog::dropObjectByFilter(const ObjectFilter& filter) {

    vector<Object*> droping_objects = getObjects(filter);
    
    for (int i = 0; i < droping_objects.size(); i++) 
        droping_objects[i]->clear();

    PersistentFacade::getInstance()->dropObjectsByFilter(filter);
    
    for (int i = objects.size() - 1; i > -1; i--) 
        if ( filter.compareObject(objects[i]) ) {
            delete objects[i];
            objects.erase(objects.begin() + i);
        }
}

Object * ObjectCatalog::addObject(Object* obj, const ObjectFilter& filter) {
    try {
        Object * exist_object = getObject(filter);
        exist_object->copyInfo(obj);
        exist_object->update();
        return exist_object;
    } catch  (const Error & err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND) {
            return addNewObject(obj);
        }    
        else
            throw err;
    }
}

Object * ObjectCatalog::addNewObject(Object* obj) {
    Object * new_object = PersistentFacade::getInstance()->createObjectByDefault(obj->getClassName());
    new_object->copyInfo(obj);
    try {
        insertObject(new_object);
    } catch (const Error & err ) {
        delete new_object;
        throw err;
    }
    return new_object;
}


