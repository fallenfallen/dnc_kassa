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

#include "ObjectState.h"
#include "PersistentFacade.h"
#include "Error.h"

ObjectState * NewState::instance = new NewState;
ObjectState * LegacyState::instance = new LegacyState;
ObjectState * DeletedState::instance = new DeletedState;

void ObjectState::changeState(Object* obj, ObjectState* state) {
    obj->setState(state);
}

void ObjectState::load(Object* obj) {}

void ObjectState::save(Object* obj) {}

void ObjectState::update(Object* obj) {}

void ObjectState::drop(Object* obj) {}

void ObjectState::reload(Object* obj) {}

/*************************************************************/
void NewState::save(Object* obj) {
    try {
        PersistentFacade::getInstance()->insertObject(obj);
        changeState(obj, LegacyState::instance);
    } catch (const Error & err) {
        throw err;
    }
}

void NewState::load(Object* obj) {
    changeState(obj, LegacyState::instance);
}

void NewState::drop(Object* obj) {
    changeState(obj, DeletedState::instance);
}

/*************************************************************/

void LegacyState::reload(Object* obj) {
    PersistentFacade::getInstance()->reloadObject(obj);
}

void LegacyState::update(Object* obj) {
    try {
        PersistentFacade::getInstance()->updateObject(obj);
    } catch (const Error & err) {
        this->reload(obj);
        throw err;
    }
}

void LegacyState::drop(Object* obj) {
    PersistentFacade::getInstance()->dropObjectsByFilter(ObjectFilterById(obj->getOID(), obj->getClassName()));
    changeState(obj, DeletedState::instance);
}

/*************************************************************/