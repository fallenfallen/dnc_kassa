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
#include "addwarecontroller.h"

#include <qstring.h>
#include <qtextcodec.h>
#include <qmessagebox.h>


#include <iostream>

#include "authorization.h"
#include "setup_param.h"
#include "reg_pgsql.h"
#include "statestore.h"
#include "receipt.h"
#include "cdsp.h"
#include "addon_func.h"
#include "hwsrv.h"
#include "const.h"
#include "wareaddition_form.h"

#include "Error.h"



using namespace std;

#define GETRIGHT(right) Authorization::GetInstance()->GetRight(right)

extern SP sp;
extern Registration Reg;
extern Receipt * receipt;
extern HWSRV *hw;


AddWareController AddWareController::instance;

//======================================================================//
AddWareController::AddWareController(QWidget *parent, const char * name) : QObject((QObject *)parent, name)
{
    //puts("AddWareController::AddWareController(QWidget *parent, const char *name)");
    quantity = 1;
    ware_barcode = "";
}

//======================================================================//
AddWareController * AddWareController::GetInstance()
{
    //puts("AddWareController * AddWareController::GetInstance()");
    return &instance;
}

//======================================================================//
double AddWareController::GetQuantity()
{
    //puts("double AddWareController::GetQuantity()");
    return quantity;
}

//======================================================================//
// parental - источник ввода количества. 0 - клавиатурный ввод, 1 - устройство
//
void AddWareController::SetQuantity(double q, int parental)
{
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )    emit AddWareError(74);
    //if ( StateStore::GetInstance()->GetViewState() == REC )    emit AddWareError(45);
    if ( StateStore::GetInstance()->GetViewState() == REC )    return;
    //-----(+)-----Zuskin-----16/12/2011-----
    if ( StateStore::GetInstance()->GetPrevState() == REC )    return;
    //---------------------------------------
    
    else if ( StateStore::GetInstance()->GetReceiptState() == COPY )    emit AddWareError(36);
    else  {
	quantity = Round(q * 1000) / 1000;
//-----(*)-----Zuskin-----04/08/2011-----
//	if  ((quantity < 0.001) || (quantity > 9999999.999) )     {
	if  ((quantity < 0.0009) || (quantity > 9999999.999) )     {
//---------------------------------------	
	    emit AddWareError(55);
	    quantity = 1;
	}
	if ( parental != -1 ) emit QuantitySet(quantity, parental);
    }
}
//======================================================================//
void AddWareController::EscapeQuantity()
{
    //puts("void AddWareController::EscapeQuantity()");
    quantity = 1;
}

