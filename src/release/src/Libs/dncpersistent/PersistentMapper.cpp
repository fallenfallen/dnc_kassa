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
#include "PersistentMapper.h"

ObjectsCollection * PersistentMapper::getObjectsCollection(const ObjectFilter &filter) {
    return getObjectsCollectionFromStorage(filter);
}


void PersistentMapper::addNewObject(Object* new_object) {
    addNewObjectToStorage(new_object);
}

void PersistentMapper::updateObject(Object* object) {
    updateObjectToStorage(object);
}

void PersistentMapper::reloadObject(Object* object) {
    reloadObjectFromStorage(object);
}