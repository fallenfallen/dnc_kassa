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

#include "updownlineedit.h"

#include <qpixmap.h>
#include <qfont.h>

//======================================================//
UpDownLineEdit::UpDownLineEdit(QWidget *parent, const char *name, QString possible_value)
    : QLineEdit(parent, name),	possible_val(possible_value)
{    
    buttonUp = new QPushButton(parent, "buttonUp");
    //QPixmap pm ("./image/arrow_up.png");
    QPixmap pm = QPixmap::fromMimeSource("arrow_up.png");
    buttonUp->setPixmap( pm);
    
    buttonDown = new QPushButton(parent, "buttonDown");
    //pm = QPixmap("./image/arrow_down.png");
    pm = QPixmap::fromMimeSource("arrow_down.png");
    buttonDown->setPixmap( pm);
    
    setPosition(0, 0);
    setMaxLength(1);
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    
    QFont f = font();
    f.setPointSize(12);
    f.setBold(false);
    setFont(f);
    
    buttonUp->setFocusPolicy(QWidget::NoFocus);
    buttonDown->setFocusPolicy(QWidget::NoFocus);
    
    QSizePolicy sp (QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(sp);
    buttonUp->setSizePolicy(sp);
    buttonDown->setSizePolicy(sp);
    
    buttonUp->setFlat(true);
    buttonDown->setFlat(true);
    
    connect ( buttonUp, SIGNAL(clicked()), SLOT(GoUp()) );
    connect ( buttonDown, SIGNAL(clicked()), SLOT(GoDown()) );
    
    index = 0;
    //puts("UpDownCreate");
}
//======================================================//
UpDownLineEdit::~UpDownLineEdit()  
{
    delete buttonUp;
    delete buttonDown;
    //puts("UpDownDestroy");
}
//======================================================//
void UpDownLineEdit::setPosition(int x, int y) 
{
    buttonUp->setGeometry(x, y, EDIT_WIDTH, BUTTON_HEIGHT);
    setGeometry(x, y+BUTTON_HEIGHT, EDIT_WIDTH, EDIT_HEIGHT);
    buttonDown->setGeometry(x, y+BUTTON_HEIGHT+EDIT_HEIGHT, EDIT_WIDTH, BUTTON_HEIGHT);
}
//======================================================//
void UpDownLineEdit::setEnabled(bool b) 
{
    buttonUp->setEnabled(b);
    buttonDown->setEnabled(b);
    QLineEdit::setEnabled(b);
}
//======================================================//
void UpDownLineEdit::focusInEvent(QFocusEvent * fe)  
{
    //puts("UpDownLineEdit::focusInEvent(QFocusEvent * fe)");
    
    QFont f = font();
    f.setBold(true);
    setFont(f);
    
    QLineEdit::focusInEvent(fe);
}
//======================================================//
void UpDownLineEdit::focusOutEvent(QFocusEvent * fe)  
{
    //puts("UpDownLineEdit::focusOutEvent(QFocusEvent * fe)");
    
    QFont f = font();
    f.setBold(false);
    setFont(f);
    
    QLineEdit::focusOutEvent(fe);
}
//======================================================//
void UpDownLineEdit::keyPressEvent(QKeyEvent * ke) 
{
    puts("UpDownLineEdit::keyPressEvent(QKeyEvent * ke)");
    
    switch(ke->key())    {
    case Qt::Key_Up :
	GoUp();
	break;	
    case Qt::Key_Down :
	GoDown();
	break;		}
    
    QLineEdit::keyPressEvent(ke);
}
//======================================================//
void UpDownLineEdit::GoUp()
{
    //puts("GoUp");
    QLineEdit::setFocus();
    if ( index > 0 ) {
	index = index - 1; 
	setValue(index);    }
}
//======================================================//
void  UpDownLineEdit::GoDown() 
{
    QLineEdit::setFocus();
    if ( index < (possible_val.length()-1) ) {
	index = index + 1;
	setValue(index);    }
}
//======================================================//
int UpDownLineEdit::setText(QChar c)
{
    int k = possible_val.find(c);
    if (k != -1) setValue(k);
    return k;
}
//======================================================//
int  UpDownLineEdit::getIndex(QChar c) {
    return possible_val.find(c);
}
//======================================================//
void UpDownLineEdit::setPossibleValue(QString str)
{
    possible_val = str;
}
//======================================================//
void UpDownLineEdit::setValue(int num) 
{
    if ( (num<0) || (num>=(int)possible_val.length()) ) return;
    QString  s;
    s[0] = possible_val[num];
    index = num;
    QLineEdit::setText( s );
}
//======================================================//    
int UpDownLineEdit::getValue() 
{
    return index;
}
//======================================================//    
    





