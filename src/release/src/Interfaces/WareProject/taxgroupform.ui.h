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

//extern KeyboardDevice * kbcode;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;

extern PgRef M;

int TaxGroup_id;

extern bool taxgroupform_flag;
//bool taxgroupform_flag = false;

void TaxGroupform::init()
{
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
    cp1251Codec = QTextCodec::codecForName(HWCODEC);
  //  puts("void TaxGroupform::init()");
    FillTaxGroupListBox();
    taxgroupform_flag = true;
    exit_flag =false;
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
   // grabKeyboard();
}

void TaxGroupform::closeEvent( QCloseEvent* e )
{
//  puts("void TaxGroupform::closeEvent( QCloseEvent* e )");
  e->accept();
  taxgroupform_flag = false;
  if (exit_flag == true)
  {
    //  puts("exit_flag == true");
      TaxGroup_id = t_id; 
  }	  
  else
  {
    //  puts("exit_flag == false");
      TaxGroup_id = Number_Group;
  }
 // printf("TaxGroup_id = %d\n",TaxGroup_id);
  return;
}


int TaxGroupform::FillTaxGroupListBox(void)
{
  //  puts("int TaxGroupform::FillTaxGroupListBox(void)");
    int i;
    int tax_count =M. GetQuantityStrInTaxGroup();
    char ** mas = (char**) calloc(tax_count, sizeof(char*));
    for ( i=0; i<tax_count; ++i) 
    {
	mas[i] = (char*) calloc(201, sizeof(char));
    }
    M.GetNameTaxGroup(mas);
    for (i = 0;i<tax_count;i++)
    {
	//printf("in FillTaxGroupListBox mas[%d] = %s\n",i,mas[i]);
	if (mas[i]==NULL)
	{
	//    puts("exit");
	    return 0;
	}
	else 
	{
	    new QListViewItem(TaxGroup_listView,dbaseCodec->toUnicode(QString(mas[i])));
	}
    } 
    //установка курсора  на текущую налоговую группу, если был выбран  товар
    int c = tax_count;
    while (--c=0) 
    {
	//printf("%s free \n", mas[c]);
	free(mas[c]);    
    }
    free(mas);
    return i;
}



void TaxGroupform::TaxGroupCansel_clicked()
{
  //  puts("void TaxGroupform::TaxGrouppushButtonCansel_clicked()");
 close();
}


void TaxGroupform::TaxGroupOK_clicked()
{
    //puts("void TaxGroupform::TaxGrouppushButtonOK_clicked()");
    if (TaxGroup_listView->childCount()>0)
    {
	QString str =  dbaseCodec->fromUnicode(TaxGroup_listView->currentItem()->text(0));
	if (TaxGroup_listView->currentItem()->text(0)!=systemCodec->toUnicode(WITHOUTGROUP))
	{
	    char * value = (char *) calloc(20,sizeof(char));
	    M.GetTaxRateId(str.ascii(),value);
	    QString s = value;
	    free(value);
	    t_id = s.toInt();
	}
	else
	{
	   t_id = 0; 
	}
    }
    exit_flag = true;
    close();
}

//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------/


void TaxGroupform::GetKeySequence()
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


void TaxGroupform::keyPressEvent(QKeyEvent *k)
{
    //printf("--------------- press k->key = %d ---------------\n", k->key());
    int state = 0;
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	if ( state == 2 )  {
	    ;
	}
    }    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	if ( state == 2 ) {
	    ;
	}
    }
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
}

void TaxGroupform::makeKeyPress(QKeyEvent *k)
{
    //puts("void TaxGroupform::makeKeyPress(QKeyEvent *k)");
    switch(k->key())   
    {
    case Qt::Key_F12 :
	{
	 //   puts("Qt::Key_F12");
	    close();
	    reject();
	    break;
	}
    case Qt::Key_Enter :
	{
	   // printf("Qt::Key_Enter");
	    if (TaxGroupOK->hasFocus())
			  TaxGroupOK_clicked();
	    if (TaxGroupCansel->hasFocus())
			  TaxGroupCansel_clicked();
	    break;
	}
    case Qt::Key_Return :
	{
	 //   printf("Qt::Key_Return");
         if (TaxGroupOK->hasFocus())
			  TaxGroupOK_clicked();
	    if (TaxGroupCansel->hasFocus())
			  TaxGroupCansel_clicked();
	    break;
	}
    case Qt::Key_Left :
	{
	//    printf("Qt::Key_Left");
	    break;
	}
    case Qt::Key_Right :
	{
	   // printf("Qt::Key_Right");
	    break;	
	}
    case Qt::Key_Tab :
	{
	   // printf("Qt::Key_Tab");
	    break;
	}
    case Qt::Key_Up :
	{
	   // printf("Qt::Key_Up");
	    break;
	}
    case Qt::Key_Down :
	{
	   // printf("Qt::Key_Down");
	    break; 
	}
    default :
    {	break; 
	//printf("default :");
    }
}		
}
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//
