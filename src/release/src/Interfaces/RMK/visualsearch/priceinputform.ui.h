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
extern SP sp;
extern BlockTimer *bt;

//===============================================================//
void priceinputform::init()
{
    // grabKeyboard();
    price = -1;
    str = "";
    adjustSize();
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    connect(priceLineEdit, SIGNAL(textChanged(const QString &)), priceLineEdit, SLOT(deselect()));
    connect(priceLineEdit, SIGNAL(returnPressed()), this, SLOT(SaveValue()));
    connect(priceLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(CheckValue()));
}

//===============================================================//
void priceinputform::destroy()
{
}

int priceinputform::SetValidatorProperties()
{
   QDoubleValidator * qv_double = new QDoubleValidator(0.001, 9999999.99, 2, priceLineEdit);
    v = qv_double;
    priceLineEdit->setValidator(v);
   QString s;
   priceLineEdit->setText("");
    return 0;
}
void priceinputform::SaveValue()
{
    price = priceLineEdit->text().toDouble();
    if ( price > 0 ) {
    accept();
    } else reject();
}

void priceinputform::CheckValue()
{
    int curpos = priceLineEdit->cursorPosition();
    QString s = priceLineEdit->text();
    switch(v->validate(s, curpos))  {
    case QValidator::Intermediate :
	if(s == "") break;
	else if(s.toDouble() - 0.001 < 0) break; ///??????????
    case QValidator::Invalid :
	priceLineEdit->setText(str);
	break;
    }
    str = priceLineEdit->text();
}

//===============================================================//
void priceinputform::keyPressEvent(QKeyEvent *k)
{
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    switch ( k->key() )  {
    case Qt::Key_Escape :    {
	    priceLineEdit->setText("");
	    break;
	} 
    case Qt::Key_F12 :    {
	    reject();
	    break;
	}
    }
}
