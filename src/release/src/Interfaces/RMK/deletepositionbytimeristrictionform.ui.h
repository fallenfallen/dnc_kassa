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
//---(+)---Zuskin---24/02/2012---
#include "reg_pgsql.h"
extern Registration Reg;      // Объект работы с базой данных по регистрации товаров
//------------------------------------------


void DeletePositionByTimeRistrictionForm::init() {
    positionTable->setColumnWidth(0, 120);
    positionTable->setColumnWidth(1, 300);
    positionTable->setColumnWidth(2, 150);
    DbCodec = QTextCodec::codecForName(DBCODEC);
    okPushButton->setFocus();
    
}

void DeletePositionByTimeRistrictionForm::setReceipt( Receipt * receipt )
{
    int count_row = 0;
    for (int i = 0; i < receipt->record_num; i++) {
	//---(*)---Zuskin---24/02/2012---
	//if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time) {
	if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time && Reg.getSaleAccessByTime(receipt->GetPosElemRef(i)->GetCode()) == false ) {
	//-----------------------------------------    
	    positionTable->setNumRows(count_row + 1);
	    //printf("receipt->GetPosElemRef(i)->GetLongtitle() = %s \n", receipt->GetPosElemRef(i)->GetLongtitle());
	    positionTable->setText(count_row, 0, DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetCode()) );
	    positionTable->setText(count_row, 1, DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetLongtitle()) );
	    positionTable->setText(count_row, 2, QString::number(receipt->GetPosElemRef(i)->quantity, 'f', 3) );    
	    count_row++;
	}
    } 	 
}

void DeletePositionByTimeRistrictionForm::keyPressEvent(QKeyEvent * k) 
{
    switch(k->key()) {
    case Qt::Key_Space :	
	accept();
	break;
    case Qt::Key_F12 :
	accept();
	break;    
    case Qt::Key_Enter :	
	accept();
	break;
    case Qt::Key_Return :
	accept();
	break;    
    case Qt::Key_Escape :	
	accept();
	break;
    }
}

void DeletePositionByTimeRistrictionForm::okPushButton_clicked()
{
    accept();
}