//======================================================================//
//  code - строка кода или штрих-кода товара
//  code_type - признак ш.к./кода true - штрих-код,  false - код
//  parental - источник ввода кода. 0 - клавиатурный воод, 1 - сканер, 2 - визуадьный подбор
//-----(*)-----Zuskin-----13/10/2011-----
//-----(*)-----Zuskin-----03/08/2011-----
//int AddWareController::AddWare(const char * code, bool code_type, int parental, const char * addition_id_from_vs)
//int AddWareController::AddWare(const char * code, bool code_type, int parental, const char * addition_id_from_vs, bool price)
//------------------------03/08/2011-----
int AddWareController::AddWare(const char * code, bool code_type, int parental, const char * addition_id_from_vs, bool price, bool qttyset)
//------------------------13/08/2011-----
{
    printf("AddWareController::AddWare(code = %s, code_type = %d, parental = %d, add_id = %s, barcode = %s)\n", code, code_type, parental, addition_id_from_vs, ware_barcode.ascii());
    
    QString add_ware_with_barcode = ware_barcode;
    ware_barcode = "";
    
    //закончилось время смены
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {	emit AddWareError(74);		return -1;    }
    
    //копия чека
    if ( StateStore::GetInstance()->GetReceiptState() == COPY ) {	emit AddWareError(36);		return -2;    }       
    
    // начислена скидка на чек
    if ( StateStore::GetInstance()->GetViewState() == REC ) {
        //-----(-)-----Zuskin-----16/12/2011----- нижеследующее условие выполняется всегда!
	//if ( StateStore::GetInstance()->GetViewState() != INS && StateStore::GetInstance()->GetViewState() != INS_TEXT ) 	
	//---------------------------------------
	emit AddWareError(45);
	return -3;	
    }
    //-----(+)-----Zuskin-----16/12/2011-----
    if ( StateStore::GetInstance()->GetPrevState() == REC )   {
        emit AddWareError(45);
	return -3;	
    }
    //---------------------------------------
    
    // Проверка правильности ввода кода с клавиатуры
    if ( ((parental == 0) && CheckCode(code, code_type)) || (parental != 0) )   {
	// Права доступа проверили
	if ( GetAccess(code_type, parental) )     {
	    int index = -1;
	    er = 0;
	    double tqtty = quantity;	    
	    QString addition_id;
	    
	    if ( atoi(addition_id_from_vs) > 0 ) {
		addition_id = addition_id_from_vs;
//-----(*)-----Zuskin-----03/08/2011-----		
//	    } else if ( !code_type )  {	
	    } else if ( ( !code_type ) && ( !price )) {	
//---------------------------------------
		addition_id = RegistrationByAdditions(code, add_ware_with_barcode);
		if ( addition_id == "-2" ) {
		    er = -100;	// просто закрыли окошко расширений товара
		    return 0;
		}
	    }
	    
	    if ( addition_id == NULL ) addition_id = "-1";
            quantity = tqtty;
            //-----(*)-----Zuskin-----13/10/2011-----
	    //er = AddNewPosition(code, code_type, index, addition_id);
	    er = AddNewPosition(code, code_type, index, addition_id, qttyset);
	    //---------------------------------------
	    //printf("AddNewPosition(code = %s, code_type = %d, index = %d ) return %d \n", code, code_type, index, er);
	    if (er != 0)  {
		if ( er == 3 || er == 12 ) {
		    if ( StateStore::GetInstance()->GetViewState() != INS && StateStore::GetInstance()->GetViewState() != INS_TEXT )  
			emit AddWareError(er);  
		    return er;
		}
		emit AddWareError(er);
		return er;		
	    }
	    
	    bool newsellreceipt = false;
	    
	    if ( (StateStore::GetInstance()->GetReceiptState() !=RETURN) && (StateStore::GetInstance()->GetReceiptState() != SELL) ) {
		StateStore::GetInstance()->SetReceiptState(SELL);
		newsellreceipt = true;
	    }
	    
	    //puts("############################");
	    emit WareAdded(index, code_type, newsellreceipt);
	    //puts("############################");
	    
	    PositionCustomerDisplay(index);
	    EscapeQuantity();
	    
	}	
    }
    
    
    return 0;
}
//======================================================================//
int AddWareController::GetResultAddWare() 
{
    return er;
}
//======================================================================//	
/*//code - код товара (внутренний или штрих-)
//code_type : true - использовать штрих-код товара, false - исрпользовать внутренний код товара
//npos : позиция чека, в которую будет добавлен товар
//returns : 
//0 - все нормально
//1 - продажа данного товара запрещена в карточке товара
//2 - возврат данного товара запрещен в карточке товара
//3 - не верный штрих-код товара 
//4 - не верный код товара
//5 - нет требуемого количества товара
//6 - не верное имя товара в БД 
//7 - запрос в БД не был выполнен, смотреть логфайл
//8 - попытка продать штучный товар как весосой
//9 - истек срок годности
//10 - переполнение при умножении
//11 - переполнение при сложении
//12 - не верный весовой штрих-код товара, товар с указанным кодом не найден*/
//-----(*)-----Zuskin-----13/10/2011-----  
//int AddWareController::AddNewPosition(const char *code, bool code_type, int & npos, const QString & addition_id)  
int AddWareController::AddNewPosition(const char *code, bool code_type, int & npos, const QString & addition_id, bool qttyset)  
//---------------------------------------
{
    //puts("int AddWareController::AddNewPosition(bool code_type, int & npos)");
    int result = 0;
    
    printf("AddNewPosition (code = %s quantity = %f) \n", code, quantity);
    //printf("AddWareController::AddNewPosition(const char *code, bool code_type, int & npos, const QString & addition_id - %s)\n", addition_id.ascii());
    //printf("addition_id.ascii() = %s \n", addition_id.ascii());
    //-----(*)-----Zuskin-----13/10/2011-----  
    //result = Reg.New_Receipt_Record(code, quantity, receipt, code_type, 
	//		 StateStore::GetInstance()->GetReceiptState() == RETURN, false, npos, addition_id.ascii());
    result = Reg.New_Receipt_Record(code, quantity, receipt, code_type, 
			 StateStore::GetInstance()->GetReceiptState() == RETURN, false, npos, addition_id.ascii(), qttyset);	
    //---------------------------------------			 
    if ( (result == 0) && (npos == receipt->record_num - 1) )    Reg.Add_Taxes(receipt, npos);
    return result; 
    
}

