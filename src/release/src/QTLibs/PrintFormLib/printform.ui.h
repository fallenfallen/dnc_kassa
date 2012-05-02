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

#include <qwidget.h>
#include <qfiledialog.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qstring.h>
#include "const.h"
#include <iostream>

QTextCodec* DefCodec = QTextCodec::codecForName(SYSCODEC);

/********************************************************/
void PrintForm::init()
{
    grabKeyboard();
    releaseKeyboard();
    connect(printPushButton, SIGNAL(clicked()), SLOT(Print()));
    connect(savePushButton, SIGNAL(clicked()), SLOT(Save()));
    connect(cancelPushButton, SIGNAL(clicked()), SLOT(Cancel()));
    
    PrintTextEdit->zoomIn(2);
}
/********************************************************/
void PrintForm::destroy()
{
    grabKeyboard();
}
/********************************************************/
void PrintForm::Save()
{
    QString filename = QFileDialog::getSaveFileName( "/home/", QString::null, this );
    
    if ( filename.isEmpty() ) return;
    
    QFile f( filename );
    
    if ( !f.open( IO_WriteOnly ) ) {
	QMessageBox::critical(this, DefCodec->toUnicode("Ошибка"), DefCodec->toUnicode("Файл не доступен для записи"), "OK", 0);
        return;
    }

    QTextStream t( &f );
    t << PrintTextEdit->text();
    f.close();
}
/********************************************************/
void PrintForm::LoadString(QString html_text)
{
    PrintTextEdit->setText(html_text);    
    
    /*QStyleSheet * mystylesheet = QStyleSheet::defaultSheet();
    
    QMap<QString, QString> attr;
    
    attr[QString("border ")] = QString("1 solid black");
    
    mystylesheet->tag(QString("table"), attr, QString(), *QMimeSourceFactory::defaultFactory(), false, (QTextDocument*)PrintTextEdit);
    PrintTextEdit->setStyleSheet(mystylesheet);   */

}
/********************************************************/
void PrintForm::Print()
{
    QPrinter * printer = new QPrinter( QPrinter::HighResolution );
    printer->setFullPage( TRUE );
    if ( printer->setup(this) ) {               // printer dialog
        QPainter p;
        if( !p.begin( printer ) )     return;
	
        QPaintDeviceMetrics metrics( p.device() );
        int dpiy = metrics.logicalDpiY();
        int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
        QRect view( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
	
	QFont f;
	f.setPointSize(7);
	QSimpleRichText richText( PrintTextEdit->text(),
                                  f,
                                  0,
                                  0,
                                  0,
                                  view.height() );
	
        richText.setWidth( &p, view.width() );
        int page = 1;
        do {
            richText.draw( &p, margin, margin, view, colorGroup() );
            view.moveBy( 0, view.height() );
            p.translate( 0 , -view.height() );
            p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
                        view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
            if ( view.top() - margin >= richText.height() )
                break;
            printer->newPage();
            page++;
        } while (TRUE);
    }
    
    delete printer;
    puts("FILE PRINT");
    
}
/********************************************************/
void PrintForm::Cancel()
{
    accept();
}
/********************************************************/
void PrintForm::keyPressEvent(QKeyEvent * k)
{
    
    switch( k->key() ) {
    case Qt::Key_F12 :  close();
    }
}

/********************************************************/



