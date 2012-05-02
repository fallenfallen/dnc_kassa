/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

/****************************************************************************
** ui.h extension file, inclucheckdateed from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

extern Trans Tr; /* = new Transactions("anuta", "plombir", "/var/lib/mysql/mysql.sock", "./log/transchecklist.log");*/
extern SP sp;
extern BlockTimer *bt;

vector<string> n_pclist;
vector<string> checklist;
vector<string> dtime_list;
vector<string> ndoc_list;
vector<string> chtype_list;
vector<string> summ_list;

//==========================================================================//
void checklistForm::init()
{
//puts("checklistForm::init()");
  check = NULL;
  cash = NULL;
  putout_fl = false;
//  fr = NULL;
  checkdatetime = NULL;
  n_docs = NULL;

  n_pclist.clear();
  checklist.clear();
  dtime_list.clear();
  ndoc_list.clear();
  chtype_list.clear();
  summ_list.clear();
  
  DefCodec = QTextCodec::codecForName("UTF8");

  if(Tr.er_con != 0) //printf("state %d\n", Tr.er_con);
  //grabKeyboard();

  fixflag = false;
  adjustSize();
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
  
  InitAccels();
}

//==========================================================================//
void checklistForm::InitAccels()
{
//puts("void checklistForm::InitAccels()");
  QAccel *redj_accel = new QAccel(this); // выход
  redj_accel->connectItem(redj_accel->insertItem(Key_F12), this, SLOT(reject()));
  
  QAccel *accept1_accel = new QAccel(this); //  
  accept1_accel->connectItem(accept1_accel->insertItem(Qt::Key_Enter), this, SLOT(FixCheck()));
  
  QAccel *accept2_accel = new QAccel(this); //  
  accept2_accel->connectItem(accept2_accel->insertItem(Qt::Key_Return), this, SLOT(FixCheck()));
}

//==========================================================================//
void checklistForm::destroy()
{
//puts("checklistForm::destroy()");
  //releaseKeyboard();
  if(check != NULL) free(check);
  if(cash != NULL) free(cash);
  if ( n_docs != NULL ) free(n_docs);
  
//  if(fr != NULL) free(fr);
  if(checkdatetime != NULL) free(checkdatetime);
  n_pclist.clear();
  checklist.clear();
  dtime_list.clear();
  ndoc_list.clear();
  chtype_list.clear();
  summ_list.clear();
}

//==========================================================================//
void checklistForm::SetVariables(int check_number, int n_pc, bool putout)
{
  //puts("checklistForm::SetVariables(int check_number, int n_pc)");
//  //printf("------------------------>>------------> check_number = %d\n", check_number);
//  //printf("------------------------>>------------> n_pc= %d\n", n_pc);
  check =(char *) calloc(5, sizeof(char));
  cash =(char *) calloc(5, sizeof(char));
  strcpy(check, QString::number(check_number));
  strcpy(cash, QString::number(n_pc));
  putout_fl = putout;
//  //printf("------------------------>>------------> check = %s\n", check);
//  //printf("------------------------>>------------> cash= %s\n", cash);
}

