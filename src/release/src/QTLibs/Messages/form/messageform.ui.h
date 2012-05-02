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

//extern KeyboardDevice * kbcode;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
 

//======================================================================//
void messageForm::init()
{    
    grabKeyboard();
    connect(closePushButton, SIGNAL(clicked()), SLOT(close()));
    adjustSize();
    
    if ( sp.autoblock_flag && bt!=NULL ) {
	bt->action_time = QTime::currentTime();
    }
    
    //if (hw->BCS_GetType() == 2 || hw->MCR_GetType() == 2) 
    
    if (  kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );

}

//======================================================================//
void messageForm::destroy()
{
    //puts("void messageForm::destroy()");
    releaseKeyboard();
}

//======================================================================//
void messageForm::SetText(QString str)
{
    //printf("Set text = %s \n", str.ascii());
    errorTextLabel->setText(str);
    adjustSize();
}
//======================================================================//
void messageForm::setIcon(int type) 
{
    //puts("MessaForm --- SetIcon");
    QPixmap pm; 
    if ( type == ERROR ) pm = QPixmap::fromMimeSource("error.png");
    if ( type == MESSAGE ) pm = QPixmap::fromMimeSource("info.png");
    
    pixmapLabel->setPixmap(pm);
}
//======================================================================//
void messageForm::makeKeyPress(QKeyEvent * k) 
{
    switch(k->key())  {
    case Qt::Key_Enter :	close();	break;
    case Qt::Key_Return :	close();	break;
    case Qt::Key_F12 :	close();	break;
    }
}
//======================================================================//
void messageForm::GetKeySequence()
{
    //puts("-----------------GetKeySequence()---------------------");
    
    if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_scaner->GetEvent(i) );	}
	kbcode_scaner->Clear();
    }
    
    if ( kbcode_reader != NULL ) {
	int n = kbcode_reader->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_reader->GetEvent(i) );	}
	kbcode_reader->Clear();
    }
    
}
//======================================================================//
void messageForm::keyPressEvent(QKeyEvent *k)
{
    
    if ( sp.autoblock_flag ) {
	if ( bt !=NULL )   bt->action_time = QTime::currentTime();    }
    
    int state = 0;
    
    //if ( kbcode != NULL) {	state = kbcode->FindCode(k);	}
    if ( kbcode_scaner != NULL) {	state = kbcode_scaner->FindCode(k);	}
    if ( kbcode_reader != NULL) {	state = kbcode_reader->FindCode(k);	}
    
    
    if ( state == 0 ) { makeKeyPress(k);  return;  }
    if ( state == 5 ) { GetKeySequence(); }
}
//======================================================================//


