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
#include "UserProfileMapper.h"
#include <iostream>


UserProfileMapper::UserProfileMapper() {
    to_db_table_name = "profiles";
    from_db_table_name = "profiles";
}


string UserProfileMapper::getIdFieldName() {
    return PROFILE_ID;
}

Object * UserProfileMapper::createObjectByDefault() {
    UserProfile * prof = new UserProfile();
    return prof;
}

Object* UserProfileMapper::createObjectFromResultRecord(const ResultRecord& record) {
    UserProfile * prof = new UserProfile();
    fromPersistent(prof, record);
    return prof;
}

void UserProfileMapper::fromPersistent(Object* object, const ResultRecord& record) {

    UserProfile * profile = (UserProfile*)object;
    UserProfileInfo p_info;

    p_info.profile_code = fromString<int>(record.getValue( PROFILE_ID ));
    p_info.profile_name = record.getValue(PROFILE_NAME);

    p_info.access_rigths[AR_REGISTRATION_BY_CODE] = record.getValue(PROFILE_R_CODEREG) == "t";
    p_info.access_rigths[AR_REGISTRATION_BY_BARCODE] = record.getValue(PROFILE_R_SCANERREG) == "t";
    p_info.access_rigths[AR_REGISTRATION_BY_MANUAL_INPUT_BARCODE] = record.getValue(PROFILE_R_BARCODEMANUALREG) == "t";
    p_info.access_rigths[AR_REGISTRATION_BY_VISUAL_SEARCH] = record.getValue(PROFILE_R_VSREG) == "t";
    p_info.access_rigths[AR_REGISTRATION_CANCEL] = record.getValue(PROFILE_R_CANCELREG) == "t";
    p_info.access_rigths[AR_QUANTITY_EDITION] = record.getValue(PROFILE_R_QUANTITYEDITION) == "t";
    p_info.access_rigths[AR_PRICE_EDITION] = record.getValue(PROFILE_R_PRICEEDITION) == "t";
    p_info.access_rigths[AR_REGISTRATION_REPEAT] = record.getValue(PROFILE_R_REGISTRATION_REPEAT) == "t";
    p_info.access_rigths[AR_CHECK_CANCEL] = record.getValue(PROFILE_R_CHECK_CANCEL) == "t";
    p_info.access_rigths[AR_MANUAL_DISCOUNT] = record.getValue(PROFILE_R_MANUAL_DISCOUNT) == "t";
    p_info.access_rigths[AR_FIXED_DISCOUNT] = record.getValue(PROFILE_R_FIXED_DISCOUNT) == "t";
    p_info.access_rigths[AR_DISCOUNT_CANCEL] = record.getValue(PROFILE_R_DISCOUNT_CANCEL) == "t";
    p_info.access_rigths[AR_RETURN_BY_NUMBER] = record.getValue(PROFILE_R_RETURN_BY_NUMBER) == "t";
    p_info.access_rigths[AR_RETURN_MANUAL] = record.getValue(PROFILE_R_RETURN_MANUAL) == "t";
    p_info.access_rigths[AR_CASH_PAYMENT] = record.getValue(PROFILE_R_CASH_PAYMENT) == "t";
    p_info.access_rigths[AR_CREDIT_PAYMENT] = record.getValue(PROFILE_R_CREDIT_PAYMENT) == "t";
    p_info.access_rigths[AR_CONT_PAYMENT] = record.getValue(PROFILE_R_CONT_PAYMENT) == "t";
    p_info.access_rigths[AR_Z_REPORT] = record.getValue(PROFILE_R_Z_REPORT) == "t";
    p_info.access_rigths[AR_OTHER_REPORT] = record.getValue(PROFILE_R_OTHER_REPORT) == "t";    
    p_info.access_rigths[AR_KKM_SYNCHRONIZATION] = record.getValue(PROFILE_R_KKM_SYNCHRONIZATION) == "t";
    p_info.access_rigths[AR_ENTERING] = record.getValue(PROFILE_R_ENTERING) == "t";
    p_info.access_rigths[AR_GETTING] = record.getValue(PROFILE_R_GETTING) == "t";
    p_info.access_rigths[AR_CHECK_COPY] = record.getValue(PROFILE_R_CHECK_COPY) == "t";
    p_info.access_rigths[AR_OPEN_BOX] = record.getValue(PROFILE_R_OPEN_BOX) == "t";
    p_info.access_rigths[AR_PROGRAMM_SETUP] = record.getValue(PROFILE_R_PROGRAMM_SETUP) == "t";
    p_info.access_rigths[AR_HANDLE_UNLOAD] = record.getValue(PROFILE_R_HANDLE_UNLOAD) == "t";
    p_info.access_rigths[AR_HANDLE_UPLOAD] = record.getValue(PROFILE_R_HANDLE_UPLOAD) == "t";
    p_info.access_rigths[AR_PS_RECONSILIATION] = record.getValue(PROFILE_R_PS_RECONSILIATION) == "t";
    

    profile->replaceInfo(p_info);
}

void UserProfileMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    UserProfile * profile = (UserProfile*)object;
    UserProfileInfo p_info = profile->getInfo();
    
    rdb_expression.addField(PROFILE_ID, p_info.profile_code);
    rdb_expression.addField(PROFILE_NAME, p_info.profile_name);
    
    rdb_expression.addField(PROFILE_R_CODEREG, p_info.access_rigths[AR_REGISTRATION_BY_CODE] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_SCANERREG, p_info.access_rigths[AR_REGISTRATION_BY_BARCODE] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_BARCODEMANUALREG, p_info.access_rigths[AR_REGISTRATION_BY_MANUAL_INPUT_BARCODE] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_VSREG, p_info.access_rigths[AR_REGISTRATION_BY_VISUAL_SEARCH] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CANCELREG, p_info.access_rigths[AR_REGISTRATION_CANCEL] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_QUANTITYEDITION, p_info.access_rigths[AR_QUANTITY_EDITION] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_PRICEEDITION, p_info.access_rigths[AR_PRICE_EDITION] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_REGISTRATION_REPEAT, p_info.access_rigths[AR_REGISTRATION_REPEAT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CHECK_CANCEL, p_info.access_rigths[AR_CHECK_CANCEL] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_MANUAL_DISCOUNT, p_info.access_rigths[AR_MANUAL_DISCOUNT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_FIXED_DISCOUNT, p_info.access_rigths[AR_FIXED_DISCOUNT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_AUTO_DISCOUNT_CANCEL, "t");
    rdb_expression.addField(PROFILE_R_DISCOUNT_CANCEL, p_info.access_rigths[AR_DISCOUNT_CANCEL] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_RETURN_BY_NUMBER, p_info.access_rigths[AR_RETURN_BY_NUMBER] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_RETURN_MANUAL, p_info.access_rigths[AR_RETURN_MANUAL] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CASH_PAYMENT, p_info.access_rigths[AR_CASH_PAYMENT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CREDIT_PAYMENT, p_info.access_rigths[AR_CREDIT_PAYMENT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CONT_PAYMENT, p_info.access_rigths[AR_CONT_PAYMENT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_PAYMENT_TYPE4, "t");
    rdb_expression.addField(PROFILE_R_Z_REPORT, p_info.access_rigths[AR_Z_REPORT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_OTHER_REPORT, p_info.access_rigths[AR_OTHER_REPORT] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_KKM_SYNCHRONIZATION, p_info.access_rigths[AR_KKM_SYNCHRONIZATION] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_ENTERING, p_info.access_rigths[AR_ENTERING] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_GETTING, p_info.access_rigths[AR_GETTING] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_CHECK_COPY, p_info.access_rigths[AR_CHECK_COPY] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_OPEN_BOX, p_info.access_rigths[AR_OPEN_BOX] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_PROGRAMM_SETUP, p_info.access_rigths[AR_PROGRAMM_SETUP] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_HANDLE_UNLOAD, p_info.access_rigths[AR_HANDLE_UNLOAD] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_HANDLE_UPLOAD, p_info.access_rigths[AR_HANDLE_UPLOAD] ? "t" : "f");
    rdb_expression.addField(PROFILE_R_PS_RECONSILIATION, p_info.access_rigths[AR_PS_RECONSILIATION] ? "t" : "f");
    
}

