/* 
 * File:   SectionCatalog.h
 * Author: shonda
 *
 * Created on 1 Февраль 2011 г., 16:00
 */

#ifndef SECTIONCATALOG_H
#define	SECTIONCATALOG_H


#include "ObjectCatalog.h"
#include "Section.h"
#include <vector>
#include <string>

using namespace std;

class SectionCatalog {
public:
    Section* getFiscalSection();
    vector<Section*> getNotFiscalSections();

    vector<Section*> getAllSections();

    void addNewSection(Section * section);
    void editSection(Section * section, Section * new_data);
    void dropSection(int section_number);

    Section * createSectionByDefault();
    Section * getSectionByNumber(int section_number);

    void setFiscalSection(int section_number);

private:
    ObjectCatalog sections;

    // выбирает фискальную секцию из нефискальных
    Section* selectFiscalSection();
    void updateSection(Section * editing_section, Section * new_data);
};

#endif	/* SECTIONCATALOG_H */

