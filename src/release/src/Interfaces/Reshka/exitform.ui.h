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

#include <cstdlib>
#include "const.h"
#include <qtextcodec.h>


int ret = 0;

void exitForm::init()
{ 
    connect(ExitButton, SIGNAL(clicked()), SLOT(Exit()));
    connect(RebootButton, SIGNAL(clicked()), SLOT(Reboot())); 
    connect(CloseReshkaButton, SIGNAL(clicked()), SLOT(CloseReshka()));
    connect(CanselButton, SIGNAL(clicked()), SLOT(close()));
    
    //QTextCodec* DefCodec = QTextCodec::codecForName(SYSCODEC);
    
    //QString true_caption = DefCodec->toUnicode(PROGRAMM_VERSION) + " - " + this->caption();
    //this->setCaption(true_caption);
    
}

void exitForm::Exit(void)
{
    puts("-----Exit-----");
    system("poweroff");
}

void exitForm::Reboot(void)
{
    puts("-----Reboot-----");
    system("reboot");
}

void exitForm::CloseReshka(void)
{
    puts("------Close------");
    ret = 1;
    close();
}

void exitForm::Up(void)
{
   puts("Up"); 
   QWidget *pb;
   pb = (QWidget *) ExitButton;
   if(pb->hasFocus())
   {
       pb = (QWidget *) CanselButton;
       pb->setFocus();
       return;
   }
   pb = (QWidget *) CanselButton;
   if(pb->hasFocus())
   {
       pb = (QWidget *) CloseReshkaButton;
       pb->setFocus();
       return;
   }
   pb = (QWidget *) CloseReshkaButton;
   if(pb->hasFocus())
   {
       pb = (QWidget *) RebootButton;
       pb->setFocus();
       return;
   }
   pb = (QWidget *) RebootButton;
   if(pb->hasFocus())
   {
       pb = (QWidget *) ExitButton;
       pb->setFocus();
       return;
   }   
}

void exitForm::Down(void)
{
    //puts("Down");
    QWidget *pb;
    pb = (QWidget *) ExitButton;
    if(pb->hasFocus())
    {
	pb = (QWidget *) RebootButton;
	pb->setFocus();
	return;
    }
    pb = (QWidget *) RebootButton;
    if(pb->hasFocus())
    {
	pb = (QWidget *) CloseReshkaButton;
	pb->setFocus();
	return;
    }
    pb = (QWidget *) CloseReshkaButton;
    if(pb->hasFocus())
    {
	pb = (QWidget *) CanselButton;
	pb->setFocus();
	return;
    }
    pb = (QWidget *) CanselButton;
    if(pb->hasFocus())
    {
	pb = (QWidget *) ExitButton;
	pb->setFocus();
	return;
    }       
}

void exitForm::keyPressEvent(QKeyEvent *k)
{
    //puts("void exitForm::keyPressEvent(QKeyEvent *k)");
    switch(k->key())
    {
    case Qt::Key_Up:
	{
	    //puts("Qt::Key_Up:");
	    Up();
	    break;
	}
    case Qt::Key_Down:
	{
	    //puts("Qt::Key_Down:");
	    Down();
	    break;
	}
    default:
	{
	    //puts("Чего-то кто-то пытается ввести!!!");
	    break;
	}
    }
}

