/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Алексанр
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "Manager.h"
#include "ManagerCatalog.h"

ManagerInfo::ManagerInfo() {
}

Manager::Manager() {
    this->class_name = "Manager";
}

ManagerInfo Manager::getInfo() {
    return info;
}

void Manager::setInfo(const ManagerInfo& manager_info) {
    info = manager_info;
}

void Manager::copyInfo(Object* obj) {
    Manager * orig_obj = (Manager*) obj;
    this->setInfo(orig_obj->getInfo());
}

