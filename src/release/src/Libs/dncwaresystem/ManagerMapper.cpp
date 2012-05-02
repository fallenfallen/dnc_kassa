/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include "ManagerMapper.h"

ManagerMapper::ManagerMapper() {
    to_db_table_name = "managers";
    from_db_table_name = "managers";
}


string ManagerMapper::getIdFieldName() {
    return MANAGER_ID_FIELD;
}

Object * ManagerMapper::createObjectByDefault() {
    Manager * manager = new Manager();
    return manager;
}

Object* ManagerMapper::createObjectFromResultRecord(const ResultRecord& record) {
    Manager * manager = new Manager();
    fromPersistent(manager, record);
    return manager;
}

void ManagerMapper::fromPersistent(Object* object, const ResultRecord& record) {

    Manager * manager = (Manager*)object;
    ManagerInfo manager_info;

    manager_info.code = fromString<int>(record.getValue( MANAGER_ID_FIELD ));
    manager_info.long_name = record.getValue(MANAGER_LONGNAME);
    manager_info.short_name = record.getValue(MANAGER_SHORTNAME);
    manager_info.barcode = record.getValue(MANAGER_BARCODE);
    manager_info.cardcode = record.getValue(MANAGER_CARDCODE);

    manager->setInfo(manager_info);
}

void ManagerMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    Manager * manager = (Manager*)object;
    ManagerInfo info = manager->getInfo();
        
    rdb_expression.addField(MANAGER_ID_FIELD, info.code);
    rdb_expression.addField(MANAGER_LONGNAME, info.long_name);
    rdb_expression.addField(MANAGER_SHORTNAME, info.short_name);
    rdb_expression.addField(MANAGER_BARCODE, info.barcode);
    rdb_expression.addField(MANAGER_CARDCODE, info.cardcode);
    
}
