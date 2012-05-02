/* 
 * File:   User.h
 * Author: shonda
 *
 * Created on 25 Апрель 2011 г., 15:23
 */

#ifndef USER_H
#define	USER_H

#include <string>
#include "Object.h"
#include "UserProfile.h"

using namespace std;

class UserInfo {
public:
    int code; // = user_id
    string long_name;
    string short_name;
    string barcode;
    string card_code;
    string password;

    int profile_id;

    UserInfo();
};


class User : public Object{
public:
    User();

    UserInfo getInfo();
    void replaceInfo(const UserInfo & profile_info);
    
    void copyInfo(Object * obj);
    
private:
    UserInfo info;
    UserProfile * profile;
    
    void assignProfile( int profile_id );
};

#endif	/* USER_H */

