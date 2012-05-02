/* 
 * File:   BarcodeCatalog.h
 * Author: shonda
 *
 * Created on 16 Сентябрь 2010 г., 18:48
 */

#ifndef BARCODECATALOG_H
#define	BARCODECATALOG_H

#include "Barcode.h"
#include "ObjectCatalog.h"
#include "ObjectFilters.h"

using namespace std;

class BarcodeCatalog {
public:
    vector<Barcode*> getWareBarcodes(const string &wcode);
    vector<Barcode*> getAdditionBarcode(const string &addition_code);

    string getWareMainBarcode(const string &wcode);
    string getAdditionMainBarcode(const string &addition_code);

    void addBarcode(Barcode *barcode);

    void dropAdditionBarcode(const string &addition_code);
    void dropWareBarcode(const string &ware_code);
    
private:
    ObjectCatalog barcode_container;

    string getMainBarcode(const ObjectFilter& barcode_filter);
};

#endif	/* BARCODECATALOG_H */

