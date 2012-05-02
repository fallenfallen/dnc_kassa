/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
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

using namespace std;

#include "Decoder.h"
#include "WareSystem.h"
#include "WareCatalog.h"
#include "setup_param.h"


extern SP sp;

int default_count_column;
/***************************************************************/
void WareAdditionForm::init() {
	 grabKeyboard();
	 default_count_column = table->numCols();
	 int i = 0;
	 table->hideColumn(i++);
	 table->setColumnWidth( i++, 135 );
	 table->setColumnWidth( i++, 120 );
	 table->setColumnWidth( i++, 80 ); 
	 table->setColumnWidth( i++, 90 ); 
	 
	 
	 if ( !sp.block_mouse )  {
		  if ( sp.one_click_selection ) connect(table, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(accept()));
		  connect(table, SIGNAL(doubleClicked( int , int , int , const QPoint &)), SLOT(accept()));
	 }
	 if(sp.block_mouse) grabMouse();
	 
}
/***************************************************************/
void WareAdditionForm::destroy() {
	 releaseKeyboard();
	 if ( sp.block_mouse ) releaseMouse();
}
/***************************************************************/
void WareAdditionForm::keyPressEvent(QKeyEvent *k)
{
	 //puts("void UserForm::keyPressEvent(QKeyEvent *k)");
	 //printf("key press code == %d \n", k->key());
	 //printf("key text == <%s> \n", k->text().ascii());
	 
	 makeKeyPress(k);   
}

