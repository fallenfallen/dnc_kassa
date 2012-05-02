/* 
 * File:   AuthorizationSystem.h
 * Author: shonda
 *
 * Created on 26 Апрель 2011 г., 12:52
 */

#ifndef AUTHORIZATIONSYSTEM_H
#define	AUTHORIZATIONSYSTEM_H


#include <string>
#include "ObjectCatalog.h"
#include "UserProfile.h"
#include "User.h"

using namespace std;

class AuthorizationSystem {
public:    
    void init();
    
    void addProfile(const UserProfileInfo & profile_info);
    void addUser(const UserInfo & user_info);

    // удалить всех пользователей кроме "1 - Администратор"
    void clearUsers();

    // удалить все профили кроме "1 - Администратор"
    void clearAuthorizationSystem();

    UserProfile * getProfileByDefault();
    UserProfile * getProfileById(int profile_code);

private:
    ObjectCatalog profiles;
    ObjectCatalog users;

    void addAdminProfile();
    void addAdminUser();
    void createAdminProfile();
    void createAdminUser();
};

#endif	/* AUTHORIZATIONSYSTEM_H */

