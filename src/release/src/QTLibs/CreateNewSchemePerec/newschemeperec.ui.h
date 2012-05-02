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
#include <malloc.h>

void NewSchemePerec::init()
{
   // puts("NewSchemePerec::init()");
    DefCodec = QTextCodec ::codecForName(SYSCODEC);
    char* tmp_str = (char*) calloc(100, sizeof(char));
    strcpy(tmp_str, "^[A-Za-zА-Яа-я]\[A-Za-zА-Яа-я0-9 _-/]{0,19}$");
    QRegExp rx(DefCodec->toUnicode(tmp_str));
    QValidator * val_schemename = new QRegExpValidator(rx, this);
    free(tmp_str);
    newname_lineEdit->setValidator(val_schemename);
}
/***********************************************************************/
void NewSchemePerec::destroy()
{
    //puts("NewSchemePerec::destroy()");
}
/***********************************************************************/
void NewSchemePerec::ok_pushButton_clicked()
{
   // puts("NewSchemePerec::ok_pushButton_clicked()");
    if (strlen(newname_lineEdit->text().ascii())== 0)  { 
	return;    }
    accept();
}
/***********************************************************************/
void NewSchemePerec::cansel_pushButton_clicked()
{
  //  puts("NewSchemePerec::cansel_pushButton_clicked()");
    reject();
}
/***********************************************************************/
QString NewSchemePerec::getSchemeName()
{
  //  puts("NewSchemePerec::getSchemeName()");
    QString qstring_result;
    qstring_result = DefCodec->fromUnicode(newname_lineEdit->text());
    return qstring_result;
}
/***********************************************************************/
void NewSchemePerec::keyPressEvent(QKeyEvent * k)
{
    switch(k->key())    {    
    case Qt::Key_F12 :
	reject();
	break;
    case Qt::Key_Enter : 
	ok_pushButton_clicked();
	break;
    case Qt::Key_Return : 
	ok_pushButton_clicked();
	break;
    case Qt::Key_Escape : 
	cansel_pushButton_clicked();
	break;
    }
}

/***********************************************************************/
void NewSchemePerec::setNewSchemeName(char* sname)
{
   // puts("NewSchemePerec::setNewSchemeName(char* sname)");
    newname_lineEdit->setText(DefCodec->toUnicode(sname));
}
/***********************************************************************/


