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
#include "reference_book_class_pgsql.h"
#include "const.h"
#include "hwsrv.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"

extern PgRef M;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;
int  AutoScheme_id;

extern bool autoschemeform_flag;
//bool autoschemeform_flag = false;

void Autoschemeform::init()
{
   // puts("void Autoschemeform::init()");
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
    cp1251Codec = QTextCodec::codecForName(HWCODEC);
    FillAutoGroupListBox();//загрузить названия автоматических скидок
    autoschemeform_flag = true;
    exit_flag = false;
   // grabKeyboard();
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
}

void Autoschemeform::closeEvent( QCloseEvent* e )
{
//  puts("void Autoschemeform::closeEvent( QCloseEvent* e )");
  e->accept();
  autoschemeform_flag = false;
    if (exit_flag == true)
  {
  //    puts("exit_flag == true");
  }	  
  else
  {
  //    puts("exit_flag == false");
      AutoScheme_id = Number_Group;
  }
  return;
}


int Autoschemeform::FillAutoGroupListBox()
{
//    puts("int Autoschemeform::FillAutoGroupListBox()");
     int i;
     int auto_count =M. GetQuantityStrInAutoSchemeGroup();//количество автоматических скидок
     //выделение памяти для mas
   char ** mas = (char**) calloc(auto_count, sizeof(char*));
    for ( i=0; i<auto_count; ++i) 
    {
	mas[i] = (char*) calloc(201, sizeof(char));
    }
    M.GetNameAutoScheme(mas);
      for (i = 0;i<auto_count;i++)
    {
	//printf("in FillAutoGroupListBox mas[%d] = %s\n",i,mas[i]);
	if (mas[i]==NULL)
	{
	//    puts("exit");
	    return 0;
	}
	else
	{
	    new QListViewItem(AutoScheme_listView,dbaseCodec->toUnicode(QString(mas[i])));
	}
    } 
    //установка курсора  на текущую  группу автоматических скидок, если был выбран  товар
     AutoScheme_listView->firstChild()->setSelected(TRUE);
     AutoScheme_listView->setCurrentItem(AutoScheme_listView->firstChild());
     
     int c =auto_count;
  
     while (--c=0) 
     {
	// printf("%s free \n", mas[c]);
	 free(mas[c]);    
     }
     free(mas); 
    return i;
}


void Autoschemeform::AutoschemeCansel_clicked()
{
    close();
}


void Autoschemeform::AutoschemeOK_clicked()
{
    int n =AutoScheme_listView->childCount();
 //   printf("n = %d\n",n);
    if (n>0)
    {
	
	QString str =  dbaseCodec->fromUnicode(AutoScheme_listView->currentItem()->text(0));
	//printf("~~~~~~~~~str = |%s|\n",str.ascii());
	
	char * value = (char *)calloc(20,sizeof(char));
	M.GetAutoSchemeId(str.ascii(),value);
	QString s =  value;
	free(value);
	//printf("return auto_scheme_id     s = |%s|\n",s.ascii());
	if (s !=systemCodec->toUnicode(WITHOUTAUTOSCHEME))
	{
	    AutoScheme_id = s.toInt();
	}
	else
	{
	   AutoScheme_id = 0; 
	}	
    }
   // printf("AutoScheme_id in Autoschemeform = %d\n",AutoScheme_id);
    exit_flag = true;
    close();
}






//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------//
void Autoschemeform::GetKeySequence()
{
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


void Autoschemeform::keyPressEvent(QKeyEvent *k)
{
  //  printf("--------------- press k->key = %d ---------------\n", k->key());
    
    
    int state = 0;
    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	}
    }
    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    //QString cardcode_str = ((KeyboardReader*) kbcode_reader)->getCode(2);
	    ;
	}
    }
    
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }

}


void Autoschemeform::makeKeyPress(QKeyEvent *k)
{
 //   puts("void Autoschemeform::makeKeyPress(QKeyEvent *k)");
    switch(k->key())   
    {
    case Qt::Key_F12 :
	{
	//    puts("Qt::Key_F12");
	    close();
	    reject();
	    break;
	}
    case Qt::Key_Enter :
	{
	//    printf("Qt::Key_Enter");
		if (AutoschemeOK->hasFocus())
			 AutoschemeOK_clicked();
		if (AutoschemeCansel->hasFocus())
			 AutoschemeCansel_clicked();
	    break;
	}
    case Qt::Key_Return :
	{
	//    printf("Qt::Key_Return");
		if (AutoschemeOK->hasFocus())
			 AutoschemeOK_clicked();
		if (AutoschemeCansel->hasFocus())
			 AutoschemeCansel_clicked();
	    break;
	}
    case Qt::Key_Left :
	{
	//    printf("Qt::Key_Left");
	    break;
	}
    case Qt::Key_Right :
	{
	//    printf("Qt::Key_Right");
	    break;	
	}
    case Qt::Key_Tab :
	{
	//    printf("Qt::Key_Tab");
	    break;
	}
    case Qt::Key_Up :
	{
	//   printf("Qt::Key_Up");
	    break;
	}
    case Qt::Key_Down :
	{
	 //   printf("Qt::Key_Down");
	    break; 
	}
    default :
    {
	//printf("default :"); 
	break; 
    }
}		
}




//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//

