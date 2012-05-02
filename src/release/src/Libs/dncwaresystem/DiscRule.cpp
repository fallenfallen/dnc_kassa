/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savrasov Alexander aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Саврасов Александр aka Zuskin.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "DiscRule.h"

DiscRuleInfo::DiscRuleInfo() {
    value = 0;
    type = 0;
    scheme = 0;
}

DiscRule::DiscRule() {
    this->class_name = "DiscRule";
}

void DiscRule::setInfo(const DiscRuleInfo& dsc_info) {
    info = dsc_info;
}

DiscRuleInfo DiscRule::getInfo() {
    return info;
}

void DiscRule::copyInfo(Object* obj) {
    DiscRule * dsc = (DiscRule*)obj;
    this->setInfo(dsc->getInfo());
}

Object * DiscRule::createCopyObject() {
    DiscRule * copy_object = new DiscRule();
    copy_object->setInfo(this->getInfo());
    return copy_object;
}

int DiscRule::getOID() const {
    return info.code;
}

int DiscRule::getSchemeId() const {
    return info.scheme;
}

string DiscRule::getLongTitle() const {
    return info.long_name;
}	

string DiscRule::getShortcut() const {
    return info.short_name;
}

double DiscRule::getValue() const {
    return info.value;
}

int DiscRule::getType() const {
    return info.type;
}

string DiscRule::getBDate() const {
    return info.bdate;
}

string DiscRule::getEDate() const {
    return info.edate;
}

string DiscRule::getBTime() const {
    return info.btime;
}

string DiscRule::getETime() const {
    return info.etime;
}

int DiscRule::getWeekdays() const {
    return info.weekdays;
}

double DiscRule::getSumCheck() const {
    return info.sumcheck;
}

double DiscRule::getAccumulation() const {
    return info.accumulation;
}

void DiscRule::setCode(const int code) {
    info.code = code;
}

void DiscRule::setSchemeId(const int schemeid) {
    info.scheme = schemeid;
}

void DiscRule::setLongTitle(const string & dsc_lttl) {
    info.long_name = dsc_lttl;
}
void DiscRule::setShortcut(const string & dsc_scut) {
    info.short_name = dsc_scut;
}

void DiscRule::setValue(const double val) {
    info.value = val;
}

void DiscRule::setType(const int type) {
    info.type = type;
}

void DiscRule::setBDate(const string & bdate) {
    info.bdate = bdate;
}

void DiscRule::setEDate(const string & edate) {
    info.edate = edate;
}

void DiscRule::setBTime(const string & btime) {
    info.btime = btime;
}

void DiscRule::setETime(const string & etime) {
    info.etime = etime;
}

void DiscRule::setWeekdays(const int wdays) {
    info.weekdays = wdays;
}

void DiscRule::setSumCheck(const double sum) {
    info.sumcheck = sum;
}

void DiscRule::setAccumulation(const double acc) {
    info.accumulation = acc;
}









