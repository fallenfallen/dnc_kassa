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
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

extern Registration Reg;
extern SP sp;
extern BlockTimer *bt;

//====================================================================//
void fixdiscountForm::init()
{
////puts("fixdiscountForm::init()");
DefCodec = QTextCodec::codecForName(SYSCODEC);
DbCodec = QTextCodec::codecForName(DBCODEC); 
grabKeyboard();
  InitTable();
  adjustSize(); 
  
  flag = false;
  type = 0;
  value = 0;
  
  area = NULL;
  code = NULL;
  name = NULL;
  shortcut = NULL;
  card = NULL;
  incode = NULL;
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime(); 
}

//====================================================================//
void fixdiscountForm::InitTable()
{
//puts("fixdiscountForm::InitTable()");
  /*fixdiscTable->setColumnReadOnly(0, true);
  fixdiscTable->setColumnReadOnly(1, true);
  fixdiscTable->setColumnReadOnly(2, true);
  fixdiscTable->setColumnReadOnly(3, true);
  fixdiscTable->setColumnReadOnly(4, true);*/
    fixdiscTable->setReadOnly(true);
  
  fixdiscTable->setColumnWidth(0, 50);
  fixdiscTable->setColumnWidth(1, 400);
  fixdiscTable->setColumnWidth(2, 120); 
  fixdiscTable->setColumnWidth(3, 10);
  fixdiscTable->setColumnWidth(4, 120); 
  
  fixdiscTable->hideColumn(3);
  fixdiscTable->hideColumn(4);
  
  fixdiscTable->hideColumn(5);
  fixdiscTable->hideColumn(6);
  
  
  fixdiscTable->setSelectionMode(QTable::SingleRow);
}

//====================================================================//
void fixdiscountForm::destroy()
{
//puts("fixdiscountForm::destroy()");
  if(area) free(area);
  if(code) free(code);
  if(name) free(name);
  if(shortcut) free(shortcut);
  if(card) free(card);
  if(incode) free(incode);
  releaseKeyboard();
  //puts("end of fixdiscountForm::destroy()");
}

//====================================================================//
void fixdiscountForm::SetArea(char ar)
{
//puts("fixdiscountForm::SetArea(char ar)");
  area = (char *) calloc(2, sizeof(char));
//  strcpy(area, ar);
  *area = ar;
 
  if(strcmp(area ,"t") != 0) textTextLabel->setText(DefCodec->toUnicode("Выберите скидку на позицию"));
  else textTextLabel->setText(DefCodec->toUnicode("Выберите скидку на чек"));
}

//====================================================================//
void fixdiscountForm::AddRowInTable(char *code, char *name, char *value, char *shortcut, int type, char * icode, char* card_code )
{
//puts("fixdiscountForm::AddRowInTable(char *code, char *name, char *value, char *shortcut, int type)");
  QString s;
  int i = fixdiscTable->numRows();  
  fixdiscTable->setNumRows(i + 1);

  QTableItem *i0 = new QTableItem(fixdiscTable, QTableItem::Never, code);  
  fixdiscTable->setItem(i, 0, i0);

  QTableItem *i1 = new QTableItem(fixdiscTable, QTableItem::Never, DbCodec->toUnicode(name));  
  fixdiscTable->setItem(i, 1, i1);

  QTableItem *i2 = new QTableItem(fixdiscTable, QTableItem::Never, DbCodec->toUnicode(value));  
  fixdiscTable->setItem(i, 2, i2);

  QTableItem *i3 = new QTableItem(fixdiscTable, QTableItem::Never, QString::number(type));  
  fixdiscTable->setItem(i, 3, i3);

  QTableItem *i4 = new QTableItem(fixdiscTable, QTableItem::Never, DbCodec->toUnicode(shortcut));  
  fixdiscTable->setItem(i, 4, i4);
  
  QTableItem *i5 = new QTableItem(fixdiscTable, QTableItem::Never, DbCodec->toUnicode(icode));  
  fixdiscTable->setItem(i, 5, i5);
  
  QTableItem *i6 = new QTableItem(fixdiscTable, QTableItem::Never, DbCodec->toUnicode(card_code));  
  fixdiscTable->setItem(i, 6, i6);
}

//====================================================================//
void fixdiscountForm::SetSelectionUp()
{    
//puts("fixdiscountForm::SetSelectionUp()");
 QTableSelection sel = fixdiscTable -> selection(0);
 int p_sel = sel.topRow();
 if((p_sel != 0) && (p_sel != -1))
  {
 fixdiscTable -> clearSelection();
   sel.init(--p_sel, 0);
   sel.expandTo(p_sel, 2);
   fixdiscTable->addSelection(sel);
   fixdiscTable->ensureCellVisible(p_sel,0); 
 }
}

//====================================================================//
void fixdiscountForm::SetSelectionDown()
{
 //puts("fixdiscountForm::SetSelectionDown()");
 QTableSelection sel = fixdiscTable -> selection(0);
 int p_sel = sel.topRow();
  if((p_sel != fixdiscTable->numRows() - 1) && (p_sel != -1))
  {
   fixdiscTable -> clearSelection();
    sel.init(++p_sel, 0);
    sel.expandTo(p_sel, 2);
    fixdiscTable->addSelection(sel);
    fixdiscTable->ensureCellVisible(p_sel,0); 
  }
}

