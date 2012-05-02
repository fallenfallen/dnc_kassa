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

#include "SectionCatalog.h"
#include "ObjectFilters.h"
#include "PersistentFacade.h"
#include "SectionMapper.h"
#include "Error.h"

Section* SectionCatalog::getFiscalSection() {
    Section * fiscal_section;
    try {
        fiscal_section = (Section*)sections.getObject( FiscalSectionFilter() );
    } catch(const Error& err) {
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
            fiscal_section = selectFiscalSection();
    }
    return fiscal_section;
}

vector<Section*> SectionCatalog::getNotFiscalSections() {
    vector<Object*> loaded_sections = sections.getObjects( NotFiscalSectionFilter() );
    vector<Section*> result;
    for (int i =0; i < loaded_sections.size(); i++) 
        result.push_back( (Section*)loaded_sections[i] );
    return result;
}

vector<Section*> SectionCatalog::getAllSections() {
    vector<Section*> result;
    result.push_back( getFiscalSection() );
    vector<Section*> not_fiscal_sections = getNotFiscalSections();

    result.insert(result.end(), not_fiscal_sections.begin(), not_fiscal_sections.end());
    return result;
}

void SectionCatalog::addNewSection(Section* section) {
    sections.addObject(section, SectionByNumberFilter(section->getNumber()));
}

void SectionCatalog::editSection(Section* section, Section* new_data) {
    new_data->setFiscal(section->isFiscal());
    try {
        getSectionByNumber(new_data->getNumber());
        if ( new_data->getNumber() != section->getNumber() )
            throw Error(220, "Указанный номер секции уже используется!");
        else
            updateSection(section, new_data);
    } catch (const Error& err ) {
        if ( err.getErrorCode() != ERROR_OBJECT_NOT_FOUND )
            throw err;
        else
            updateSection(section, new_data);
    }
}

void SectionCatalog::updateSection(Section* editing_section, Section* new_data) {
    editing_section->copyInfo(new_data);
    editing_section->update();
}

void SectionCatalog::dropSection(int section_number) {
    //-----(-)-----Zuskin-----18/10/2011-----
    //Section * section = getSectionByNumber(section_number);
    //if (section->isFiscal())
    //    throw Error(219, "Фискальная секция не может быть удалена!");
    //---------------------------------------
    sections.dropObjectByFilter( SectionByNumberFilter(section_number) );
}

Section * SectionCatalog::getSectionByNumber(int section_number) {
    return (Section*) sections.getObject( SectionByNumberFilter(section_number) );
}

Section * SectionCatalog::createSectionByDefault() {
    Section * new_section = (Section*)PersistentFacade::getInstance()->createObjectByDefault("Section");
    
    try {
        getFiscalSection();
    } catch(const Error & err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND)
            new_section->setFiscal(true);
    }
    
    return (Section*)sections.addObject(new_section, SectionByNumberFilter(new_section->getNumber()));
}

Section * SectionCatalog::selectFiscalSection() {
    Section * section = (Section*)sections.getObject( NotFiscalSectionFilter() );
    section->setFiscal(true);
    section->update();
    return section;
}

void SectionCatalog::setFiscalSection(int section_number) {
    Section * current_fiscal_section = getFiscalSection();
    current_fiscal_section->setFiscal(false);
    current_fiscal_section->update();
    
    Section * new_fiscal_section = getSectionByNumber(section_number);
    new_fiscal_section->setFiscal(true);
    new_fiscal_section->update();
}



