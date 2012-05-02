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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

extern SP sp;

void CashInSectionsForm::init() {
    grabKeyboard();  
    cashinsectionsTable->setColumnWidth(0, 80);
    cashinsectionsTable->setColumnWidth(1, 200);
    cashinsectionsTable->setColumnWidth(2, 150);  
    
    codec = QTextCodec::codecForName(DBCODEC);
    FillTable();
    cashinsectionsTable->setCurrentCell(0, 0);
}

void CashInSectionsForm::destroy() {
    releaseKeyboard();
}

void CashInSectionsForm::FillTable() {    
    cashinsectionsTable->setNumRows(sp.sections.size() + 1);
    double itog = 0;
    int i;
    for (i = 0; i < sp.sections.size(); i++) {
	cashinsectionsTable->setText(i, 0, QString::number(sp.sections[i].number));
	cashinsectionsTable->setText(i, 1, codec->toUnicode(sp.sections[i].name.c_str()));
	cashinsectionsTable->setText(i, 2, QString::number(sp.sections[i].cash, 'f', 2));
	itog += sp.sections[i].cash;	
    } 
    cashinsectionsTable->setText(i, 1, codec->toUnicode("Итого: "));
    cashinsectionsTable->setText(i, 2, QString::number(itog, 'f', 2));
}

void CashInSectionsForm::keyPressEvent(QKeyEvent * k) 
{
    switch(k->key()) {
    case Qt::Key_Space :	
	accept();
	break;
    case Qt::Key_F12 :
	reject();
	break;    
    case Qt::Key_Enter :	
	accept();
	break;
    case Qt::Key_Return :
	accept();
	break;    
    case Qt::Key_Escape :	
	reject();
	break;
    case Qt::Key_Up :	
	setPrevCurrentSection();
	break;
    case Qt::Key_Down :	
	setNextCurrentSection();
	break;	
    }
}

void CashInSectionsForm::setNextCurrentSection() {
    int current_section = cashinsectionsTable->currentRow();
    if ( ++current_section < cashinsectionsTable->numRows() )
	cashinsectionsTable->setCurrentCell(current_section, 0);
}

void CashInSectionsForm::setPrevCurrentSection() {
    int current_section = cashinsectionsTable->currentRow();
    if ( --current_section > -1 ) 
	cashinsectionsTable->setCurrentCell(current_section, 0);
}

void CashInSectionsForm::okPushButton_clicked()
{
    accept();
}

double CashInSectionsForm::GetResultSum() {
    int current_row = cashinsectionsTable->currentRow();
    return cashinsectionsTable->text(current_row, 2).toDouble();
}

int CashInSectionsForm::GetSelectedSection() {
    if ( cashinsectionsTable->currentRow() == sp.sections.size() )
	return 0;
    return cashinsectionsTable->currentRow();  
}

void CashInSectionsForm::setOperationText(const QString & text) {
    operationtextTextLabel->setText( codec->toUnicode(text) );
}



