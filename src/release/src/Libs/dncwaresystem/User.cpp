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

#include "User.h"
#include "AuthorizationSystem.h"
#include "WareSystem.h"

UserInfo::UserInfo() {
    profile_id = 1;
}



User::User() {
    this->class_name = "User";
}

UserInfo User::getInfo() {
    return info;
}

void User::replaceInfo(const UserInfo& user_info) {
    assignProfile(user_info.profile_id);
    info = user_info;
}

void User::assignProfile(int profile_id) {
    AuthorizationSystem * as = WareSystem::getInstance()->getAuthorizationSystem();
    profile = as->getProfileById(profile_id);
    info.profile_id = profile_id;
}

void User::copyInfo(Object* obj) {
    User * orig_obj = (User*) obj;
    this->replaceInfo(orig_obj->getInfo());
}

