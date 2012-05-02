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

#include "SectionSetupForm.h"
#include "Decoder4.h"
#include <iostream>
#include <QMessageBox>

SectionSetupForm::SectionSetupForm() {
    widget.setupUi(this);

    widget.sectionsTableWidget->setColumnWidth(0, 110);
    widget.sectionsTableWidget->setColumnWidth(1, 200);
    widget.sectionsTableWidget->setColumnWidth(2, 110);

    connect(widget.addsectionPushButton, SIGNAL(clicked()), SLOT(addSection()));
    connect(widget.addsectionPushButton, SIGNAL(clicked()), SLOT(saveFiscalIndex()));
    
    connect(widget.changesectionPushButton, SIGNAL(clicked()), SLOT(changeSectionInfo()));
    connect(widget.dropsectionPushButton, SIGNAL(clicked()), SLOT(dropSection()));
    connect(widget.dropsectionPushButton, SIGNAL(clicked()), SLOT(saveFiscalIndex()));

    connect(widget.sectionsTableWidget, SIGNAL(itemSelectionChanged()), SLOT(selectSection()));

    connect(widget.fiscalsectionComboBox, SIGNAL(activated(int)), SLOT(setFiscalSection(int)));
    connect(widget.fiscalsectionComboBox, SIGNAL(activated(int)), SLOT(saveFiscalIndex()));

    connect(widget.sectionheadtextTextEdit, SIGNAL(textChanged()), SLOT(headTextChanged()));
    connect(widget.sectionbottomtextTextEdit, SIGNAL(textChanged()), SLOT(bottomTextChanged()));

    section_number_validator = new QIntValidator(0, 999, this);
    widget.sectionnumberLineEdit->setValidator(section_number_validator);
    
    //---(+)---Zuskin---15/02/2012---
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
    //-------------------------------

    try {
        WareSystem::startInstance();
        section_catalog = WareSystem::getInstance()->getSectionCatalog();
        loadSections();
    } catch (const Error & err) {
        //cout << "err.code - " << err.getErrorCode() << endl;
        if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND ) 
            {
            //---(-)---Zuskin---15/02/2012---
    	    //errorMessage("Не удалось загрузить список секций");
    	    //-------------------------------
    	    }
        else 
            catchError(err);
    }
}

SectionSetupForm::~SectionSetupForm() {
    delete section_number_validator;    
    try {
        WareSystem::finishInstance();
    } catch (const Error & err ) {
        catchError(err);
    }
}

void SectionSetupForm::catchError(const Error& err) {
    // error message
    cout << "Ошибка " << err.getErrorCode() << " " << err.getErrorUserText() << endl;
    cout << err.getErrorLogText() << endl;
    QString err_str = toForm("Ошибка № ") + toForm(err.getErrorCode()) + toForm("\n") + toForm(err.getErrorUserText());
    QMessageBox::critical(this, toForm("Ошибка"), err_str);
}

void SectionSetupForm::errorMessage(const char* err_text) {

    //---(*)---Zuskin---15/02/2012---
    //QMessageBox::critical(this, toForm("Ошибка"), toForm(err_text) );
    QMessageBox mbox(this);
    mbox.setIcon(QMessageBox::Critical);
    mbox.setWindowTitle(toForm("Ошибка"));
    mbox.setText(toForm(err_text));
    mbox.addButton(QMessageBox::Ok);
    
    if (this->isActiveWindow()==false) {
        mbox.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry()));
    }
    mbox.exec();
    //-------------------------------
}

void SectionSetupForm::loadSections() {
    current_fiscal_section_index = -1;
    vector<Section*> sections = section_catalog->getAllSections();
    for (int i=0; i < sections.size(); i++) {
        showAddedSection(sections[i]);
    }
    widget.fiscalsectionComboBox->setCurrentIndex(0);
    current_fiscal_section_index = 0;
}

void SectionSetupForm::showAddedSection(Section* added_section) {
    int row_num = widget.sectionsTableWidget->rowCount();
    widget.sectionsTableWidget->setRowCount( row_num + 1 );
    showSectionInTable(added_section, row_num);
    widget.sectionsTableWidget->selectRow(row_num);

    QString section_name_with_number = toForm(added_section->getNumber()) + QString(" ") +
            toForm(added_section->getName());
    widget.fiscalsectionComboBox->addItem( section_name_with_number );
}

