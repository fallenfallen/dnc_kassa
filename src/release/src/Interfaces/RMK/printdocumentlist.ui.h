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

//#include "setup_doc_pgsql.h"
#include <qtextcodec.h>
#include "const_conf.h"
#include "const.h"


//extern Receipt * receipt;

/************************************************/
void PrintDocumentListForm::init()
{
    documents = NULL;
    path = "";
    grabKeyboard();
    //releaseKeyboard();
    DocumentTable->setColumnWidth(0, 230);
    
    connect(DocumentTable, SIGNAL( doubleClicked( int, int, int , const QPoint & ) ), SLOT(PrintDocument()));
}
/************************************************/
int PrintDocumentListForm::GetDocumentInfo()
{
    SetupDocument doc_db(LINCASH_CONF_PATH);
    if ( doc_db.er_con != 0 ) return -1;
    
    QTextCodec* codec_db =  QTextCodec::codecForName(DBCODEC);
    QTextCodec * codec_sys =  QTextCodec::codecForName(SYSCODEC);
    
    int count_doc = 0;
    
    if ( doc_db.GetActiveDocuments(documents, count_doc) != 0 ) return -1;   
    
    DocumentTable->setNumRows(count_doc);
    
    //cout << "count = " << count_doc << endl;
    for ( int i = 0; i < count_doc; i++ )   {
	//cout << "name = " << documents[i].name << " path = " << documents[i].path << endl;
	DocumentTable->setText(i, 0, codec_db->toUnicode(documents[i].name));
    }	
    
    if ( count_doc > 0 ) {
	DocumentTable->setCurrentCell(0, 0);
	path = codec_db->toUnicode(documents[DocumentTable->currentRow()].path);
    }
    
    return count_doc;
    
}
/************************************************/
QString PrintDocumentListForm::GetPath()
{
    return path;
}
/************************************************/
void PrintDocumentListForm::PrintDocument()
{
    if ( DocumentTable->currentRow() < 0 ) return;
    QTextCodec* codec_db =  QTextCodec::codecForName(DBCODEC);
    path = codec_db->toUnicode(documents[DocumentTable->currentRow()].path);
    accept();
}
/************************************************/
void PrintDocumentListForm::keyPressEvent(QKeyEvent * k)
{
    switch ( k->key() )   {
    case Qt::Key_Enter:
	PrintDocument();
	break;
    case Qt::Key_Return:
	PrintDocument();
	break;	
    case Qt::Key_F12:
	reject();
	break;	
    case Qt::Key_Space:
	PrintDocument();
	break;	
    case Qt::Key_Up:
	if ( DocumentTable->currentRow() > 0 ) DocumentTable->setCurrentCell(DocumentTable->currentRow()-1, 0);
	break;	
    case Qt::Key_Down:
	if ( DocumentTable->currentRow() < DocumentTable->numRows() ) DocumentTable->setCurrentCell(DocumentTable->currentRow()+1, 0);
	break;		
    }
    
}
/************************************************/
void PrintDocumentListForm::destroy()
{
    if ( documents != NULL ) free(documents);
    grabKeyboard();
}
/************************************************/

