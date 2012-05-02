/* 
 * File:   SectionSetupForm.h
 * Author: shonda
 *
 * Created on 1 Февраль 2011 г., 19:02
 */

#ifndef _SECTIONSETUPFORM_H
#define	_SECTIONSETUPFORM_H

#include "ui_SectionSetupForm.h"
#include <vector>
#include <string>
#include "Error.h"
#include "WareSystem.h"
#include <QKeyEvent>
#include <QValidator>
//---(+)---Zuskin---15/02/2012---
#include <QDesktopWidget>
//-------------------------------

using namespace std;

class SectionSetupForm : public QDialog {
    Q_OBJECT
public:
    SectionSetupForm();
    virtual ~SectionSetupForm();

public slots:
    void addSection();
    void dropSection();
    void changeSectionInfo();
    void selectSection();

    void setFiscalSection(int );
    void saveFiscalIndex();

    void keyPressEvent(QKeyEvent * event);

private slots:
    void headTextChanged();
    void bottomTextChanged();

private:
    Ui::SectionSetupForm widget;

    SectionCatalog * section_catalog;
    int current_fiscal_section_index;

    QValidator * section_number_validator;

    void loadSections();
    void catchError(const Error& err);
    void errorMessage(const char* err_text);

    void showAddedSection(Section* added_section);
    void showSectionInTable(Section* section, int row_num);
    void showChangedSection(Section* select_section, int row_num);

    Section * getCurrentSection();
    void showSectionInfoInWidgets(Section * section);
    int getSelectSectionNumber();
    void removeSelectRow();

    void setToSectionFromWidgets(Section* section);
    void clearInfoWidgets();
};

#endif	/* _SECTIONSETUPFORM_H */
