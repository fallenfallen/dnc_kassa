/* 
 * File:   UserProfileMapper.h
 * Author: shonda
 *
 * Created on 25 Апрель 2011 г., 16:04
 */

#ifndef USERPROFILEMAPPER_H
#define	USERPROFILEMAPPER_H

#include "RDBMapper.h"
#include "UserProfile.h"

#include <string>

using namespace std;

// field name in DB

const string PROFILE_ID = "profile_id";
const string PROFILE_NAME = "profile_name";

const string PROFILE_R_CODEREG = "intcode";
const string PROFILE_R_SCANERREG = "barcode_scaner";
const string PROFILE_R_BARCODEMANUALREG = "barcode_manual";
const string PROFILE_R_VSREG = "visual_search";
const string PROFILE_R_CANCELREG = "storno";
const string PROFILE_R_QUANTITYEDITION = "quant_reduction";
const string PROFILE_R_PRICEEDITION = "price_reduction";
const string PROFILE_R_REGISTRATION_REPEAT = "repeate_pos";
const string PROFILE_R_CHECK_CANCEL = "cancel_check";
const string PROFILE_R_MANUAL_DISCOUNT = "red_manual";
const string PROFILE_R_FIXED_DISCOUNT = "red_fixed";
const string PROFILE_R_AUTO_DISCOUNT_CANCEL = "red_cancel_auto";
const string PROFILE_R_DISCOUNT_CANCEL = "red_cancel_other";
const string PROFILE_R_RETURN_BY_NUMBER = "return_by_number";
const string PROFILE_R_RETURN_MANUAL = "open_return_check";
const string PROFILE_R_CASH_PAYMENT = "payment_nal";
const string PROFILE_R_CREDIT_PAYMENT = "payment_type2 ";
const string PROFILE_R_CONT_PAYMENT = "payment_type3";
const string PROFILE_R_PAYMENT_TYPE4 = "payment_type4";
const string PROFILE_R_Z_REPORT = "service_report_z";
const string PROFILE_R_OTHER_REPORT = "service_report_x";
const string PROFILE_R_KKM_SYNCHRONIZATION = "service_sync_time";
const string PROFILE_R_ENTERING = "entering";
const string PROFILE_R_GETTING = "getting";
const string PROFILE_R_CHECK_COPY = "copy_check";
const string PROFILE_R_OPEN_BOX = "open_box";
const string PROFILE_R_PROGRAMM_SETUP = "setup";
const string PROFILE_R_HANDLE_UNLOAD = "handle_unload";
const string PROFILE_R_HANDLE_UPLOAD = "handle_upload";
const string PROFILE_R_PS_RECONSILIATION = "ps_reconsiliation";


class UserProfileMapper : public RDBMapper{
public:
    UserProfileMapper();
    virtual string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* USERPROFILEMAPPER_H */