void SectionSetupForm::showSectionInTable(Section* section, int row_num) {
    SectionInfo section_info = section->getInfo();
    widget.sectionsTableWidget->setItem( row_num, 0, new QTableWidgetItem( toForm(section_info.number) ) );
    widget.sectionsTableWidget->item(row_num, 0)->setTextAlignment(Qt::AlignCenter);
    widget.sectionsTableWidget->setItem( row_num, 1, new QTableWidgetItem( toForm(section_info.name) ) );
    if ( section_info.is_fiscal )
        widget.sectionsTableWidget->setItem( row_num, 2, new QTableWidgetItem( toForm("да") ) );
    else
        widget.sectionsTableWidget->setItem( row_num, 2, new QTableWidgetItem( toForm("нет") ) );
    widget.sectionsTableWidget->item(row_num, 2)->setTextAlignment(Qt::AlignCenter);
    widget.sectionsTableWidget->repaint(); 
}

void SectionSetupForm::addSection() {
    Section * new_section;
    try {
        new_section = section_catalog->createSectionByDefault();
        showAddedSection(new_section);
    } catch(const Error& err) {
        errorMessage("Не удалось добавить новую секцию");
        catchError(err);
        return;
    }    
}

void SectionSetupForm::dropSection() {
    //-----(+)-----Zuskin-----18/10/2011-----
    int select_row = widget.sectionsTableWidget->currentRow();
    //cout << "select row = " << select_row << endl;
    if ( select_row < 0 ) {
        errorMessage("Нет выделенной секции");
        return;
    }
    Section * tmpsection = section_catalog->getSectionByNumber(getSelectSectionNumber());
    if (tmpsection->isFiscal()) {
        vector<Section*> sections = section_catalog->getAllSections();
	if (sections.size() == 1) {
    	    QMessageBox msgBox((QWidget *)this);
	    msgBox.setWindowTitle(toForm("ВНИМАНИЕ!"));
	    msgBox.setText(toForm("Это фискальная секция!"));
	    msgBox.setInformativeText(toForm("Вы точно хотите удалить эту секцию?"));
	    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	    msgBox.setIcon(QMessageBox::Question);
	    msgBox.setDefaultButton(QMessageBox::No);
	    int state = msgBox.exec();
	    if (state == QMessageBox::No) return;
	}
	else {
	    QMessageBox msgBox((QWidget *)this);
	    msgBox.setWindowTitle(toForm("ВНИМАНИЕ!"));
	    msgBox.setText(toForm("Запрещено удалять фискальную секцию, если есть хотя бы одна нефискальная!"));
	    msgBox.setStandardButtons(QMessageBox::Ok);
	    msgBox.setIcon(QMessageBox::Warning);
	    int state = msgBox.exec();
	    return;	
	}    
    }
    //---------------------------------------

    try {
        section_catalog->dropSection(getSelectSectionNumber());
    } catch (const Error & err) {
        //errorMessage("Не удалось удалить текущюю секцию");
        catchError(err);
        return;
    } catch(const char* err_text) {
        errorMessage(err_text);
        return;
    }
    removeSelectRow();
}

void SectionSetupForm::removeSelectRow() {
    int current_row = widget.sectionsTableWidget->currentRow();
    if ( current_row == 0 )
        widget.sectionsTableWidget->selectRow(current_row + 1);
    if ( current_row > 0 )
        widget.sectionsTableWidget->selectRow(current_row - 1);
    widget.sectionsTableWidget->removeRow(current_row);

    widget.fiscalsectionComboBox->removeItem(current_row);
}

void SectionSetupForm::selectSection() {
    Section * select_section;
    try {
        select_section = getCurrentSection();
    } catch (const char * err_text) {
        return;
    }
    showSectionInfoInWidgets(select_section);
}

Section * SectionSetupForm::getCurrentSection() {
    try  {
        Section * select_section = section_catalog->getSectionByNumber(getSelectSectionNumber());
        return select_section;
    } catch (const Error & err)  {
        clearInfoWidgets();
        throw "Не удалось получить информацию о текущей секции";
    }
}

int SectionSetupForm::getSelectSectionNumber() {
    int select_row = widget.sectionsTableWidget->currentRow();
    //cout << "select row = " << select_row << endl;
    if ( select_row < 0 ) {
        clearInfoWidgets();
        throw "Нет выделенной секции";
    }
    int select_section_number = widget.sectionsTableWidget->item(select_row, 0)->text().toInt();
    //cout << "select_section_number " << select_section_number << endl;
    return select_section_number;
}

void SectionSetupForm::changeSectionInfo() {
    try {
        Section * select_section = getCurrentSection();
        Section * section_new_data = new Section();
        setToSectionFromWidgets(section_new_data);
        section_catalog->editSection(select_section, section_new_data);
        showChangedSection(select_section, widget.sectionsTableWidget->currentRow());
    } catch (const Error &err) {
        //errorMessage("Не удалось обновить информацию о текущей секции");
        catchError(err);
        selectSection();
    } catch(const char* err_text) {
        errorMessage(err_text);
        return;
    }
}

