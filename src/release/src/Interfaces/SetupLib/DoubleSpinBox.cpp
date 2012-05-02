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

#include "DoubleSpinBox.h"


DoubleSpinBox::DoubleSpinBox(QWidget *parent, const char *name)
    : QSpinBox(parent, name)
{
    //валидатор от 0 до 100.00, число знаков после запятой 2, назначить этому объекту
    QRegExp rx2("^(100\\.00)|(\\d{0,2}[\\.\\,]{1}\\d{0,2})$");
    setValidator (new QRegExpValidator(rx2, this));
    
    suffixLabel = new QLineEdit(this, "suffixLabel");
    //suffixLabel->setGeometry(QRect(width(), 6, 16, 19));
    suffixLabel->setGeometry(QRect(45, 7, 16, 10));
    suffixLabel->setText("%");
    suffixLabel->setFrame(false);
    suffixLabel->setFocusPolicy(QWidget::NoFocus);
    
    /*QPalette p ( suffixLabel->palette() );  
    p.setColor(QPalette::Active, QColorGroup::Background, QColor("white"));
    suffixLabel->setPalette(p);*/
    
    //suffixLabel->setPaletteBackgroundColor(QColor("white"));
    //connect( dsb, SIGNAL( valueChanged(int) ), this, SLOT( ParametersChanged() ) );
    QObject::connect( (const QObject*)editor(), SIGNAL( textChanged(const QString &) ), 
		     SLOT(editorTextChanged( const QString & )) );
}

//======================================================//
DoubleSpinBox::~DoubleSpinBox()
{
    delete suffixLabel;
}


//======================================================//
void DoubleSpinBox::editorTextChanged( const QString & kstr )
{
 //   double d = kstr.toDouble();
//    setValue(d);
//    QString s = QString::number(kstr.toDouble(), 'f', 2);
//    maxmoneyLineEdit->setText(s);
/*    int k = kstr.length();  // длина строки
    
    char* str = (char*) calloc(k+1, sizeof(char));
    strcpy(str, kstr.ascii());
    
    if ( *(str+k-1) == ',' ) {
	*(str+k-1) = '.';	
	(str);
    }

    free(str);*/
}
//======================================================//

QString DoubleSpinBox::mapValueToText(int value)
{
    QString str = QString( "%1.%2" ) .arg( (int) value / 100 ).arg( (int) value % 100 );
    //printf("значение преобразует в текст \n");
    return str;
}
//======================================================//
int DoubleSpinBox::mapTextToValue(bool *ok)
{
    //printf("преобразует текст в значение  \n");
    QString str = text();
    //printf("text of double spin box = %s \n", str.ascii());
    //printf("text of double spin box = %s \n", str.ascii()):
    if ( str.isEmpty() ) return 0;
    if ( str == QString(".") ) return 0;
    str = QString::number( 100*str.toDouble(ok), 'f', 0 );
    //printf("str = %s \n", str.ascii());
    int res = str.toInt();
    
    return res;
}
//======================================================//
void DoubleSpinBox::setGeometry( const QRect & rect )
{
    QSpinBox::setGeometry(rect);
   // suffixLabel->setGeometry(QRect(width()-40, 6, 16, 19));
}
//======================================================//
