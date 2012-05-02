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
#include "WareGroupMapper.h"
//---(+)---Zuskin---20/02/2012---
#include "PersistentFacade.h"
//-------------------------------

WareGroupMapper::WareGroupMapper() {
    to_db_table_name = "ware_group";
    from_db_table_name = "ware_group";
}

string WareGroupMapper::getIdFieldName() {
    return GROUP_ID_FIELD;
}

string WareGroupMapper::getCodeFieldName() {
    return GROUP_CODE;
}

Object * WareGroupMapper::createObjectByDefault() {
    WareGroup * ware_group = new WareGroup();
    //---(+)---Zuskin---20/02/2012---
    MaximumFilter max_filter(GROUP_ID_FIELD, "WareGroup");
    ware_group->setGroupId((int)PersistentFacade::getInstance()->getMaximum(max_filter)+1);
    //-------------------------------
    return ware_group;
}

Object* WareGroupMapper::createObjectFromResultRecord(const ResultRecord& record) {
    WareGroup * group = new WareGroup();
    fromPersistent(group, record);
    return group;
}

void WareGroupMapper::fromPersistent(Object* object, const ResultRecord& record) {
    WareGroup* group = (WareGroup*)object;
    group->setCode(record.getValue(GROUP_CODE));
    group->setTitle(record.getValue(GROUP_TITLE));
    group->setText(record.getValue(GROUP_TEXT));
    group->assignParent(fromString<int>(record.getValue(GROUP_PARENT_ID)));
}

void WareGroupMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    WareGroup* group = (WareGroup*)object;
    rdb_expression.addField(GROUP_CODE, group->getCode());
    rdb_expression.addField(GROUP_TITLE, group->getTitle());
    rdb_expression.addField(GROUP_TEXT, group->getText());
    rdb_expression.addField(GROUP_PARENT_ID, group->getParentId());

    /******** for test ************/
    //---(*)---Zuskin---20/02/2012---
    //rdb_expression.addField("group_id", fromString<int>(group->getCode()));
    if (group->getGroupId() > 0) {
        rdb_expression.addField(GROUP_ID_FIELD, group->getGroupId());
    }    
    //-------------------------------
}

