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
#include "addon_func.h"
#include "hwsrv.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "dedicationcombobox.h"
#include "messages.h"
#include <math.h>
#include <cstdlib>

DedicationComboBox * Type_comboBox;


extern HWSRV  * hw;
extern PgRef M;
extern bool addautoredform_flag;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

//bool addautoredform_flag = false;


void AddAutoRedform::AddComboBoxes()
{
    Type_comboBox = new DedicationComboBox(groupBox45, "r");
    Type_comboBox->setGeometry(125,125,268,30);
    Type_comboBox->insertItem(systemCodec->toUnicode("Процентная скидка"));
    Type_comboBox->insertItem(systemCodec->toUnicode("Скидка суммой"));
    Type_comboBox->insertItem(systemCodec->toUnicode("Процентная надбавка"));
    Type_comboBox->insertItem(systemCodec->toUnicode("Надбавка суммой"));
}


void AddAutoRedform::SetTabOrderafterCreateComboBox()
{
   // puts("SetTabOrderafterCreateComboBox()");
    setTabOrder(Name_lineEdit,ShortName_lineEdit );
    setTabOrder(ShortName_lineEdit,Values_lineEdit );
    setTabOrder(Values_lineEdit,Type_comboBox );
    setTabOrder(Type_comboBox, Begin_dateEdit);
    setTabOrder(Begin_dateEdit, End_dateEdit);
    setTabOrder(End_dateEdit, Begin_timeEdit);
    setTabOrder(Begin_timeEdit,End_timeEdit );
    setTabOrder(End_timeEdit, Monday_checkBox);
    setTabOrder(Monday_checkBox, Tuesday_checkBox);
    setTabOrder(Tuesday_checkBox,Wednesday_checkBox );
    setTabOrder(Wednesday_checkBox, Thursday_checkBox);
    setTabOrder(Thursday_checkBox, Friday_checkBox);
    setTabOrder(Friday_checkBox, Saturday_checkBox);
    setTabOrder(Saturday_checkBox, Sunday_checkBox);
    setTabOrder(Sunday_checkBox,Quantity_lineEdit );
    setTabOrder(Quantity_lineEdit, PositionSum_lineEdit);
    setTabOrder(PositionSum_lineEdit, CheckSum_lineEdit);
    setTabOrder(CheckSum_lineEdit,Pref1_lineEdit );
    setTabOrder(Pref1_lineEdit, Pref2_lineEdit);
    setTabOrder(Pref2_lineEdit,ClearAll_Button );
    setTabOrder(ClearAll_Button, OK);
    setTabOrder(OK,Exit_Button );
}




void AddAutoRedform::init()
{
   // puts("void AddAutoRedform::init()");
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
    AddComboBoxes();
    SetTabOrderafterCreateComboBox();
    ClearAll_Button_clicked();
    SetValidatorProperties();
    addautoredform_flag = true;
    Name_lineEdit->setFocus();//установить фокус текущим
    
    Begin_dateEdit->setDate(QDate::currentDate());
    End_dateEdit->setDate(QDate::currentDate());
    Begin_timeEdit->setTime(QTime::currentTime());
    End_timeEdit->setTime(QTime::currentTime());
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    //grabKeyboard();
}

void AddAutoRedform::Myinit(int par, const char * str)
{
   //puts("void AddAutoRedform::Myinit(AutoSchemeId)");
    
    Param = par;
    if (Param == 1)
    {
//	puts("Это создание скидки");
	AutoSchemeId = str;
    }
    else
    {
//	puts("Это редактирование скидки");
	LoadInfoAboutRed(str);
    }
}



void AddAutoRedform::closeEvent( QCloseEvent* e )
{
 //   puts("void AddAutoRedform::closeEvent( QCloseEvent* e )");
    e->accept();
    addautoredform_flag = false;
    return;
}

