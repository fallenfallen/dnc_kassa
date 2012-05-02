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
#include <cstdlib>

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;
extern PgRef M;

int Group_id;
extern bool waregroupform_flag;
//bool waregroupform_flag = false;

void WareGroupform::init()
{
    
   // puts("void WareGroupform::init()");
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
    group_count = M.GetQuantityStrInWareGroup();//количество групп в ware_group
    FillWaresGroupListBox();//функция загрузки группы товаров
    //функция для установки курсора на нужную группу товаров
    waregroupform_flag = true;
    exit_flag =false;
    //grabKeyboard();
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    WareGroup_listView->hideColumn(1);
}

void WareGroupform::closeEvent( QCloseEvent* e )
{
  //puts("void WareGroupform::closeEvent( QCloseEvent* e )");
  e->accept(); 
  waregroupform_flag = false;
  if (exit_flag == true)
  {
     // puts("exit_flag == true");
      Group_id = g_id; 
  }	  
  else
  {
     // puts("exit_flag == false");
      Group_id = Number_Group;
  }
  return;
}


void WareGroupform::FillWaresGroupListBox()
{
  //  puts("void WareGroupform::FillWaresGroupListBox()");
    int i = 0;
    WareGroup_listView->clear();
    int null_ware_group_count = M.GetQuantityStrInNULLWareGroup();
    if (null_ware_group_count>0){
      char ** mas = (char**) calloc(null_ware_group_count, sizeof(char*));
      for (i = 0; i<null_ware_group_count; i++) 
      {
	  mas[i] = (char*) calloc(201, sizeof(char));
      }
      M.GetNameWareGroupByParentId("NULL",mas);
      for (i = 0;i<null_ware_group_count;i++)
      {
	   char * value = (char *)calloc(20,sizeof(char));
	   M.GetGroupId(mas[i],value);
	   QString s =  value;
	   free(value); 
	  QListViewItem * plvItem = new QListViewItem(WareGroup_listView,dbaseCodec->toUnicode(QString(mas[i])),s); 
	  plvItem->setOpen(true);
	  ShowNextLevel(plvItem);
      }
      while (--null_ware_group_count = 0) 
      {
	  free(mas[null_ware_group_count]);  
      }
      free(mas);
  }
  new QListViewItem(WareGroup_listView,systemCodec->toUnicode(WITHOUTGROUP)); 
}



int WareGroupform:: ShowNextLevel(QListViewItem  *plvItem)
{
  //  puts("int WareGroupform:: ShowNextLevel(QListViewItem  *plvItem)");
    int i = 0;
    QString str  = dbaseCodec->fromUnicode(plvItem->text(0));
    QListViewItem * newItem = plvItem;
    QString group = dbaseCodec->fromUnicode(plvItem->text(1));
    if (group !="ERROR"){ 
      int k1 = M.GetQuantityStrInNotNULLWareGroup(group.ascii());
      if (k1 >0){
	    char ** mas11 = (char**) calloc(k1, sizeof(char*));
	    for (  i=0; i<k1; i++) 
	    {
		mas11[i] = (char*) calloc(201, sizeof(char));
	    }
	    M.GetNameWareGroupByParentId(group.ascii(),mas11,0, k1);
	    for ( i = 0;i<k1;i++)
	    {
	      char * value = (char *)calloc(20,sizeof(char));
		 M.GetGroupId(mas11[i], value, group.ascii());
		 QString s = value;
		// printf("insert qlistviewitem * othersItem  by name = |%s|,group_id = |%s|\n", mas11[i], value);
		 free(value);
		 QListViewItem  * othersItem = new QListViewItem(newItem,dbaseCodec->toUnicode(QString(mas11[i])),s);
			ShowNextLevel(othersItem);
	    }
	    while (--k1 = 0) 
	    {		
		free(mas11[k1]);  
	    }	   
	    free(mas11); 
      }
      return k1;
    }
    return 0;
}

void WareGroupform::WareGroupOK_clicked()
{
    if (WareGroup_listView->childCount()>0)
    {
	
	QString grouptext = dbaseCodec->fromUnicode(WareGroup_listView->currentItem()->text(0));
	if (WareGroup_listView->currentItem()->text(0)!=systemCodec->toUnicode(WITHOUTGROUP))
	{
	//    printf("str = %s\n",grouptext.ascii());
	    char * value = (char *)calloc(20,sizeof(char));
	    M.GetGroupId(grouptext.ascii(),value);
	    g_id = atoi(value);
	//    printf("Group_id in WareGroupform = %d\n",Group_id);
	    free(value);
	}
	else
	{
	   g_id = 0; 
	}
    }
    exit_flag = true;
    close();
}


void WareGroupform::WareGroupCansel_clicked()
{
      // puts("void WareGroupform::WareGrouppushButtonCansel_clicked()");
       close();
}

  
//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------/
void WareGroupform::GetKeySequence()
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


void WareGroupform::keyPressEvent(QKeyEvent *k)
{
 //   printf("--------------- press k->key = %d ---------------\n", k->key());
    int state = 0;
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    ;
	}
    }

    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    ;
	}
    }
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
}

void WareGroupform::makeKeyPress(QKeyEvent *k)
{
  //  puts("void WareGroupform::makeKeyPress(QKeyEvent *k)");
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
         if (WareGroupOK->hasFocus())
				 WareGroupOK_clicked();
	    if (WareGroupCansel->hasFocus())
			  WareGroupCansel_clicked();
	    break;
	}
    case Qt::Key_Return :
	{
         if (WareGroupOK->hasFocus())
				 WareGroupOK_clicked();
	    if (WareGroupCansel->hasFocus())
			  WareGroupCansel_clicked();
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
	//    printf("Qt::Key_Up");
	    break;
	}
    case Qt::Key_Down :
	{
	//    printf("Qt::Key_Down");
	    break; 
	}
    default :
    {	break; 
	//printf("default :");
    }
}		
}
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//
