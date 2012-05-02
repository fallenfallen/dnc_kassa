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

#include "FixedDiscount.h"

FixedDiscountInfo::FixedDiscountInfo() {
    range = POSITION_DISCOUNT;
    type = PERCENT_REDUCTION;
    rate = 0;
    visible = true;
    //-----(+)-----Zuskin-----19/07/2011-----
    summ = 0;
    scheme_id = 0;
    maxbonuspercent = 100;
    //---------------------------------------
}

FixedDiscount::FixedDiscount() {
    this->class_name = "FixedDiscount";
}

void FixedDiscount::setInfo(const FixedDiscountInfo& discount_info) {
    info = discount_info;
}

FixedDiscountInfo FixedDiscount::getInfo() {
    return info;
}

void FixedDiscount::copyInfo(Object* obj) {
    FixedDiscount * fdiscount = (FixedDiscount*)obj;
    this->setInfo(fdiscount->getInfo());
}

Object * FixedDiscount::createCopyObject() {
    FixedDiscount * copy_object = new FixedDiscount();
    copy_object->setInfo(this->getInfo());
    return copy_object;
}

string FixedDiscount::getCode() const {
    return info.code;
}