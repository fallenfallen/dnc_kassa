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

#include "hwsighandlerscontroller.h"
#include "const.h"
#include "hwsrv.h"
#include <signal.h>
#include <sys/wait.h>
#include <qtextcodec.h>
#include "additionalrequestForm.h"


extern HWSRV  * hw;

QTextCodec * systemCodec = QTextCodec::codecForName(SYSCODEC);
QTextCodec * hwCodec = QTextCodec::codecForName(HWCODEC);
QTextCodec * dbCodec = QTextCodec::codecForName(DBCODEC);


int NumTrans;
int TypeOperation;
int OriginalCodOper;
QString ReferNum;
double Check_Summ;
double Addit_Summ;

/****************************************************************************/
void npaspayForm::init()
{
    NumTrans = -1;//номер транзакции	//номер чека

    hw->PS_SetCodCurrency(810);	//по умолчанию код валюты - рубли
    hw->PS_SetModeEnterCard(3);	//по умолчанию тип ввода карты неопределен
    
    TypeOperation = 0;//тип операции
    //1 - ОПЛАТА ТОВАРОВ И УСЛУГ
    //2 - ОТМЕНА
    //3 - ВОЗВРАТ
    //4 - СВЕРКА ИТОГОВ
    OriginalCodOper = 1;	//код оригинальной транзакции используется для отмены ( 1 - оплата, 3 - возврат )
    ReferNum = "111111";	//номер ссылки используется для возврата из чека (вводится кассиром)
    
    Check_Summ = 0;	// сумма в копейках
    Addit_Summ = 0;	// сумма в копейках    
    
    status = -1;
    
    connect(HwSignalHandlersController::GetInstance(), SIGNAL(PinPadSignal()), SLOT(CatchSignal()));	    	    
    HwSignalHandlersController::GetInstance()->SetControlledObject(NPASPAY);
    
}

