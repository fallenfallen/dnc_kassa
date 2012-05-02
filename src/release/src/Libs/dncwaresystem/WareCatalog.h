/* 
 * File:   WareCatalog.h
 * Author: shonda
 *
 * Created on 10 Март 2010 г., 14:24
 */

#ifndef _WARECATALOG_H
#define	_WARECATALOG_H

#include "Ware.h"
#include "ObjectCatalog.h"
#include "WareGroup.h"

using namespace std;

class WareCatalog {
public:
    Ware * getWareByCode(const string & internal_code);
    void addWare(const WareInfo & ware_info, bool replace_quantity = true);

    void dropAllAdditions();
    void deleteFSFromAllWares();

    void clearCatalog();

    WareGroup * getWareGroupByCode(const string & group_code);
    WareGroup * getWareGroupById(int group_id);

    vector<WareGroup *> getGroupList();
    vector<WareGroup *> getGroupSubGroups(const string & parent_group_code);
    vector<WareGroup *> getGroupSubGroups(int parent_group_id);
    
    void addGroup(WareGroup * new_group);

    void dropGroupSubGroups(int parent_group_id);
    void clearGroupInWares(int group_id);
    void dropWareFromGroup(int group_id);

    //---(*)---Zuskin---20/02/2012---
    //void updateSaleRestrictionByTime(const string & start_time, const string & count_time);
    void updateSaleRestrictionByTime(const string & start_time, const string & count_time, int id);
    //-------------------------------

private:
    ObjectCatalog ware_catalog;
    ObjectCatalog group_catalog;
    
};

#endif	/* _WARECATALOG_H */

