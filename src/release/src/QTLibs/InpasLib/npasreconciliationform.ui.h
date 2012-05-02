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

extern HWSRV * hw;


/*-----------------------------------*/
void npasReconciliationForm::init()
{
    grabKeyboard();
    connect(okPushButton, SIGNAL(clicked()), SLOT(Reconciliation()));
}
void npasReconciliationForm::destroy()
{
    releaseKeyboard();
}
/*-----------------------------------*/
void npasReconciliationForm::keyPressEvent(QKeyEvent * k)
{
    switch ( k->key() )    {
    case Qt::Key_F12 :	{    reject();   break;    }
    case Qt::Key_Enter :  {    	    Reconciliation();	    break;	}
    case Qt::Key_Return :  {    	    Reconciliation();	    break;	}    
    }	
}
/*-----------------------------------*/
void npasReconciliationForm::Reconciliation()
{
    
    QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
    QTextCodec * HwCodec = QTextCodec::codecForName(HWCODEC);
    
    //puts("СВЕРКА ИТОГОВ ИНПАС");
    
    hw->PS_SetCodeOper(4);
    hw->PS_SetUniqNumTrans(hw->ECR_GetCheckNumber());
    int res = hw->PS_SendRequest();
    //puts("отсылаем запрос!");
    if ( res != 0 ){
	//puts("Транзакция не выполнена!");
	QString mess = DefCodec->toUnicode("Транзакция не выполнена:\n");
	mess = mess+DefCodec->toUnicode(hw->PS_GetErrorDescr(res));
	statusTextLabel->setText(mess);
	return;
    }
    
    //puts("Транзакция выполнена!");
    
    //дополнительные данные для ответа
    QString data = DefCodec->fromUnicode(HwCodec->toUnicode(hw->PS_GetAddDataAnswer()));	
    //printf(" data = %s\n",data.ascii());
    
    //проверяем статус ответа
    res = hw->PS_GetTransStatus();
    //printf("res   = %d\n", res);
   
    
    //----------------------------21/02/2012-------------------------------------
    int TypeSystem = hw->PS_GetTypeSystem();
    switch(TypeSystem){
    case 2:{
	    //СБЕР
	    switch(res){
		 case 0:{
			 if (!data.isEmpty()) 
			     statusTextLabel->setText(DefCodec->toUnicode(data.ascii()));
			 else 
			     statusTextLabel->setText(DefCodec->toUnicode("Одобрено"));
			 res = 1;
			 break;
		     }
		 default:{
			 if (!data.isEmpty()) 
			     statusTextLabel->setText(DefCodec->toUnicode(data.ascii()));
			 else 
			     statusTextLabel->setText(DefCodec->toUnicode("Отказано"));
			 res = 16;
			 break;
		     }
		 }
	break;}
    default:{
	    switch ( res ) {
	    case 1: {	
		    if (!data.isEmpty())   
			statusTextLabel->setText(DefCodec->toUnicode(data.ascii()));
		    else		  
			statusTextLabel->setText(DefCodec->toUnicode("Одобрено"));
		    break;	}		  
	    case 34:{
		    if (!data.isEmpty())  
			statusTextLabel->setText(DefCodec->toUnicode(data.ascii()));
		    else	  
			statusTextLabel->setText(DefCodec->toUnicode("Нет соединения"));
		    break;}
	    case 53:{
		    if (!data.isEmpty())  
			statusTextLabel->setText(DefCodec->toUnicode(data.ascii()));
		    else	  
			statusTextLabel->setText(DefCodec->toUnicode("Операция прервана"));
		    break;}
	    default:{
		    statusTextLabel->setText(DefCodec->toUnicode("Статус транзакции имеет неправильное значение!"));
		    break;}
	    } 	
	}
	break;
    }
    
    return;    
}
