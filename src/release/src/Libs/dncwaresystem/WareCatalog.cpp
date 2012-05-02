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

#include "WareCatalog.h"
#include "Error.h"
#include "ObjectFilters.h"
#include "RDBOperation.h"



Ware * WareCatalog::getWareByCode(const string& internal_code) {
    return (Ware*)ware_catalog.getObject(WareFilterByCode(internal_code));
}

void WareCatalog::addWare(const WareInfo& ware_info, bool replace_quantity) {
    ObjectFilterByCode filter(ware_info.internal_code, "Ware");
    try {
        Ware * exist_ware = (Ware*) ware_catalog.getObject(filter);
        exist_ware->replaceInfo(ware_info, replace_quantity);
        exist_ware->update();
    } catch (const Error & err) {
         if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND) {
            Ware new_ware;
            new_ware.replaceInfo(ware_info);
            ware_catalog.addObject(&new_ware, filter);
         }  else
            throw err;
    }
}

void WareCatalog::dropAllAdditions() {
    vector<Object*> ware_with_fs = ware_catalog.getObjects(AllWareWithFS());
    for (vector<Object*>::iterator it = ware_with_fs.begin(); it != ware_with_fs.end(); it++ ) {
        ((Ware*)(*it))->clearAdditions();
    }
}

void WareCatalog::deleteFSFromAllWares() {
    vector<Object*> ware_with_fs = ware_catalog.getObjects(AllWareWithFS());
    for (vector<Object*>::iterator it = ware_with_fs.begin(); it != ware_with_fs.end(); it++ ) {
        ((Ware*)(*it))->clearAdditions();
        ((Ware*)(*it))->setFeaturesSet(NULL);
        ((Ware*)(*it))->update();
    }
}

void WareCatalog::clearCatalog() {
    group_catalog.dropObjectByFilter(GroupSubItemFilter(-1));
    ware_catalog.dropObjectByFilter(AllWaresFilter());
}

WareGroup * WareCatalog::getWareGroupByCode(const string& group_code) {
    return (WareGroup*)group_catalog.getObject(ObjectFilterByCode(group_code, "WareGroup"));
}

WareGroup * WareCatalog::getWareGroupById(int group_id) {
    return (WareGroup*)group_catalog.getObject(ObjectFilterById(group_id, "WareGroup"));
}

vector<WareGroup*> WareCatalog::getGroupList() {
    return getGroupSubGroups(-1);
}

vector<WareGroup*> WareCatalog::getGroupSubGroups(const string& parent_group_code) {
    WareGroup * parent_group = (WareGroup*)group_catalog.getObject(ObjectFilterByCode(parent_group_code, "WareGroup"));
    return getGroupSubGroups(parent_group->getOID());
}

vector<WareGroup*> WareCatalog::getGroupSubGroups(int parent_group_id) {
    vector<Object*> groups_object = group_catalog.getObjects(GroupSubItemFilter(parent_group_id));
    vector<WareGroup*> root_groups;
    for (vector<Object*>::iterator it = groups_object.begin(); it != groups_object.end(); it++) {
        root_groups.push_back( (WareGroup*) (*it) );
    }
    return root_groups;
}

void WareCatalog::addGroup(WareGroup* new_group) {
    group_catalog.addObject(new_group, ObjectFilterByCode(new_group->getCode(), "WareGroup"));
}

void WareCatalog::dropGroupSubGroups(int parent_group_id) {
    group_catalog.dropObjectByFilter(GroupSubItemFilter(parent_group_id));
}

void WareCatalog::clearGroupInWares(int group_id) {
    vector<Object*> ware_in_group = ware_catalog.getObjects(GroupSubWareFilter(group_id));
    for (vector<Object*>::iterator it = ware_in_group.begin(); it != ware_in_group.end(); it++ ) {
        ((Ware*)(*it))->setWareGroup(NULL);
        ((Ware*)(*it))->update();
    }
}

void WareCatalog::dropWareFromGroup(int group_id) {
    ware_catalog.dropObjectByFilter(GroupSubWareFilter(group_id));
}

//---(*)---Zuskin---20/02/2012---
//void WareCatalog::updateSaleRestrictionByTime(const string& start_time, const string& count_time) {
void WareCatalog::updateSaleRestrictionByTime(const string& start_time, const string& count_time, int id) {
    //ResultSet rs = RDBOperation::getInstance()->getFromTable("sale_time_limit", EmptyObjectFilter(""));
    ResultSet rs = RDBOperation::getInstance()->getFromTable("sale_time_limit", ObjectFilterById(id, "TaxRateGr")); // TaxRateGr because of id field name = 'id'
    //printf("rs.getCountRecord()= %d\n", rs.getCountRecord());
    if ( rs.getCountRecord() > 0 ) {
        //printf("updating record with id=%d\n", id);
        RDBUpdateExpression rdb_expr;
        rdb_expr.addField("start_time", start_time);
        rdb_expr.addField("count_time", count_time);
        //---(+)---
        rdb_expr.where_filter.addWhereCondition("id", "=", id);
        //---------
        
        RDBOperation::getInstance()->updateToDB("sale_time_limit", rdb_expr);
    } else {
        //printf("inserting record with id=%d\n", id);
        RDBInsertExpression rdb_expr;
        rdb_expr.addField("start_time", start_time);
        rdb_expr.addField("count_time", count_time);
        //---(+)---
        rdb_expr.addField("id", id);
        //---------
        RDBOperation::getInstance()->insertToDB("sale_time_limit", rdb_expr);
    }
    
}
//-------------------------------
