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

#include "AuthorizationSystem.h"
#include "ObjectFilter.h"
#include "ObjectFilters.h"
#include "Error.h"


void AuthorizationSystem::init() {
    addAdminProfile();
    addAdminUser();
}

void AuthorizationSystem::addProfile(const UserProfileInfo& profile_info) {
    if ( profile_info.profile_code == 1 )
        throw Error(221, "Профиль администратора не может быть изменен или удален. В систему он добавляется автоматически");

    UserProfile * profile = new UserProfile();
    profile->replaceInfo(profile_info);
    profiles.addObject(profile, ObjectFilterById(profile_info.profile_code, "Profile"));
    delete profile;    
}

void AuthorizationSystem::addUser(const UserInfo& user_info) {
    User * user = new User();
    if ( user_info.code == 1 ) {
        UserInfo admin_user_info = user_info;
        admin_user_info.profile_id = 1;
        user->replaceInfo(admin_user_info);
    } else {
        user->replaceInfo(user_info);
    }    
    users.addObject(user, ObjectFilterById(user_info.code, "User"));
    delete user;
}

void AuthorizationSystem::clearUsers() {
    users.dropObjectByFilter(AllUsersWithoutAdmin());
}

void AuthorizationSystem::clearAuthorizationSystem() {
    users.dropObjectByFilter(AllUsersWithoutAdmin());
    profiles.dropObjectByFilter(AllProfilesWithoutAdmin());
}

UserProfile * AuthorizationSystem::getProfileByDefault() {
    return getProfileById(1);
}

UserProfile * AuthorizationSystem::getProfileById(int profile_code) {
    return (UserProfile*)profiles.getObject(ObjectFilterById(profile_code, "Profile"));
}

void AuthorizationSystem::addAdminProfile() {
    try {
        profiles.getObject(ObjectFilterById(1, "Profile"));
    } catch (const Error & err) {
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
            createAdminProfile();
        else
            throw err;
    }
}

void AuthorizationSystem::addAdminUser() {
    try {
        users.getObject(ObjectFilterById(1, "User"));
    } catch (const Error & err) {
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
            createAdminUser();
        else
            throw err;
    }
}

void AuthorizationSystem::createAdminProfile() {
    UserProfile * admin_profile = new UserProfile();
    UserProfileInfo admin_profile_info;
    admin_profile_info.profile_code = 1;
    admin_profile_info.profile_name = "Администратор";
    for (int i = 0; i < RIGHTS_COUNT; i++)
        admin_profile_info.access_rigths[i] = true;
    admin_profile->replaceInfo(admin_profile_info);

    profiles.addObject(admin_profile, ObjectFilterById(admin_profile_info.profile_code, "Profile"));
}

void AuthorizationSystem::createAdminUser() {
    UserInfo admin_user_info;
    admin_user_info.code = 1;
    admin_user_info.long_name = "Администратор";
    admin_user_info.short_name = "Админ";
    admin_user_info.profile_id = 1;
    User * admin_user = new User();
    admin_user->replaceInfo(admin_user_info);
    users.addObject(admin_user, ObjectFilterById(admin_user_info.code, "User"));
}


