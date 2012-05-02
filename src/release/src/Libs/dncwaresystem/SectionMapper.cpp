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

#include <iostream>
#include "SectionMapper.h"
#include "PersistentFacade.h"

SectionMapper::SectionMapper() {
    to_db_table_name = "sections";
    from_db_table_name = "sections";
}

string SectionMapper::getIdFieldName() {
    return SECTION_ID_FIELD;
}

Object * SectionMapper::createObjectByDefault() {
    Section * section = new Section();

    SectionInfo new_section_info;
    MaximumFilter max_filter(SECTION_NUMBER, "Section");
    new_section_info.number = ((int)PersistentFacade::getInstance()->getMaximum(max_filter)) + 1;
    new_section_info.is_fiscal = false;
    new_section_info.name = "Новая секция";
    new_section_info.head_text = "Заголовок новой секции";
    new_section_info.bottom_text = "Нижняя подпись новой секции";
    new_section_info.cash = 0;
    section->replaceInfo(new_section_info);
    
    return section;
}

Object* SectionMapper::createObjectFromResultRecord(const ResultRecord& record) {
    Section * section = new Section();
    fromPersistent(section, record);
    return section;
}

void SectionMapper::fromPersistent(Object* object, const ResultRecord& record) {
    Section * section = (Section*) object;
    SectionInfo info;
    
    info.number = fromString<int>(record.getValue(SECTION_NUMBER));
    info.name = record.getValue(SECTION_NAME);
    info.head_text = record.getValue(SECTION_HEAD_TEXT);
    info.bottom_text = record.getValue(SECTION_BOTTOM_TEXT);
    info.is_fiscal = record.getValue(SECTION_IS_FISCAL) == "t"  ? true : false;
    info.cash = Round(fromString<double>(record.getValue(SECTION_CASH))*100)/100;
    //printf("void SectionMapper::fromPersistent() info.cash = %2f\n", info.cash);
    info.auto_encashment = record.getValue(SECTION_AUTO_ENCASHMENT) == "t"  ? true : false;
    
    section->replaceInfo(info);
}

void SectionMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    Section * section = (Section*)object;
    SectionInfo info = section->getInfo();

    rdb_expression.addField(SECTION_NUMBER, info.number);
    rdb_expression.addField(SECTION_NAME, info.name);
    rdb_expression.addField(SECTION_HEAD_TEXT, info.head_text);
    rdb_expression.addField(SECTION_BOTTOM_TEXT, info.bottom_text);
    rdb_expression.addField(SECTION_IS_FISCAL, info.is_fiscal ? "t" : "f");
    rdb_expression.addField(SECTION_CASH, Round(info.cash*100)/100);
    rdb_expression.addField(SECTION_AUTO_ENCASHMENT, info.auto_encashment ? "t" : "f");
    //printf("void SectionMapper::toPersistent() info.cash = %2f\n", Round(info.cash*100)/100);
    
}
