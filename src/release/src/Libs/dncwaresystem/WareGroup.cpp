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

#include "WareGroup.h"
#include "WareCatalog.h"
#include "WareSystem.h"

WareGroup::WareGroup() {
    class_name = "WareGroup";
    parent = NULL;
    //---(+)---Zuskin---20/02/2012---
    group_id = 0;
    //-------------------------------
}

string WareGroup::getText() const {
    return text;
}
string WareGroup::getTitle() const {
    return title;
}
string WareGroup::getCode() const {
    return code;
}

//---(+)---Zuskin---20/02/2012---
int WareGroup::getGroupId() const {
    return group_id;
}
//-------------------------------

void WareGroup::setTitle(const string& gtitle) {
    title = gtitle;
}
void WareGroup::setText(const string& gtext) {
    text = gtext;
}
void WareGroup::setCode(const string& gcode) {
    code = gcode;
}

//---(+)---Zuskin---20/02/2012---
void WareGroup::setGroupId(int gid) {
    group_id = gid;
}
//-------------------------------

int WareGroup::getParentId() const {
    if ( parent != NULL )
        return parent->getOID();
    else
        return -1;
}

WareGroup* WareGroup::getParentGroup() {
    return parent;
}

void WareGroup::assignParent(int parent_group_id) {
    if ( parent_group_id > 0 ) {
        WareCatalog * group_catalog = WareSystem::getInstance()->getWareCatalog();
        parent = group_catalog->getWareGroupById(parent_group_id);
    } else 
        parent =  NULL;
}

void WareGroup::setParent(WareGroup* parent_group) {
    parent = parent_group;
}

void WareGroup::copyInfo(Object* obj) {
    WareGroup * orig_group = ((WareGroup*)obj);
    this->code = orig_group->getCode();
    this->title = orig_group->getTitle();
    this->text = orig_group->getText();
    this->parent = orig_group->getParentGroup();
}

Object * WareGroup::createCopyObject() {
    //cout << "WareGroup::createCopyObject()" << endl;
    WareGroup * copy_object = new WareGroup();
    copy_object->setCode(this->getCode());
    copy_object->setText(this->getText());
    copy_object->setTitle(this->getTitle());
    copy_object->setParent(this->getParentGroup());
    return copy_object;
}

void WareGroup::clear() {
    WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
    ware_catalog->dropGroupSubGroups(this->getOID());
    ware_catalog->dropWareFromGroup(this->getOID());
}
