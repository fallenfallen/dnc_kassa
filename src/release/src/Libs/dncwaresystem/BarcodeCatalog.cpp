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

#include "BarcodeCatalog.h"
#include "Error.h"

vector<Barcode*> BarcodeCatalog::getAdditionBarcode(const string& addition_code) {
    vector<Barcode*> result_barcodes;
    vector<Object*> objects = barcode_container.getObjects(AdditionBarcodeFilter(addition_code));
    for ( vector<Object*>::iterator it = objects.begin(); it != objects.end(); it++ ) {
        result_barcodes.push_back((Barcode*)(*it));
    }
    return result_barcodes;
}

vector<Barcode*> BarcodeCatalog::getWareBarcodes(const string& wcode) {
    //return barcode_container.getObjects(WareBarcodeFilter(wcode));
}

string BarcodeCatalog::getWareMainBarcode(const string& wcode) {
    return getMainBarcode(WareBarcodeFilter(wcode));
}

string BarcodeCatalog::getAdditionMainBarcode(const string& addition_code) {
    return getMainBarcode(AdditionBarcodeFilter(addition_code));
}

string BarcodeCatalog::getMainBarcode(const ObjectFilter& barcode_filter) {
    vector<Object*> exist_barcodes;
    try {
        exist_barcodes = barcode_container.getObjects(barcode_filter);
    } catch (const Error & err) {
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
            return string();
        else
            throw err;
    }

    if ( exist_barcodes.size() < 1 )
        return string();

    vector<Object*>::iterator barcode_it = exist_barcodes.begin();
    Barcode * barcode = (Barcode*) (*barcode_it);
    while ( barcode_it != exist_barcodes.end() && barcode->isMainBarcode() == false ) {
        barcode_it++;
        barcode = (Barcode*) (*barcode_it);
    }

    if ( barcode_it == exist_barcodes.end() )
        return ((Barcode*)exist_barcodes[0])->getText();
    else
        return barcode->getText();
}

void BarcodeCatalog::addBarcode(Barcode* barcode) {
    if (barcode->getText().empty())
        return;
    if ( barcode->getAdditionCode().empty() ) {
        if ( getWareMainBarcode(barcode->getWareInternalCode()) == "" )
            barcode->setMainBarcode(true);
        ExistWareBarcodeFilter filter(barcode->getWareInternalCode(), barcode->getText());
        barcode_container.addObject(barcode, filter);
    } else {
        ExistAdditionBarcodeFilter filter(barcode->getAdditionCode(), barcode->getText());
        barcode_container.addObject(barcode, filter);
    }
}

void BarcodeCatalog::dropAdditionBarcode(const string& addition_code) {
    barcode_container.dropObjectByFilter(AdditionBarcodeFilter(addition_code));
}

void BarcodeCatalog::dropWareBarcode(const string& ware_code) {
    barcode_container.dropObjectByFilter( WareBarcodeFilter(ware_code)  );
}

