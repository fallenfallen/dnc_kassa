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
#include "UserMapper.h"

UserMapper::UserMapper() {
    to_db_table_name = "users";
    from_db_table_name = "users";
}


string UserMapper::getIdFieldName() {
    return USER_ID_FIELD;
}

Object * UserMapper::createObjectByDefault() {
    User * user = new User();
    return user;
}

Object* UserMapper::createObjectFromResultRecord(const ResultRecord& record) {
    User * user = new User();
    fromPersistent(user, record);
    return user;
}

void UserMapper::fromPersistent(Object* object, const ResultRecord& record) {

    User * user = (User*)object;
    UserInfo user_info;

    user_info.code = fromString<int>(record.getValue( USER_ID_FIELD ));
    user_info.long_name = record.getValue(USER_LONGNAME);
    user_info.short_name = record.getValue(USER_SHORTNAME);
    user_info.profile_id = fromString<int>(record.getValue(USER_PROFILE_ID));
    user_info.barcode = record.getValue(USER_BARCODE);
    user_info.card_code = record.getValue(USER_CARDCODE);
    user_info.password = record.getValue(USER_PASSWORD);

    user->replaceInfo(user_info);
}

void UserMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    User * user = (User*)object;
    UserInfo info = user->getInfo();

    rdb_expression.addField(USER_ID_FIELD, info.code);
    rdb_expression.addField(USER_LONGNAME, info.long_name);
    rdb_expression.addField(USER_SHORTNAME, info.short_name);
    rdb_expression.addField(USER_BARCODE, info.barcode);
    rdb_expression.addField(USER_CARDCODE, info.card_code);
    rdb_expression.addField(USER_PASSWORD, info.password);

    rdb_expression.addField(USER_PROFILE_ID, info.profile_id);
    
}
