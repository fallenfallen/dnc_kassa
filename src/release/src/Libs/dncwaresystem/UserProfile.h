/* 
 * File:   UserProfile.h
 * Author: shonda
 *
 * Created on 25 Апрель 2011 г., 14:31
 */

#ifndef USERPROFILE_H
#define	USERPROFILE_H

#include <string>
#include <vector>
#include "Object.h"

using namespace std;

const int RIGHTS_COUNT = 28;

enum ProfileAccessRights {
    AR_REGISTRATION_BY_CODE = 0,
    AR_REGISTRATION_BY_BARCODE = 1,
    AR_REGISTRATION_BY_MANUAL_INPUT_BARCODE = 2,
    AR_REGISTRATION_BY_VISUAL_SEARCH = 3,
    AR_REGISTRATION_CANCEL = 4,
    AR_QUANTITY_EDITION = 5,
    AR_PRICE_EDITION = 6,
    AR_REGISTRATION_REPEAT = 7,
    AR_CHECK_CANCEL = 8,
    AR_MANUAL_DISCOUNT = 9,
    AR_FIXED_DISCOUNT = 10,
    AR_DISCOUNT_CANCEL = 11,
    AR_RETURN_BY_NUMBER = 12,
    AR_RETURN_MANUAL = 13,
    AR_CASH_PAYMENT = 14,
    AR_CREDIT_PAYMENT = 15,
    AR_CONT_PAYMENT = 16,
    AR_Z_REPORT = 17,
    AR_OTHER_REPORT = 18,
    AR_KKM_SYNCHRONIZATION = 19,
    AR_ENTERING = 20,
    AR_GETTING = 21,
    AR_CHECK_COPY = 22,
    AR_OPEN_BOX = 23,
    AR_PROGRAMM_SETUP = 24,
    AR_HANDLE_UNLOAD = 25,
    AR_HANDLE_UPLOAD = 26,
    AR_PS_RECONSILIATION = 27
};


class UserProfileInfo {
public:
    int profile_code;  // == profile_id 
    string profile_name;
    vector<bool> access_rigths;

    UserProfileInfo();
};

class UserProfile : public Object {
public:
    UserProfile();

    UserProfileInfo getInfo();
    void replaceInfo(const UserProfileInfo & profile_info);

    void copyInfo(Object * obj);

    void setAccessRight(ProfileAccessRights right, bool r_value);
    bool getAccessRight(ProfileAccessRights right);
    
private:
    UserProfileInfo info;
};

#endif	/* USERPROFILE_H */

