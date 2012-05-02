/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savrasov Alexander aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Саврасов Александр aka Zuskin.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "DiscScheme.h"

DiscSchemeInfo::DiscSchemeInfo() {
    //-----(+)-----Zuskin-----15/12/2011-----
    this->is_default = false;
    //---------------------------------------
}

DiscScheme::DiscScheme() {
    this->class_name = "DiscScheme";
}

void DiscScheme::setInfo(const DiscSchemeInfo& dsc_info) {
    info = dsc_info;
}

DiscSchemeInfo DiscScheme::getInfo() {
    return info;
}

void DiscScheme::copyInfo(Object* obj) {
    DiscScheme * dsc = (DiscScheme*)obj;
    this->setInfo(dsc->getInfo());
}

Object * DiscScheme::createCopyObject() {
    DiscScheme * copy_object = new DiscScheme();
    copy_object->setInfo(this->getInfo());
    return copy_object;
}

int DiscScheme::getOID() const {
    return info.code;
}
string DiscScheme::getLongTitle() const {
    return info.long_name;
}	
string DiscScheme::getShortcut() const {
    return info.short_name;
}
void DiscScheme::setCode(const int code) {
    info.code = code;
}

void DiscScheme::setLongTitle(const string & dsc_lttl) {
    info.long_name = dsc_lttl;
}
void DiscScheme::setShortcut(const string & dsc_scut) {
    info.short_name = dsc_scut;
}

//-----(+)-----Zuskin-----14/12/2011-----
bool DiscScheme::getIsDefault() const {
    return info.is_default;
}

void DiscScheme::setIsDefault(const bool & dsc_isdfl) {
    info.is_default = dsc_isdfl;
}
//---------------------------------------
