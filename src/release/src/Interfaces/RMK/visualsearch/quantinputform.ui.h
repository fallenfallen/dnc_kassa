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
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
extern SP sp;
extern BlockTimer *bt;
extern Registration Reg;

//===============================================================//
void quantinputForm::init()
{

    q = 1.00;
    str = "0.00";
    // grabKeyboard();
    adjustSize();
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    connect(quantLineEdit, SIGNAL(textChanged(const QString &)), quantLineEdit, SLOT(deselect()));
    connect(quantLineEdit, SIGNAL(returnPressed()), this, SLOT(SaveValue()));
    connect(quantLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(CheckValue()));
}

//===============================================================//
void quantinputForm::destroy()
{
//puts("quantinputForm::destroy()");
//  releaseKeyboard();
}

/*
//===============================================================//
void quantinputForm::SetValue(double quant)
{
puts("quantinputForm::SetValue(double quant)");
  q = quant;
}*/

//===============================================================//
int quantinputForm::SetValidatorProperties(const char * code)
{
    
   /* int weit = 0;
    double maxq = -1;
    int er = Reg.Get_Max_Quantity(code, weit, maxq);
    
    printf("Reg.Get_Max_Quantity(code = %s, weit = %d , maxq = %f) = %d \n", code, weit, maxq, er);
    
    if ( er != 0 )   return er;
    
    if ( maxq == -1 )      maxq = 9999999.999;
    
    printf("maxq = %d \n", maxq);
    
    //if ( weit == 0 )    {
	 // регистрация штучного товара
    //	QIntValidator * qv_int= new QIntValidator(1, (int)maxq, quantLineEdit);
    //	v = qv_int;
    //}    else     {
	// регистрация весового товара	
	//QDoubleValidator * qv_double = new QDoubleValidator(0.001, maxq, 3, quantLineEdit);
        QDoubleValidator * qv_double = new QDoubleValidator(0.001, 9999999.999, 3, quantLineEdit);
	v = qv_double;
    //}
	quantLineEdit->setValidator(v);
    
    if ( maxq < 0.001 )	return -1;
	
	
    if ( AddWareController::GetInstance()->GetQuantity() > maxq )    {
	if ( maxq >= 1 )
	    AddWareController::GetInstance()->SetQuantity(1.0, 0);
	else
	    AddWareController::GetInstance()->SetQuantity(maxq, 0);	
	QString s;
	quantLineEdit->setText(s.setNum(AddWareController::GetInstance()->GetQuantity(), 'f', 3));
    }*/
    
    QDoubleValidator * qv_double = new QDoubleValidator(0.001, 9999999.999, 3, quantLineEdit);
    v = qv_double;
    quantLineEdit->setValidator(v);
    AddWareController::GetInstance()->SetQuantity(1.0, 0);
    QString s;
    quantLineEdit->setText(s.setNum(AddWareController::GetInstance()->GetQuantity(), 'f', 3));
    return 0;
}

/*
//===============================================================//
double quantinputForm::GetValue()
{
  return q;
}
*/
//===============================================================//
void quantinputForm::SaveValue()
{
    //printf("quantLineEdit->text().toDouble() = %f \n", quantLineEdit->text().toDouble());
    
    if ( quantLineEdit->text().toDouble() > 0 ) {
	//puts("AddWareController::GetInstance()->SetQuantity(quantLineEdit->text().toDouble(), 0)");
	AddWareController::GetInstance()->SetQuantity(quantLineEdit->text().toDouble(), 0);
	//q = quantLineEdit->text().toDouble();
	accept();
    } else reject();
}

//===============================================================//
void quantinputForm::CheckValue()
{
    //puts("quantinputForm::CheckValue()");
    int curpos = quantLineEdit->cursorPosition();
    QString s = quantLineEdit->text();
    switch(v->validate(s, curpos))  {
    case QValidator::Intermediate :
	if(s == "") break;
	else if(s.toDouble() - 0.001 < 0) break; ///??????????
    case QValidator::Invalid :
	quantLineEdit->setText(str);
	break;
	//    case QValidator::Acceptable :
    }
    str = quantLineEdit->text();
}

//===============================================================//
void quantinputForm::keyPressEvent(QKeyEvent *k)
{
    //puts("quantinputForm::keyPressEvent(QKeyEvent *k)");
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    switch ( k->key() )  {
    case Qt::Key_Escape :    {
	    quantLineEdit->setText("");
	    break;
	} 
    case Qt::Key_F12 :    {
	    //q = -1;
	    reject();
	    break;
	}
	
    /*case Qt::Key_Enter :    {
	    printf("Закрываем окно ввода количества\n");
	    accept();
	    break;	
	}	*/
    }
}