void AddAutoRedform::SetValidatorProperties(void)
{
  //  puts("void AddAutoRedform::SetValidatorProperties(void)");
    
    QRegExp pricerx("^\\d{1,9}\\.\\d{2}$");
    pricevalidator = new QRegExpValidator(pricerx, this);
    PositionSum_lineEdit->setValidator(pricevalidator);
    Values_lineEdit->setValidator(pricevalidator);
    
    QRegExp check_sumrx("^\\d{1,11}\\.\\d{2}$");
    check_sumvalidator = new QRegExpValidator(check_sumrx, this);
    CheckSum_lineEdit->setValidator(check_sumvalidator );
    
    QRegExp quantityrx("^\\d{1,9}\\.\\d{3}$");
    quantityvalidator = new QRegExpValidator(quantityrx, this);
    Quantity_lineEdit->setValidator(quantityvalidator);
	    
    QRegExp prefrx("(\\d{0,9}){0,3}");
    prefvalidator = new QRegExpValidator(prefrx, this);
    Pref1_lineEdit->setValidator(prefvalidator);
    Pref2_lineEdit->setValidator(prefvalidator);
    
    QRegExp titlerx("(\\w|[ ,.-]){0,99}");
    titlevalidator = new QRegExpValidator(titlerx, this);
    Name_lineEdit->setValidator(titlevalidator);
    ShortName_lineEdit->setValidator(titlevalidator);
    
}


void AddAutoRedform::LoadInfoAboutRed(QString id)
{
//    puts("void AddAutoRedform::LoadInfoAboutRed()");    
    int i;    
    int col_fields = M.GetQuantityFieldsAutoReduction();
    char ** mas = (char**) calloc( col_fields, sizeof(char*));
    for ( i=0; i< col_fields; ++i) 
    {
	mas[i] = (char*) calloc(201, sizeof(char));
    }
    M.LoadAutoRedInfoWithAutoSchemeId(id.ascii(), mas);
    AutoSchemeId = mas[0];
    AutoRedId = id;
    Name_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[1])));
    ShortName_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[2])));
    Values_lineEdit->setText(QString(mas[3]));
    Type_comboBox->setCurrentItem(atoi(mas[4]));
     
    char * bbb = (char *) calloc(strlen(mas[5]),sizeof(char));
    if (strcmp(mas[5],"")!=0)
	strcpy(bbb,mas[5]);
    else
	strcpy(bbb,"00:00:00");
    char * datefrombd = (char *) calloc(11,sizeof(char));
    for (int k = 0; k<10; k++)
	*(datefrombd+k) = *(bbb+k);
    QDate  date; 
    Begin_dateEdit->setDate(date.fromString(datefrombd,ISODate));

    
    if (strcmp(mas[6],"")!=0)
	strcpy(bbb,mas[6]);
    else
	strcpy(bbb,"00:00:00");
    for (int k = 0; k<10; k++)
	*(datefrombd+k) = *(bbb+k);
    End_dateEdit->setDate(date.fromString(datefrombd,ISODate));

    strcpy(bbb,mas[7]);
    char * timefrombd = (char *) calloc(11,sizeof(char));
    for (int k = 0; k<10; k++)
	*(timefrombd+k) = *(bbb+k);
    QTime  time; 
    Begin_timeEdit->setTime(time.fromString(timefrombd,ISODate));
    
     strcpy(bbb,mas[8]);
    for (int k = 0; k<10; k++)
	*(timefrombd+k) = *(bbb+k);
    End_timeEdit->setTime(time.fromString(timefrombd,ISODate));
    free(bbb);
    free(timefrombd);
    
    LoadFlags(mas[9]);
    
    Quantity_lineEdit->setText(QString(mas[10]));
    PositionSum_lineEdit->setText(QString(mas[11]));
    CheckSum_lineEdit->setText(QString(mas[12]));	    
    Pref1_lineEdit->setText(QString(mas[13])); 
    Pref2_lineEdit->setText(QString(mas[14]));
	
    int c = col_fields;
    while (--c=0) 
    {
//	printf("%s free \n", mas[c]);
	free(mas[c]);    
    }
    free(mas);
    
}