void SectionSetupForm::showChangedSection(Section* select_section, int row_num) {
    showSectionInTable(select_section, row_num);

    QString section_name_with_number = toForm(select_section->getNumber()) + QString(" ") + 
            toForm(select_section->getName());
    widget.fiscalsectionComboBox->setItemText(row_num, section_name_with_number);
}

void SectionSetupForm::setToSectionFromWidgets(Section* section) {
    section->setNumber(widget.sectionnumberLineEdit->text().toInt() );
    section->setName(fromForm(widget.sectionnameLineEdit->text() ).toStdString());
    section->setHeadText(fromForm( widget.sectionheadtextTextEdit->toPlainText() ).toStdString());
    section->setBottomText(fromForm( widget.sectionbottomtextTextEdit->toPlainText() ).toStdString());
    section->setAutoEncashment( widget.sectionautoencashmentCheckBox->isChecked() );
}

void SectionSetupForm::showSectionInfoInWidgets(Section* section) {
    SectionInfo section_info = section->getInfo();
    widget.sectionnumberLineEdit->setText( toForm(section_info.number) );
    widget.sectionnameLineEdit->setText( toForm(section_info.name) );
    widget.sectionheadtextTextEdit->setText( toForm(section_info.head_text) );
    widget.sectionbottomtextTextEdit->setText( toForm(section_info.bottom_text) );

    widget.sectionbottomtextTextEdit->setEnabled(!section_info.is_fiscal);
    widget.sectionbottomtextLabel->setEnabled(!section_info.is_fiscal);
    widget.sectionautoencashmentCheckBox->setChecked( section_info.auto_encashment );
    //-----(+)-----Zuskin-----2011-05-12-----
    widget.sectionautoencashmentCheckBox->setEnabled(!section_info.is_fiscal);
    //---------------------------------------
}

void SectionSetupForm::clearInfoWidgets() {
    widget.sectionnumberLineEdit->setText( "" );
    widget.sectionnameLineEdit->setText( "" );
    widget.sectionheadtextTextEdit->setText( "" );
    widget.sectionbottomtextTextEdit->setText( "" );
    widget.sectionautoencashmentCheckBox->setChecked(false);
}

void SectionSetupForm::setFiscalSection(int index) {

    int new_fiscal_section_number = widget.sectionsTableWidget->item(index, 0)->text().toInt();
    int old_fiscal_section_number = widget.sectionsTableWidget->item(current_fiscal_section_index, 0)->text().toInt();
    try {
        section_catalog->setFiscalSection( new_fiscal_section_number );
        showSectionInTable(section_catalog->getSectionByNumber(new_fiscal_section_number), index);
        showSectionInTable(section_catalog->getSectionByNumber(old_fiscal_section_number), current_fiscal_section_index);
        selectSection();
    } catch (const Error & err ) {
        catchError(err);
        widget.fiscalsectionComboBox->setCurrentIndex( current_fiscal_section_index );
    } catch ( const char * err_text ) {
        errorMessage(err_text);
        widget.fiscalsectionComboBox->setCurrentIndex( current_fiscal_section_index );
    }
}

void SectionSetupForm::saveFiscalIndex() {
    current_fiscal_section_index = widget.fiscalsectionComboBox->currentIndex();
}

void SectionSetupForm::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Enter:
        break;
    case Qt::Key_Return:
        break;
     default:
         QDialog::keyPressEvent(event);
     }
}

void SectionSetupForm::headTextChanged() {
    int SECTION_HEAD_TEXT_MAX_LEN = 200;
    QString head_text = widget.sectionheadtextTextEdit->toPlainText();
    if ( head_text.length() <= SECTION_HEAD_TEXT_MAX_LEN )
        return;
    head_text = head_text.left(SECTION_HEAD_TEXT_MAX_LEN);
    widget.sectionheadtextTextEdit->setText(head_text);

    widget.sectionheadtextTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SectionSetupForm::bottomTextChanged() {
    int SECTION_BOTTOM_TEXT_MAX_LEN = 200;
    QString bottom_text = widget.sectionbottomtextTextEdit->toPlainText();
    if ( bottom_text.length() <= SECTION_BOTTOM_TEXT_MAX_LEN )
        return;
    bottom_text = bottom_text.left(SECTION_BOTTOM_TEXT_MAX_LEN);
    widget.sectionbottomtextTextEdit->setText(bottom_text);

    widget.sectionbottomtextTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}
