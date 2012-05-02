/* 
 * File:   Ware.h
 * Author: shonda
 *
 * Created on 1 Февраль 2010 г., 18:37
 */

#ifndef _WARE_H
#define	_WARE_H

#include <iostream>
#include <string>
#include <vector>

#include "WareAddition.h"
#include "FeaturesSet.h"
#include "ObjectCatalog.h"
#include "WareFlags.h"
#include "WareGroup.h"

using namespace std;

class WareInfo {
public:
    string internal_code;
    string title;
    string text;
    double price;
    double quantity;
    string article;
    double min_price;
    string expiration; // срок годности в формате '12.11.2010 16:74:45'
    WareFlags flags;
    int section_num;
    //---(*)---Zuskin---20/02/2012---
    //bool sale_limit_by_time;
    int sale_limit_by_time;
    //-------------------------------

    string features_set_code;
    int features_set_id;
    int group_id;
    int taxgr_id;

    WareInfo();
};


class Ware : public Object {
public:
    Ware();
    //Ware(const string& internal_code); // ??

    virtual string getCode() const;
    virtual void setCode(const string & code); // ??

    WareInfo getInfo();
    void replaceInfo(const WareInfo & ware_info, bool replace_quantity = true);
    void copyInfo(Object * obj);

    double getUnallotedQuantity();
    double getAllRestQuantity();
    void addQuantity(double ware_quantity);

    string getMainBarcode(); //пока пустая строка

    bool isUseFeaturesSet();
    void setFeaturesSet(FeaturesSet * fset_point);
    FeaturesSet * getFeaturesSet();
    void assignFeaturesSet(const string & fset_code);
    void assignFeaturesSet(int fset_id);
    int getFeaturesSetId();  
    
    vector<Feature*> getFeatures();

    void assignGroup(int group_id);
    void setWareGroup(WareGroup * ware_group);
    WareGroup * getWareGroup();
    int getGroupId();

    int getTaxGroupId(); // пока -1

    vector<WareAddition*> getAdditions();
    void addAddition(WareAddition * addition, bool replace_quantity = true);
    WareAddition * getAdditionByCode(const string &code);

    virtual void clear();
    virtual void clearAdditions();

private:
    double base_quantity;
    WareInfo info;
    
    FeaturesSet * features_set;
    WareGroup * group;
    //TaxGroup tax_group;
    
    ObjectCatalog additions;
    
    void init();
    void replaceExistAddition(WareAddition * addition, bool replace_quantity);
    void addNewAddition(WareAddition * additions);
};



#endif	/* _WARE_H */