/***************************************************************/
void WareAdditionForm::makeKeyPress(QKeyEvent * k) 
{    
	 if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4144 && k->key() < 4155 ) ) return;
	 int current_row = table->currentRow();
	 int current_col = table->currentColumn();
	 
	 switch (k->key())    {    
	 case Qt::Key_F12 :
		  reject();
		  break;
	 case Qt::Key_Enter :
		  accept();
		  break;
	 case Qt::Key_Return :
		  accept();
		  break;
	 case Qt::Key_Up :
		  if ( current_row > 0 ) 
				table->setCurrentCell(current_row - 1, current_col);
		  break;
	 case Qt::Key_Down :
		  if ( current_row < table->numRows() - 1 )
				table->setCurrentCell(current_row + 1, current_col);
		  break;     
	 case Qt::Key_Left :
		  if ( current_col > 0 ) 
				table->setCurrentCell(current_row, current_col - 1);
		  break;
	 case Qt::Key_Right :
		  if ( current_col < table->numCols() - 1 )
				table->setCurrentCell(current_row, current_col + 1);
		  break;     	
	 case Qt::Key_Escape :
		  reject();
		  break;
	 }	
}
/***************************************************************/
int WareAdditionForm::setWare(Ware * ware) {
	 
	 try {
		  //table->setNumCols(4);
		  table->setNumRows(0);
		  
		  vector<WareAddition*> additions = ware->getAdditions();
		  cout << "additions count = " << additions.size() << endl;
		  
		  if ( additions.size() == 0 ) 
				return 0;
		  warenameLabel->setText( toForm(ware->getInfo().title) );
		  
		  //table->setNumRows(additions.size());
		  int column_count = table->numCols();
		  cout << "column_count - " << column_count << endl;
		  
		  vector<Feature*> features = ware->getFeatures();
		  cout << "features coint - " << features.size() << endl;
		  table->setNumCols(column_count + features.size());
		  
		  for (unsigned int i = 0; i < features.size(); i++) {
				cout << "features_name - " << features[i]->getName() << endl;
				table->horizontalHeader()->setLabel( column_count + i, toForm(features[i]->getName()) );
				table->setColumnWidth(column_count + i, 100);
				table->setColumnStretchable ( column_count + i, true );
		  }
		  
		  for (unsigned int addition_num = 0; addition_num < additions.size(); addition_num++) {
				printf("addition_num - %d \n", addition_num);
				if ( sp.show_absent_goods || additions[addition_num]->getQuantity() > 0) {
					 int row_num = table->numRows();
					 printf("row_num = %d \n", row_num);
					 table->setNumRows(row_num + 1);
					 int column_num = 0;
					 table->setText( row_num, column_num++, toForm(additions[addition_num]->getCode()) );
					 table->setText( row_num, column_num++, toForm(additions[addition_num]->getArticle() ) );
					 table->setText( row_num, column_num++, toForm(additions[addition_num]->getMainBarcode()) );
					 table->setText( row_num, column_num++, toForm(additions[addition_num]->getPrice()) );
					 table->setText( row_num, column_num++, toForm(additions[addition_num]->getQuantity(), 3) );
					 for (unsigned int f_num = 0; f_num < features.size(); f_num++) {
						  string value = additions[addition_num]->getSelectedValue(features[f_num]);
						  table->setText( row_num, f_num + column_count, toForm( value ) );
					 }
				}
		  }
		  
		  int result_count_row = table->numRows();
		  
		  double unalloated_quantity = ware->getUnallotedQuantity();
		  cout << "unalloated_quantity = " << unalloated_quantity << endl;
		  if ( sp.show_absent_goods ||  unalloated_quantity > 0) {
				int row_num = table->numRows();
				cout << "row_num - " << row_num << endl;
				table->setNumRows(row_num + 1);
				int column_num = 0;
				cout << "ware main barcode - " << ware->getMainBarcode() << endl;
				cout << "column_num - " << column_num << endl;
				
				table->setText( row_num, column_num++, toForm("-1") );
				table->setText( row_num, column_num++, toForm("Без характеристик") );
				
				if ( ware_reg_barcode.isEmpty() ) 
				    table->setText( row_num, column_num++, toForm(ware->getMainBarcode()) );
				else 
				    table->setText( row_num, column_num++, ware_reg_barcode );
				table->setText( row_num, column_num++, toForm(ware->getInfo().price) );
				table->setText( row_num, column_num++, toForm(unalloated_quantity, 3) );
		  }
		  
		  table->setCurrentCell(0, 0);
		  //cout << "result_count_row = " << result_count_row << endl;
		  return result_count_row;
		  
	 } catch (const Error & err) {
		  cout << "Ошибка " << err.getErrorCode() << " " << err.getErrorUserText() << endl;
		  //cout << err.getErrorLogText() << endl;
		  //QString err_str = toForm("Ошибка № ") + toForm(err.getErrorCode()) + toForm("\n") + toForm(err.getErrorUserText());
		  //QMessageBox::critical(this, toForm("Ошибка"), err_str);
	 }
	 
	 //puts("---------end set ware--------");
}
/***************************************************************/
QString WareAdditionForm::getAdditionId() {
	 
	 
	 int row = table->currentRow();
	 return table->text(row, 0);
	 
	 /*WareAdditionElement result;
		
	int row = table->currentRow();
	if ( row != -1 ) {
	strcpy(result.addition_id, table->text(row, 0).ascii());
	strcpy(result.addition_text , fromForm(getAdditionText(row)).ascii());
	strcpy(result.addition_price, table->text(row, 4).ascii());
	strcpy(result.addition_quantity, table->text(row, 3).ascii());
	
	printf("result.addition_id = %s \n", result.addition_id);
	printf("result.addition_text = %s \n", result.addition_text);
	}
	return result;*/
	 
}
/***************************************************************/
QString WareAdditionForm::getAdditionText(int row) {
	 
	 if ( row == -1 ) return QString("");
	 if ( table->numCols() == default_count_column ) return QString("");
	 QString text;
	 if ( !table->text(row, default_count_column).isEmpty() ) {
		  text += " (";
		  text += table->text(row, default_count_column);
	 }    
	 for (int col = default_count_column + 1; col < table->numCols();  col++) {
		  if ( !table->text(row, col).isEmpty() ) {
				text += ", ";
				text += table->text(row, col);
		  }
	 }
	 text += ")";
	 return text;
}

/***************************************************************/
void WareAdditionForm::setWareRegBarcode( const QString & barcode )
{
    ware_reg_barcode = barcode;
}
/***************************************************************/






