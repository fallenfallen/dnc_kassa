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
#include <vector>
#include <iostream>
#include "ObjectsCollection.h"
#include "Error.h"

ObjectsCollection::ObjectsCollection() {
}

ObjectsCollection::~ObjectsCollection() {
    for ( map<int, Object*>::iterator it = objects.begin(); it != objects.end(); it++ ) {
        delete it->second;
    }
    objects.clear();
}

void ObjectsCollection::addObject(Object* obj) {
    if ( objects.find(obj->getOID()) != objects.end() )
        return;
    objects[obj->getOID()] = obj;
}

Object * ObjectsCollection::seizeFirstObject() {
    // передает права на владения объектом "Object" (из первого элемента) вызвовшему функцию объекту !!!
    if ( objects.empty() )
        throw Error(209, "Невозможно получить информацию об объекте", "Оbjects collection is empty");
    Object * o = objects.begin()->second;
    objects.erase(objects.begin());
    return o;
}

bool ObjectsCollection::isEmpty() {
    return objects.empty();
}

vector<Object* > ObjectsCollection::seizeObjects() {
    vector<Object *> result_objects;
    for(map<int, Object*>::iterator it = objects.begin(); it != objects.end(); it++) {
        result_objects.push_back(it->second);
        objects.erase(it);
    }
    return result_objects;
}

void ObjectsCollection::setLoadedState() {
    for(map<int, Object*>::iterator it = objects.begin(); it != objects.end(); it++) {
        (it->second)->setloadedState();
    }
}