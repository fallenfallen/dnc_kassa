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
#include "Section.h"

SectionInfo::SectionInfo() {
    number = 0;
    is_fiscal = false;
    cash = 0;
}


Section::Section() {
    this->class_name = "Section";
}

SectionInfo Section::getInfo() {
    return info;
}

void Section::replaceInfo(const SectionInfo& section_info) {
    this->info = section_info;
}

void Section::copyInfo(Object* obj) {
    Section * orig_object = (Section*) obj;
    this->replaceInfo(orig_object->getInfo());
}

int Section::getNumber() const {
    return info.number;
}

void Section::setNumber(int s_number) {
    info.number = s_number;
}

string Section::getName() const {
    return info.name;
}

void Section::setName(const string& s_name) {
    info.name = s_name;
}

void Section::setHeadText(const string& head_text) {
    info.head_text = head_text;
}

void Section::setBottomText(const string& bottom_text) {
    info.bottom_text = bottom_text;
}

void Section::setFiscal(bool fiscal) {
    info.is_fiscal = fiscal;
}

bool Section::isFiscal() const {
    return info.is_fiscal;
}

bool Section::getAutoEncashment() {
    return info.auto_encashment;
}

void Section::setAutoEncashment(bool auto_encashment) {
    info.auto_encashment = auto_encashment;
}

double Section::getCashSum() const{
    return Round(info.cash*100)/100;
}

void Section::addCashSum(double sum) {
    //-----(*)-----Zuskin-----28/12/2011-----
    //printf("void Section::addCashSum(%2d)\n", sum);
    //info.cash += sum;
    info.cash += Round(sum*100)/100;
    info.cash = Round(info.cash*100)/100;
    //printf("info.cash = %2d\n", info.cash);
    //---------------------------------------
    this->update();
}

void Section::deductCashSum(double sum) {
    //-----(*)-----Zuskin-----28/12/2011-----
    //info.cash -= sum;
    info.cash -= Round(sum*100)/100;
    info.cash = Round(info.cash*100)/100;
    //---------------------------------------
    this->update();
}

void Section::zeroCashSum() {
    if ( info.auto_encashment == false )
	return;
    info.cash = 0;
    this->update();
}