//======================================================================//
// returns 
// "0" - выбран сам товар без определения дх
// "-1" - просто закрытое окошко
// ">0" - выбрано расширение
QString AddWareController::RegistrationByAdditions(const char* ware_code, const QString & ware_reg_barcode) {

    printf("AddWareController::RegistrationByAdditions(const char* ware_code = %s) \n", ware_code);
    QString result = "";
    try {
	WareSystem::startInstance();
	
	WareCatalog * catalog = WareSystem::getInstance()->getWareCatalog();
	//cout << "find ware" << endl; 
	Ware* ware = catalog->getWareByCode(ware_code);
	if ( ware->isUseFeaturesSet() ) {
	    WareAdditionForm f;
	    //f.setSetupParam(sp);
	    f.setWareRegBarcode(ware_reg_barcode);
	    int count_addition_with_base = f.setWare(ware);
	    printf("row in additional table  - %d \n", count_addition_with_base);
	    if (count_addition_with_base == 0) return QString("-1");
	    if ( f.exec() == QDialog::Accepted ) {
		result = f.getAdditionId();
		printf(" Registration by addition_id - %s \n", result.ascii());
	    } else result = QString("-2");
	}
	
	WareSystem::finishInstance();
	
    } catch (const Error & err) {
	cout << "Ошибка " << err.getErrorCode() << " " << err.getErrorUserText() << endl;
	//cout << err.getErrorLogText() << endl;
	//QString err_str = toForm("Ошибка № ") + toForm(err.getErrorCode()) + toForm("\n") + toForm("Не удалось получить список расширений товара");
	//QMessageBox::critical(0, toForm("Ошибка"), err_str);
    }
    
    //printf("WareAdditionForm returns addition_id - %s \n", result.ascii());
    double coeff = 1;
    if ( result == "0" ) {
	if ( !ware_reg_barcode.isEmpty() )
	    Reg.GetBarcodeCoefficient(ware_code, ware_reg_barcode.ascii(), coeff);
    }
    if ( result != "-1" ) quantity = coeff;
    
    return result;
}


//##############################--PRIVATE--SECTION--##########################################//

//======================================================================//
bool AddWareController::AuthorizationRequest(int r)
{
//puts("bool AddWareController::AuthorizationRequest(int r)");
  bool result = Authorization::GetInstance()->RequestWithRight(r);
  if(result)
    emit AuthRequest();
  return result;
}

//======================================================================//
bool AddWareController::GetAccess(bool code_type, int parental)
{
//puts("bool AddWareController::GetAccess(bool code_type, int parental)");
  bool access_flag = true;
  
  //мозгоимение ((( 
  
  if((parental == 1) && !GETRIGHT(1)) // Регистрация сканером ш.-к.
    access_flag = AuthorizationRequest(1);
    
  else if((parental == 2) && !GETRIGHT(3)) // Регистрация через визуальный подбор
    access_flag = AuthorizationRequest(3);
    
  else if((parental == 0) && code_type && !GETRIGHT(2)) // Регистрация ш.-к. с клавиатуры
    access_flag = AuthorizationRequest(2);
    
  else if((parental == 0) && !code_type && !GETRIGHT(0)) // Регистрация по внутреннему коду
    access_flag = AuthorizationRequest(0);
    
  if(!access_flag)
    emit AddWareError(72);
    
  return access_flag;
}

//======================================================================//
bool AddWareController::CheckCode(const char * code, bool code_type)
{
    //puts("bool AddWareController::CheckCode(bool code_type)	");
    //проверка штрих-кода
    if ( code_type && (strlen(code) > MAXLENBARCODE) )   {
	emit AddWareError(54);
	return false;
    }	
    /*
    if ( !code_type )  {
	QString s = code;
	bool nf = true;
	int id = s.toInt(&nf);
	if ( !nf )    {
	    emit Error(56);
	    return false;
	}
    }*/
    return true;
}


//=====================================================================//
void AddWareController::PositionCustomerDisplay(int pos)
{
    //puts("void AddWareController::PositionCustomerDisplay(int pos)");
    if(sp.cdsp_detalize_product)
    {
	QTextCodec * DbCodec, * HwCodec;
	DbCodec = QTextCodec::codecForName(DBCODEC);
	HwCodec = QTextCodec::codecForName(HWCODEC);
	
	//printf("receipt->GetPosElemRef(pos = %d)->price = %f \n", pos, receipt->GetPosElemRef(pos)->price);
	double price = receipt->GetPosElemRef(pos)->price;
	double sum = Round(quantity * price * 100) / 100;
	double total = receipt->GetResultSum();
	
	QString prod_name, p_str, q_str, sum_str, total_str;
	
	prod_name = HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetPosElemRef(pos)->GetLongtitle()));
	p_str.setNum(price, 'f', 2);
	q_str.setNum(quantity, 'f', 3);
	sum_str.setNum(sum, 'f', 2);
	total_str.setNum(total, 'f', 2);
	
	//const char * name_str, const char * price_str, const char * quant_str, const char * sum_str, const char * total_str, HWSRV * hw_ref
	CDSP cdsp(prod_name.ascii(), p_str.ascii(), q_str.ascii(), sum_str.ascii(), total_str.ascii(), hw);
	cdsp.RegistrationPositionCustomerDisplay();
    }
}

//=====================================================================//


