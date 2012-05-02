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

#include "UserProfile.h"

UserProfileInfo::UserProfileInfo() {
    access_rigths.insert(access_rigths.begin(), RIGHTS_COUNT, true);
}



UserProfile::UserProfile() {
    this->class_name = "Profile";
}

UserProfileInfo UserProfile::getInfo() {
    return info;
}

void UserProfile::replaceInfo(const UserProfileInfo& profile_info) {
    info = profile_info;
}

void UserProfile::copyInfo(Object* obj) {
    UserProfile * orig_obj = (UserProfile*) obj;
    this->replaceInfo(orig_obj->getInfo());
}

void UserProfile::setAccessRight(ProfileAccessRights right, bool r_value) {
    info.access_rigths[right] = r_value;
}

bool UserProfile::getAccessRight(ProfileAccessRights right) {
    return info.access_rigths[right];
}