//==========================================================================//
int checklistForm::FillForm()
{
    //-----(+)-----Zuskin-----24/10/2011-----
    if (putout_fl) { // если нужен список отложенных ранее чеков
                // вывести список ранее отложенных чеков
	int res = 0;
	if (string(check) == "-1") {
	    res = Tr.Get_PutOut_Checks(&n_pclist, &checklist, &dtime_list, &ndoc_list, &chtype_list, &summ_list);
	}
	else {
	    res = Tr.Get_PutOut_Checks(&n_pclist, &checklist, &dtime_list, &ndoc_list, &chtype_list, &summ_list, string(check));
	}	
	if (res > 0) {
	    for (int jj = 0; jj < res; jj++) {
		QString tstr = DefCodec->toUnicode("РМК №");
		tstr.append(n_pclist[jj]);	
		tstr.append(DefCodec->toUnicode(" чек №"));
		tstr.append(checklist[jj]);
		tstr.append(DefCodec->toUnicode(" от "));
		tstr.append(dtime_list[jj]);
		tstr.append(DefCodec->toUnicode(" на "));
		string sm = summ_list[jj];
		sm.erase(sm.length()-1, 1);
		tstr.append(sm);
		checkListBox->insertItem(tstr); 
	    }
	    return 0;
	}
       return -1;
    }
 //---------------------------------------------------   
    
    //puts("checklistForm::FillForm()");
  int n;
  if(checkdatetime != NULL) { 
      free(checkdatetime); 
      checkdatetime = NULL;
  } //защитимся от повторного выделения памяти
  if ( n_docs != NULL ) {
      free(n_docs);
      n_docs = NULL;
  }
  int k = Tr.Get_Check_List(check, cash, checkdatetime, n, n_docs);
  if( k != 0)     return 1;
  else  {
      QString s;
      int i;
   //   //printf("------------------>>> check = %s\n",check);
   //   //printf("------------------>>> *checkdatetime = %s\n", *checkdatetime);
      for(i = n-1; i >= 0; i--)    {
	//  //printf("------------------>>>            ------------>>>       check = %s\n",check);
	//  //printf("------------------>>>            ------------>>>      *(checkdatetime+%d) = %s\n", i,*(checkdatetime+i));
	  s = DefCodec->toUnicode("чекN ");      
	  s.append(check);
	  s.append(DefCodec->toUnicode(" от "));
	  s.append(*(checkdatetime + i));
	  s.append(DefCodec->toUnicode(" докN "));
	  s.append(n_docs[i]);
	  checkListBox->insertItem(s);    }
  }
  return 0;
}

//==========================================================================//
void checklistForm::GetCheckInfo(char *check_num, char *datetime, char * n_doc)
{
  //puts("checklistForm::GetCheckInfo(char *check, char *datetime)");
  strcpy(check_num, check);
  ////printf("-------------->>----*(checkdatetime + fixed_index) = %s\n",*(checkdatetime+checkListBox->count()-fixed_index-1));

  strcpy(datetime, *(checkdatetime +checkListBox->count()-fixed_index-1));
  //printf("----------\n n_docs[checkListBox->count()-fixed_index-1] = %s \n", n_docs[checkListBox->count()-fixed_index-1]);
  strcpy(n_doc, n_docs[checkListBox->count()-fixed_index-1]);
  
  //printf("check <%s>\n", check);
  //printf("datetime <%s>\n", datetime);
}

//==========================================================================//
void checklistForm::FixCheck()
{
  //puts("checklistForm::FixCheck()");
  fixed_index = checkListBox->currentItem();
  ////printf("-------------->>----fixed_index = %d\n",fixed_index);
  //for(unsigned int i = 0; i<checkListBox->count();i++)
 ////printf("-------------->>----checkListBox->text(%d) = %s\n",i,checkListBox->text(i).ascii());
  fixflag = true;
  accept();
}

//==========================================================================//
void checklistForm::Close()
{
    fixflag = false;
  reject();
}

//==========================================================================//
void checklistForm::keyPressEvent(QKeyEvent *k)
{
//puts("checklistForm::keyPressEvent(QKeyEvent *k)");
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
  switch(k->key())
  {
    case Qt::Key_Enter :
      FixCheck();
      break;
    case Qt::Key_Return :
      FixCheck();
      break;
    case Qt::Key_F12 :
       Close();
    case Qt::Key_Escape :
       Close();       
    case Qt::Key_Up :
  //    SetSelectionUp();
      break;
    case Qt::Key_Down :
  //    SetSelectionDown();
      break;
  }
}



string checklistForm::GetNPC()
{ 
    if (fixflag)
	return n_pclist[fixed_index];
    return "";
}


string checklistForm::GetCheckNum()
{
    if (fixflag)
	return checklist[fixed_index];
    return "";
}


string checklistForm::GetDateTime()
{
    if (fixflag)
	return dtime_list[fixed_index];
    return "";
}


string checklistForm::GetNDoc()
{
    if (fixflag)
	return ndoc_list[fixed_index];
    return "";
}



string checklistForm::GetChType()
{
    if (fixflag)
	return chtype_list[fixed_index];
    return "";

}
