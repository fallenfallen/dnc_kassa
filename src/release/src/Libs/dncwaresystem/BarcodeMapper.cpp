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

#include "BarcodeMapper.h"

BarcodeMapper::BarcodeMapper() {
    //table_name = "ware_barcode";
    from_db_table_name = "ware_barcode";
    to_db_table_name = "ware_barcode";
}

string BarcodeMapper::getIdFieldName() {
    return BARCODE_ID_FIELD;
}

Object * BarcodeMapper::createObjectByDefault() {
    Barcode * barcode = new Barcode();
    return barcode;
}

Object * BarcodeMapper::createObjectFromResultRecord(const ResultRecord& record) {
    Barcode * barcode = new Barcode();
    fromPersistent(barcode, record);
    return barcode;
}

void BarcodeMapper::fromPersistent(Object* object, const ResultRecord& record) {
    Barcode * barcode = (Barcode*) object;
    barcode->setText(record.getValue(BARCODE_TEXT));
    barcode->setAdditionCode(record.getValue(BARCODE_ADDITION_CODE));
    barcode->setWareInternalCode(record.getValue(BARCODE_WARE_CODE));
    barcode->setCoefficient(fromString<double>(record.getValue(BARCODE_COEFFICIENT)));
    barcode->setMainBarcode((record.getValue(BARCODE_MAIN_BARCODE)) == "t");
}

void BarcodeMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    Barcode * barcode = (Barcode*) object;
    rdb_expression.addField(BARCODE_TEXT, barcode->getText());
    rdb_expression.addField(BARCODE_ADDITION_CODE, barcode->getAdditionCode());
    rdb_expression.addField(BARCODE_WARE_CODE, barcode->getWareInternalCode());
    rdb_expression.addField(BARCODE_COEFFICIENT, barcode->getCoefficient());
    rdb_expression.addField(BARCODE_MAIN_BARCODE, (barcode->isMainBarcode() ? "t" : "f"));
}