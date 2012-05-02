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

#include "Barcode.h"

Barcode::Barcode() {
    this->class_name = "Barcode";
    coefficient = 1;
    main_barcode = false;
}

Barcode::Barcode(const string & barcode) {
    this->class_name = "Barcode";
    text = barcode;
    coefficient = 1;
    main_barcode = false;
}

string Barcode::getText() const {
    return text;
}
void Barcode::setText(const string& barcode_text) {
    text = barcode_text;
}

void Barcode::setWareInternalCode(const string& wcode) {
    ware_internal_code = wcode;
}

void Barcode::setAdditionCode(const string& addition_code) {
    this->addition_code = addition_code;
}

void Barcode::setCoefficient(double coeff) {
    this->coefficient = coeff;
}

void Barcode::setMainBarcode(bool is_main_barcode) {
    this->main_barcode = is_main_barcode;
}

string Barcode::getWareInternalCode() const {
    return ware_internal_code;
}

string Barcode::getAdditionCode() const {
    return addition_code;
}

double Barcode::getCoefficient() const {
    return coefficient;
}

bool Barcode::isMainBarcode() const {
    return main_barcode;
}

void Barcode::copyInfo(Object* obj) {
    Barcode* barcode_info = (Barcode*) obj;
    this->setAdditionCode(barcode_info->getAdditionCode());
    this->setCoefficient(barcode_info->getCoefficient());
    this->setMainBarcode(barcode_info->isMainBarcode());
    this->setText(barcode_info->getText());
    this->setWareInternalCode(barcode_info->getWareInternalCode());
}