/*****************************************************************************/
void npaspayForm::setOperationType(int type)
{
   // printf("SetOperation(%d)\n", type);
    TypeOperation = type;
}
/*****************************************************************************/
void npaspayForm::setAdditSumm(double s)
{	
    Addit_Summ = s*100;
}
/*****************************************************************************/
void npaspayForm::setRRN(QString s)
{
    if ( s == "" ) return;
    ReferNum = s;
}
/*****************************************************************************/
int npaspayForm::Payment()
{
    
	 //printf("Operaytion type = %d \n", TypeOperation);
	 hw->PS_SetCodeOper(TypeOperation);   
    
	 switch ( TypeOperation )  {	
	 case 1:  {
		//  puts("ОПЛАТА ТОВАРОВ И УСЛУГ");
		  hw->PS_SetSumm(Check_Summ);
		  if ( NumTrans > 0 ) hw->PS_SetUniqNumTrans(NumTrans);  //номер чека
		  break;}
	 case 2:{
		//  puts("ОТМЕНА");  // в течении суток
		  hw->PS_SetSumm(Check_Summ);    // сумма возврата (товара / ов)
		  if ( Check_Summ > 0 )hw->PS_SetAdditSumm(Addit_Summ);	// сумма первоначальная (чека)
		  if ( NumTrans > 0 ) hw->PS_SetUniqNumTrans(NumTrans);//  номер чека   // добавлено условие 19_10_11
		  
		  hw->PS_SetOriginalCodOper(OriginalCodOper);		// оплата - 1
		  break;}
	 case 3:{
		//  puts("ВОЗВРАТ");
		  hw->PS_SetSumm(Check_Summ);		
		  if ( Check_Summ > 0 ) hw->PS_SetAdditSumm(Addit_Summ);
		  if ( NumTrans > 0 ) hw->PS_SetUniqNumTrans(NumTrans);// добавлено условие 19_10_11
		  int TS  = hw->PS_GetTypeSystem();//--------24/02/2012---mirabell
		  // номер ссылки из чека 
		  if (( hw->PS_SetReferNum(ReferNum.ascii()) != 0 ) && ( TS != 2 )){
		      Message->setText(systemCodec->toUnicode("Не верно введен RRN"));
		      return -2;
		  }
		  break;  }
	 default:{
		  Message->setText(systemCodec->toUnicode("Операция не определена"));
		  return -2;
	      }
	 }

	/* puts("===========================");
	 printf("Operation : %d \n", TypeOperation);
	 printf("Uniq Number transaction (refer num) : %d \n", NumTrans);
	 printf("Check Summ : %f \n", Check_Summ);
	 printf("Original code operation : %d \n", OriginalCodOper);
	 printf("AdditSumm : %f \n", Addit_Summ);
	 printf("RRN : %s \n", ReferNum.ascii());
	 puts("===========================");
    */
    
	 int res = hw->PS_SendRequest();
	// puts("отсылаем запрос!");
    
	 if ( res != 0 ){
	//	  puts("Транзакция не выполнена!");
		  QString mess = systemCodec->toUnicode("Транзакция не выполнена:\n");
		  mess = mess+systemCodec->toUnicode(hw->PS_GetErrorDescr(res));
		  Message->setText(mess);
		  return res;
	 }
    
	 puts("Транзакция выполнена!");
    
	 //дополнительные данные для ответа в чек
	 QString data = systemCodec->fromUnicode(hwCodec->toUnicode(hw->PS_GetAddDataAnswer()));	
	// printf(" data = %s\n",data.ascii());
	 //проверяем статус ответа
	 res = hw->PS_GetTransStatus();
	 // printf("res   = %d\n", res);
	 
	     //----------------------------21/02/2012-------------------------------------
	 int TypeSystem = hw->PS_GetTypeSystem();
	 switch (TypeSystem){
	 case 2:{//СБЕР
		 switch(res){
		 case 0:{
			 if (!data.isEmpty()) 
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else 
			     Message->setText(systemCodec->toUnicode("Одобрено"));
			 res = 1;//привязываем хвосты к INPAS а ля одобрено
			 break;
		     }
		 default:{
			 if (!data.isEmpty()) 
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else 
			     Message->setText(systemCodec->toUnicode("Отказано"));
			 res = 16;//привязываем хвосты к INPAS а ля отказано
			 break;
		     }
		 }
		
		 break;}
	 default:{//ИНПАС
		 switch ( res ) {
		 case 0:{
				
			 if (!data.isEmpty()) 
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else
			     Message->setText(systemCodec->toUnicode("Неопределенный статус"));	
			 break;}
		 case 1: {
	
			 if (!data.isEmpty())   
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else	  
			     Message->setText(systemCodec->toUnicode("Одобрено"));
			 break;	}
		 case 16:{
			 if (!data.isEmpty())  
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else	 
			     Message->setText(systemCodec->toUnicode("Отказано"));
			 break;	}
		 case 17:{
			 if (!data.isEmpty()) 
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else	 
			     Message->setText(systemCodec->toUnicode("Выполнено в OFFLINE 1"));
			 break;	}			  
		 case 34:{
			 if (!data.isEmpty())  
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else	  
			     Message->setText(systemCodec->toUnicode("Нет соединения"));
			 break;}
		 case 53:{
			 if (!data.isEmpty())  
			     Message->setText(systemCodec->toUnicode(data.ascii()));
			 else	  
			     Message->setText(systemCodec->toUnicode("Операция прервана"));
			 break;}
		 default:{
			 Message->setText(systemCodec->toUnicode("Статус транзакции имеет неправильное значение!"));
			 break;}
		 }
	     }
	 }
	 return res;
}
/*****************************************************************************/
void npaspayForm::CatchSignal()
{	
    int Mode = hw->PS_GetMode();
    if ( (Mode != 4) && (Mode != 5) ) {
	// create asnwer form 
	additionalrequestForm * f = new additionalrequestForm();
	f->WaitAnswer();
	f->exec();
	delete(f);
    }
}
/*****************************************************************************/
void npaspayForm::setCheckSumm(double sum)
{
    Check_Summ = sum * 100;
}
/*****************************************************************************/
void npaspayForm::setNumCheck(int check_num)
{
    NumTrans = check_num;
}
/*****************************************************************************/
void npaspayForm::setParam()
{
//    hw->PS_SetCodeOper(TypeOperation);
    
}
/*****************************************************************************/
void npaspayForm::destroy()
{
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
}

/*****************************************************************************/
int npaspayForm::OK()
{
    puts("Enter press");
    return 0;
}
/*****************************************************************************/
int npaspayForm::GetStatus()
{
    return status;
}
/*****************************************************************************/
void npaspayForm::keyPressEvent(QKeyEvent * k)
{
    int c_key = k->key(); 
    
    switch ( c_key )    {
        case Qt::Key_F12 :  accept();    break;    
        case Qt::Key_Return :    
	    //status = OK(); 
	    accept();
	    break;   
        case Qt::Key_Enter :    
	    //status = OK();
	    accept();
	    break;   
    }
}

/*****************************************************************************/
void npaspayForm::OkButton_clicked()
{
    accept();
}

/*****************************************************************************/




