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

#include "Object.h"
#include <iostream>

Object::Object() {
    setState(NewState::instance);
    oid = -1;
}

string Object::getClassName()    {        
    return class_name;
}

void Object::setOID(int id)    {
    oid = id;
}
int Object::getOID() const {
    return oid;
}

string Object::getCode() const {
    return string();
}

void Object::setloadedState() {
    state->load(this);
}

void Object::save() {
    state->save(this);
}

void Object::update() {
    state->update(this);
}

void Object::drop() {
    clear();
    state->drop(this);
}

void Object::reload() {
    state->reload(this);
}

void Object::setState(ObjectState* st) {
    state = st;
}

void Object::lookParallelObject(Object* obj) {}

void Object::clear() {}

void Object::copyInfo(Object* obj) {}

/*Object* Object::createCopyObject() {
    //cout << "Object::createCopyObject()" << endl;
    //cout << "this class name = " << this->getClassName() << endl;
    Object * copy_object = new Object(*this);
    //cout << "copy_object class name = " << copy_object->getClassName() << endl;
    //cout << "copy object code = " << copy_object->getCode() << endl;
    return copy_object;
}*/