//====================================================================//
void fixdiscountForm::ClearTable()
{
//puts("fixdiscountForm::ClearFixdiscTable()");
  for(int i = 0; i < fixdiscTable->numRows(); i++)
    fixdiscTable->removeRow(i);
  fixdiscTable->setNumRows(0);
} 

//====================================================================//
int fixdiscountForm::FillTable()
{
    //puts("fixdiscountForm::FillTable()");
    
    int numrow = Reg.Get_List(area);
    // Проверить на доступности БД
    if ( numrow == -1 )  return -1;
    
    //Проверить, что список фиксированных скидок не пуст
    if ( numrow == 0 )      return -2;
    
    if ( numrow > 0 )  {
	char * c = NULL;
	char * n = NULL;
	char * s = NULL;
	char * v = NULL;
	char * t = NULL;
	char * cc = NULL;
	type = 0;
	for(int i = 0; i < numrow; i++)	    {
	    //Внимание! память под строки выделяется внутри функции
	    Reg.Get_List_Position(c, v, n, s, &type, t, cc, i, numrow); 
	    
	    //-----(*)-----Zuskin-----28/07/2011-----
	    //if ( type == 0 || type == 2)    strcat(v, "%");
	    if ( type == 0 || type == 2 || type == 4)    strcat(v, "%");
	    //-------------------------------------------------
	    else strcat(v, " ");
	    AddRowInTable(c, n, v, s, type, t, cc);
	}
	fixdiscTable->adjustColumn(0);
	fixdiscTable->adjustColumn(1);
	fixdiscTable->adjustColumn(2);
	//fixdiscTable->adjustSize();
	
	QTableSelection sel = fixdiscTable->selection(0);
	int p_sel = sel.topRow();
	sel.init(p_sel, 0);
	sel.expandTo(p_sel, 2);  
	fixdiscTable->removeSelection(sel);
	
	p_sel = 0;
	sel.init(p_sel, 0);
	sel.expandTo(p_sel, 2);
	fixdiscTable->addSelection(sel); 
	fixdiscTable->ensureCellVisible(p_sel, 0);
    }
    return 0;
}

//====================================================================//
void fixdiscountForm::SaveChoisedPosition()
{
//puts("fixdiscountForm::SaveChoisedPosition()");
  QTableSelection sel = fixdiscTable->selection(0);
  int p_sel = sel.topRow();

  code = (char *) calloc( (fixdiscTable->text(p_sel, 0).length()) * 2 + 1, sizeof(char));
  name = (char *) calloc( (fixdiscTable->text(p_sel, 1).length()) * 2 + 1, sizeof(char));
  shortcut = (char *) calloc( (fixdiscTable->text(p_sel, 4).length()) * 2 + 1, sizeof(char));
  
  incode = (char *) calloc( (fixdiscTable->text(p_sel, 5).length()) * 2 + 1, sizeof(char));
  card = (char *) calloc( (fixdiscTable->text(p_sel, 6).length()) * 2 + 1, sizeof(char));
  
  
  strcpy(code, DbCodec->fromUnicode(fixdiscTable->text(p_sel, 0)));  
  printf("code = %s \n", code);
  strcpy(name, DbCodec->fromUnicode(fixdiscTable->text(p_sel, 1)));  
  strcpy(shortcut, DbCodec->fromUnicode(fixdiscTable->text(p_sel, 4)));
  strcpy(incode, DbCodec->fromUnicode(fixdiscTable->text(p_sel, 5)));
  strcpy(card, DbCodec->fromUnicode(fixdiscTable->text(p_sel, 6)));
  
  bool f;
  QString value_str = fixdiscTable->text(p_sel, 2); 
  value_str.remove('%');
  value = value_str.toDouble(&f); 
  type = fixdiscTable->text(p_sel, 3).toInt(&f);
  flag = true;
}

//====================================================================//
void fixdiscountForm::GetChoisedPosition(QString &c, QString &n, QString &s, QString &ic, QString &cc, int *t, double *v)
{
//puts("fixdiscountForm::GetChoisedPosition(QString &c, QString &n, QString &s, int *t, double *v)");
  if(flag)
  {
    c = code;
    n = name;
    s = shortcut;
//printf("значение скидки возвращаемое %f\n", value);
    *v = value;
    *t = type;
    ic = incode;
    cc = card;
  }
 // else {//puts("Not Flag"); printf("%d\n", *t);}
}

//====================================================================//
void fixdiscountForm::keyPressEvent(QKeyEvent *k)
{
//puts("fixdiscountForm::keyPressEvent(QKeyEvent *k)");
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
  switch(k->key())
  {
    case Qt::Key_Up :
    {
      SetSelectionUp();
      break;      
    }
    case Qt::Key_Down :
    {
      SetSelectionDown();
      break;      
    }
    case Qt::Key_Enter :
    {
      SaveChoisedPosition();
      if(flag) close();
      break;
    }
    case Qt::Key_Return :
    {
      SaveChoisedPosition();
      if(flag) close();
      break;
    } 
    case Qt::Key_F12 :
    {
      close();
      break;
    }
  }
}