int AddAutoRedform::Contingencies(void)
{
//    puts("int AddAutoRedform::Contingencies(void)");
    QString str;
    
    QString er_str;
    QString er_id;
    
    //ограничения на наименование
    str = dbaseCodec->fromUnicode(Name_lineEdit->text());
     if (str == "")
    {
	 WareReferenceErrorText(1, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
     }
     if (Param == 1)
     {
	 char * value = (char *) calloc(201,sizeof(char));
	 int n = M.GetAutoRedIdByName(str.ascii(),value);
	 str = value;
	 free(value);
	 if (str == "ERROR")
	 {
	     WareReferenceErrorText(2, er_str, er_id);
	     InterfaceErrorWin(er_str, er_id);
	     return -1;  
	 }
	 if (n!=0)
	 {
	     WareReferenceErrorText(3, er_str, er_id);
	     InterfaceErrorWin(er_str, er_id);
	     return -1; 
	 }
     }
    //ограничения на текст для чека
     str = systemCodec->fromUnicode(ShortName_lineEdit->text());
     if (str == "")
     {
	 WareReferenceErrorText(4, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
     }
     
    //ограничения на значение
    str = Values_lineEdit->text();
    if (str == "")
    {
	WareReferenceErrorText(5, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
    }
  //  printf("Type_comboBox->currentItem() = %d\n",Type_comboBox->currentItem());
    if (((Type_comboBox->currentItem() == 0) || (Type_comboBox->currentItem() == 2)) && (str.toDouble()>100))
    {
	//процентная скидка
	WareReferenceErrorText(6, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1; 
    }
    return 0;  
}




void AddAutoRedform::ClearAll_Button_clicked()
{
  //  puts("void AddAutoRedform::ClearAll_Button_clicked()");
    Name_lineEdit->setText("");
    ShortName_lineEdit->setText("");
    Values_lineEdit->setText("0");
    Type_comboBox->setCurrentItem(0);
    QString datefrombd = "0000-00-00";
    QDate  date; 
    Begin_dateEdit->setDate(date.fromString(datefrombd,ISODate));
    End_dateEdit->setDate(date.fromString(datefrombd,ISODate));
    Begin_timeEdit->setTime(QTime(00,00,00));
    End_timeEdit->setTime(QTime(00,00,00));
    Quantity_lineEdit->setText("0");
    PositionSum_lineEdit->setText("0");
    CheckSum_lineEdit->setText("0");
    Pref1_lineEdit->setText("0");
    Pref2_lineEdit->setText("0");
    
    Monday_checkBox->setChecked(FALSE);
    Tuesday_checkBox->setChecked(FALSE);
    Wednesday_checkBox->setChecked(FALSE);
    Thursday_checkBox->setChecked(FALSE);
    Friday_checkBox->setChecked(FALSE);
    Saturday_checkBox->setChecked(FALSE);
    Sunday_checkBox->setChecked(FALSE);
}


QString AddAutoRedform::TransformationData(QDate date)
{
    QDate str =date;
    QString string;
    QString Empty = "00.00.0000";
    if (str.toString("dd.MM.yyyy") == "")
	string = "NULL";
    else
	string = "CAST('" + str.toString("dd.MM.yyyy")+"' AS DATE )";
    return  string;
}


QString AddAutoRedform::TransformationTime(QTime time)
{
    QTime str =time;
    QString string = "CAST('" + str.toString("hh:mm:ss")+"' AS TIME )";
    return  string;
}

void AddAutoRedform::Exit_Button_clicked()
{
//    puts("void AddAutoRedform::Exit_Button_clicked()");
    close();
}

//преобразование для флагов
QString AddAutoRedform::transformationflags(void)
{
  //    printf("char * AddAutoRedform::transformationflags()\n");  
      param = (char *) calloc (14,sizeof(char));
      if (Monday_checkBox->isChecked())
	  strcpy(param,"1");
      else
	  strcpy(param,"0");  
      strcat(param,",");
      if (Tuesday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");  	  
      strcat(param,",");
      if (Wednesday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");  
      strcat(param,",");
      if (Thursday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");     
      strcat(param,",");
      if (Friday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");        
      strcat(param,",");
      if (Saturday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");  
      strcat(param,",");
      if (Sunday_checkBox->isChecked())
	  strcat(param,"1");
      else
	  strcat(param,"0");  
      SaveFlags(param);
      QString str = param;
      free(param);
      return str;
}

void AddAutoRedform::SaveFlags(char * param)
// 0 -- не верный формат параметра
{
//  puts( "void AddAutoRedform::SaveFlags(char * param)\n");
  bool wrong_par_flag = false;
   //Прочитаем все параметры-флаги
  int fls[9];
  int k = 0;
  for(unsigned int i = 0; i < strlen(param); i+=2)
  {
    if( ( ( *(param + i) == '0' ) || ( *(param + i) == '1' ) ) && ( ( *(param + i + 1) == ',' ) || ( *(param + i + 1) == '\0' ) ) )
    {
       fls[k] = (int) ( *(param+i) - 48 );
       k++;
    }
    else
    {
      wrong_par_flag = true;
      break;
    }
  }
  if (wrong_par_flag)
  {
    flags = (char *) calloc(2, sizeof(char));
    strcpy(flags, "0");
  }
  else
  {
    // Побитово закодируем флаги
    int bitfl = 0;
    for (int i = 0; i < 9; i ++)
	if(fls[i] != 0)
        bitfl = bitfl | (int)pow(2, i);

    // Полученное число преобразуем в строку
    char *strfl = itoa(bitfl); // Не забыть очистить память
    flags = (char *) calloc(strlen(strfl) + 1, sizeof(char));
    strcpy(flags, strfl);
   strcpy(param,flags);
    free(strfl);                    // Очищение памяти
  }
}


void AddAutoRedform::OK_clicked()
{
 if (Contingencies() == 0)
    {
	QString auto_scheme_id = AutoSchemeId;
	QString longtitle = dbaseCodec->fromUnicode(Name_lineEdit->text());
	QString shortname = dbaseCodec->fromUnicode(ShortName_lineEdit->text());
	QString value = Values_lineEdit->text();
	QString type;
	if (Type_comboBox->currentItem() == 0)
	    type = "0";
	if (Type_comboBox->currentItem() == 1)
	    type = "1";
	if (Type_comboBox->currentItem() == 2)
	    type = "2";
	if (Type_comboBox->currentItem() == 3)
	    type = "3";
	//printf("!!!!!!!  type = %s\n",type.ascii());
	QString begindate = TransformationData(Begin_dateEdit->date());
	if (begindate == NULL)
	{
	//    puts("datebegin == NULL");
	    begindate = "00:00:00";
	}
	QString enddate = TransformationData(End_dateEdit->date());
	if (enddate == NULL)
	{
	//    puts("dateend == NULL");
	    enddate = "00:00:00";
	}
	QString begintime = TransformationTime(Begin_timeEdit->time());
	QString endtime= TransformationTime(End_timeEdit->time());
	QString weekdays= transformationflags();
	QString quantity = Quantity_lineEdit->text();
	QString position_sum = PositionSum_lineEdit->text();
	QString check_sum = CheckSum_lineEdit->text();
	QString card_prefix_begin = Pref1_lineEdit->text();
	QString card_prefix_end = Pref2_lineEdit->text();
	QString mes_str;
	QString mes_id;
	if (Param == 1)
	{
	    QString codered =itoa(M.FindNextAutoRedIdFromAutoRed()+1);
	    QString Str = codered+","+ auto_scheme_id + ",'"+longtitle +"','"+shortname + "',"+value + "," + type +","+begindate +","+enddate +","+ begintime +","+endtime +","+weekdays +","+quantity +","+position_sum +","+check_sum +","+card_prefix_begin +","+card_prefix_end;
	    //запрос о создании скидки
	    int g = M.Create_Auto_Reduction(Str.ascii());
	//    printf("g = %d\n",g);
	    if (g == 1)
	    {
		WareReferenceMessageText(1,mes_str,mes_id);
		InterfaceMessageWin(mes_str,mes_id);
		close();
	    }
	    
	}
	else
	{
	    int g = M.UpdateAutoReduction(AutoRedId.ascii(), auto_scheme_id.ascii(), longtitle.ascii(), shortname.ascii(),  value.ascii(), type.ascii(), begindate.ascii(), enddate.ascii(), begintime.ascii(), endtime.ascii(), weekdays.ascii(), quantity.ascii(), position_sum.ascii(), check_sum.ascii(), card_prefix_begin.ascii(), card_prefix_end.ascii());
	
	//    printf("g = %d\n",g);
	    if (g == 0)
	    {
		WareReferenceMessageText(3,mes_str,mes_id);
		InterfaceMessageWin(mes_str,mes_id);
		close();
	    }
	}
    } 
}


void AddAutoRedform::LoadFlags(char * str)
{
   // puts("void AddAutoRedform::LoadFlags(char * str)");
    unsigned long int k =(unsigned long int)(atoi(str));
    int massiv[7];
    for (int i = 0;i<7;i++)
    {
	massiv[i] = k&1;
	k =k>>1;
	//printf("massiv[%d] = %d\n",i,massiv[i]);
    }
    if (massiv[0] == 0)
	Monday_checkBox->setChecked(FALSE);
    else
	Monday_checkBox->setChecked(TRUE);
    if (massiv[1] == 0)
	Tuesday_checkBox->setChecked(FALSE);
    else
	Tuesday_checkBox->setChecked(TRUE);
        if (massiv[2] == 0)
	Wednesday_checkBox->setChecked(FALSE);
    else
	Wednesday_checkBox->setChecked(TRUE);
        if (massiv[3] == 0)
	Thursday_checkBox->setChecked(FALSE);
    else
	Thursday_checkBox->setChecked(TRUE);
        if (massiv[4] == 0)
	Friday_checkBox->setChecked(FALSE);
    else
	Friday_checkBox->setChecked(TRUE);
        if (massiv[5] == 0)
	Saturday_checkBox->setChecked(FALSE);
    else
	Saturday_checkBox->setChecked(TRUE);
        if (massiv[6] == 0)
	Sunday_checkBox->setChecked(FALSE);
    else
	Sunday_checkBox->setChecked(TRUE);
}







//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------/
void AddAutoRedform::GetKeySequence()
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


void AddAutoRedform::keyPressEvent(QKeyEvent *k)
{
 //   printf("--------------- press k->key = %d ---------------\n", k->key());
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
    if ( state == 5 )     { GetKeySequence();  }
}

void AddAutoRedform::makeKeyPress(QKeyEvent *k)
{
   // puts("void AddAutoRedform::makeKeyPress(QKeyEvent *k)");
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
	   if (ClearAll_Button->hasFocus())
			 ClearAll_Button_clicked();
		if (OK->hasFocus())
			 OK_clicked();
		if (Exit_Button->hasFocus())
			 Exit_Button_clicked();
	    break;
	}
    case Qt::Key_Return :
	{
	//    printf("Qt::Key_Return");
		if (ClearAll_Button->hasFocus())
			 ClearAll_Button_clicked();
		if (OK->hasFocus())
			 OK_clicked();
		if (Exit_Button->hasFocus())
			 Exit_Button_clicked();
				
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
	  //  printf("Qt::Key_Up");
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


