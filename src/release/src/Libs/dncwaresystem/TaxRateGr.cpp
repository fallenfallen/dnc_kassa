/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Алексанр
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "TaxRateGr.h"
#include "TaxesCatalog.h"

TaxRateGrInfo::TaxRateGrInfo() {
}

TaxRateGr::TaxRateGr() {
    this->class_name = "TaxRateGr";
}

TaxRateGrInfo TaxRateGr::getInfo() {
    return info;
}

void TaxRateGr::setInfo(const TaxRateGrInfo& taxrategr_info) {
    info = taxrategr_info;
}

void TaxRateGr::copyInfo(Object* obj) {
    TaxRateGr * orig_obj = (TaxRateGr*) obj;
    this->setInfo(orig_obj->getInfo());
}

