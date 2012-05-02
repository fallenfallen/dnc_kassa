/* 
 * File:   WareAddition.h
 * Author: shonda
 *
 * Created on 3 Февраль 2010 г., 11:31
 */

#ifndef _WAREADDITION_H
#define	_WAREADDITION_H

#include <map>
#include <string>

#include "Feature.h"
#include "FeatureValue.h"
#include "Object.h"
#include "ObjectCatalog.h"
#include "Barcode.h"

using namespace std;

class WareAdditionInfo {
public:
    string code;
    string article;
    double price;
    double quantity;
    int ware_id;

    WareAdditionInfo();
};


class WareAddition : public Object{
public:
    
    WareAddition();
    
    virtual string getCode() const;
    string getArticle() const;
    double getPrice() const;
    double getQuantity() const;

    WareAdditionInfo getInfo();
    void replaceInfo(const WareAdditionInfo & wa_info, bool replace_quantity = true);

    void copyInfo(Object * obj);

    string getMainBarcode();
    vector<Barcode*> getBarcodes();

    string getSelectedValue(Feature * feature);
    void setSelectedValueByCode(const string &value_code);
    
    virtual void clear();
    
private:
    WareAdditionInfo info;
    double quantity;
    
    ObjectCatalog descriptions;
    ObjectCatalog possible_value;
    string selectMainBarcode();

    void insertSelectedValue(FeatureValue * fvalue);
};

#endif	/* _WAREADDITION_H */

