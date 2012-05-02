/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
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
#include "authorization.h"
//#include "insert_transaction.h"
#include "blocktimer.h"

#include <qtextcodec.h>
#include <qmessagebox.h> 
#include <qfocusdata.h>
#include <qmap.h>

#include "messages.h"
#include "trans_pgsql.h"
#include "reg_pgsql.h"
#include "setup_param.h"
#include "addon_func.h"
#include "const.h"
#include "accessrights.h" 
#include "const_conf.h"
#include "npasreconciliationform.h"


const int Z_REPORT_TYPE =  1; 		// z -отчет
const int X_REPORT_TYPE = 2;		// x-отчет
const int SEC_REPORT_TYPE = 7;		// по секциям
const int CASH_REPORT_TYPE = 8;		// по кассирам
const int TIME_REPORT_TYPE = 10;	// почасовой
const int QUANT_REPORT_TYPE = 11;	// количеств
const int WARE_REPORT_TYPE = 12;     // по товарам
const int CLOSE_SESSION_TYPE = 15;	//закрытие смены



int res;
bool f;

extern Trans Tr;

extern SP sp;
extern BlockTimer *bt;

extern HWSRV * hw;

extern DebugLevelLog regformlog;
extern char * fileflag_path;


int current_type = 2;

QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
QTextCodec * DbCodec = QTextCodec::codecForName(DBCODEC);
QTextCodec * HwCodec = QTextCodec::codecForName(HWCODEC);


//====================================================================//
void reportForm::init()
{
    
    //puts("void reportForm::SetParams(HWSRV *hw)");
    hw  = hw;
    kkm_connect = false;
    
    ins_tr = new InsertTransactions(hw, NULL, &Tr, &regformlog);
    ins_tr->SetSP(&sp);
    ins_tr->SetUserId(Authorization::GetInstance()->GetUid());
    
    res = hw->ECR_ReadStatus();
    //printf("service! GetStatus res: %d\n", res);
    if(res != 0)  {    Kkm_Error(res);    kkm_connect = false;  }
    else  {    
	kkm_connect = true;
	res = hw->ECR_SetMode(0);
	if ( res != 0 ) Kkm_Error(res);
    }
    //-----(+)-----Zuskin-----17/10/2011-----
    int curState = hw->ECR_GetCheckState();
    switch (curState) {
    case 1: { // открыт чек продажи/возврата
	   hw->ECR_CancelCheck();
	}
    case 2: { // открыт нефискальный (сервисный) документ
	    hw->ECR_CloseNotFiscalDoc();
	}
    case 3: {// ожидание команды продолжения печати
	    hw->ECR_ContinuePrinting();
	}
   }
   //-------------------------------------------------
      
    if(Tr.er_con != 0)  {
	printf("ERROR DATABASE CONNECTION!!!!\n");
	db_connect = false;
    }  else  {
	db_connect = true;
    }
    if(sp.autoblock_flag)   bt->action_time = QTime::currentTime(); 
    
    grabKeyboard();
    //releaseKeyboard();
  
    InitRights();
    initRadioButton();
}

//====================================================================//
void reportForm::InitRights()
{

    if ( !sp.query_for_authorization ) {
	// НЕ запрашивать пароль при недостатке прав	
	if(Authorization::GetInstance()->GetRight(19))    {
	    zlockPixmapLabel->hide();
	    zlockPixmapLabel2->hide();
	}    else    {
	    zreportRadioButton->setEnabled(false);	
	    closesessionRadioButton->setEnabled(false);	
	}	
    }   else {
	// ЗАПРАШИВАТЬ пароль при недостатке прав
	if(Authorization::GetInstance()->GetRight(19))    {
	    zlockPixmapLabel->hide();
	    zlockPixmapLabel2->hide();
	}
	//   zlockPixmapLabel->hide(); 
    }
    
}
//====================================================================//
void reportForm::initRadioButton() {
    
    this->setFocusPolicy(QWidget::NoFocus);
    
    closesessionRadioButton->setFocusPolicy(QWidget::StrongFocus);    
    zreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    xreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    sectionreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    cashierreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    hourreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    countreportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    warereportRadioButton->setFocusPolicy(QWidget::StrongFocus);
    
    setTabOrder(closesessionRadioButton, zreportRadioButton);
    setTabOrder(zreportRadioButton, xreportRadioButton);
    setTabOrder(xreportRadioButton, sectionreportRadioButton);
    setTabOrder(sectionreportRadioButton, cashierreportRadioButton);
    setTabOrder(cashierreportRadioButton, hourreportRadioButton);
    setTabOrder(hourreportRadioButton, countreportRadioButton);
    setTabOrder(countreportRadioButton, warereportRadioButton);
    
    radio_buttons[0] = closesessionRadioButton;
    radio_buttons[1] = zreportRadioButton;
    radio_buttons[2] = xreportRadioButton;
    radio_buttons[3] = sectionreportRadioButton;
    radio_buttons[4] = cashierreportRadioButton;
    radio_buttons[5] = hourreportRadioButton;
    radio_buttons[6] = countreportRadioButton;
    radio_buttons[7] = warereportRadioButton;
	    
    onSelect(2);
}
//====================================================================//
void reportForm::destroy()
{
  releaseKeyboard();
}
//====================================================================//
void reportForm::keyPressEvent(QKeyEvent *k)
{    
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    switch(k->key())  {
    case Qt::Key_Enter :      Report();      break;
    case Qt::Key_Return :     Report();      break;
    case Qt::Key_F12 :	      CloseServiceForm();
    case Qt::Key_Up :         SelectUp();      break;      
    case Qt::Key_Down :       SelectDown();      break;      
    case Qt::Key_F8 :	      ContinuePrinting();      break;      
    }
}

//====================================================================//
void reportForm::SetEnabledDialog(bool f)
{
    zreportRadioButton->setEnabled(f && (sp.query_for_authorization || Authorization::GetInstance()->GetRight(19)));
    closesessionRadioButton->setEnabled(f && (sp.query_for_authorization || Authorization::GetInstance()->GetRight(19)));
    xreportRadioButton->setEnabled(f);
    sectionreportRadioButton->setEnabled(f);
    cashierreportRadioButton->setEnabled(f);
    hourreportRadioButton->setEnabled(f);
    countreportRadioButton->setEnabled(f);  
    warereportRadioButton->setEnabled(f);
}
//====================================================================//
void reportForm::AutoTakeReport()
{
    int  res;
    
    if ( sp.report_x )  {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(X_REPORT_TYPE);  //X_REPORT_TYPE);
    }
    
    if ( sp.report_section )  {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(SEC_REPORT_TYPE);   //SEC_REPORT_TYPE);
    }	
    
    if(sp.report_cashier)  {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(CASH_REPORT_TYPE);  //CASH_REPORT_TYPE);
    }
    
    if(sp.report_by_hour)  {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(TIME_REPORT_TYPE); //TIME_REPORT_TYPE);
    }
    
    if(sp.report_count)  {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(QUANT_REPORT_TYPE); //QUANT_REPORT_TYPE);
    }
    
    if ( sp.report_ware ) {
	res = hw->ECR_SetMode(2);
	if(res != 0) Kkm_Error(res);
	TakeReport(WARE_REPORT_TYPE); 
    }
  
    res = hw->ECR_SetMode(3);
    if(res != 0) Kkm_Error(res);
    
    //-----(+)-----Zuskin-----16/03/2011-----
    system("/usr/bin/dnc_before_z.sh");
    //--------------------------------------------------
    TakeReport(Z_REPORT_TYPE); //Z_REPORT_TYPE);
    //-----(+)-----Zuskin-----16/03/2011-----
    system("/usr/bin/dnc_after_z.sh");
    //--------------------------------------------------
    if (sp.report_unload)  {      CreateUnloadFlag();   }
    if (sp.report_psreconsiliation)  {    
	if (hw->PS_GetType()!=0)	{
	    npasReconciliationForm npasrf;
	    npasrf.exec();
	} 
    }
  
}
//===================================================================//
QString reportForm::GetCountFormat(int count, int len) {
    QString tmp = QString::number(count);
    QString result = "";
    
    for ( int i = 0; i < (len - tmp.length()); i++) {
	result += "0";
    }
    result += tmp;
    return result;
    
}
//===================================================================//
int reportForm::PrintCashierDiscountInfo(KkmRegistrations kkm_reg,  const char * seller_num) {
    
    //-----(+)-----Zuskin-----06/06/2011-----
    // напечатаем информацию о сторно и отменах чеков продаж
    double canceled_count = 0;
    double canceled_sum = 0;
    int st = Tr.GetCanceledCountByCashier(seller_num, canceled_count);
    if ( st != 0 ) { Error(104); return 0; }
    st = Tr.GetCanceledSumByCashier(seller_num, canceled_sum);
    if ( st != 0 ) { Error(104); return 0; }
    if (canceled_count > 0) {
        res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("отмен чеков продаж")), GetCountFormat(canceled_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(canceled_sum, 'f', 2), " ");
    }
    double storno_count = 0;
    double storno_sum = 0;
    st = Tr.GetStornoCountByCashier(seller_num, storno_count);
    if ( st != 0 ) { Error(104); return 0; }
    st = Tr.GetStornoSumByCashier(seller_num, storno_sum);
    if ( st != 0 ) { Error(104); return 0; }
    if (storno_count > 0) {
        res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("сторно продаж")), GetCountFormat(storno_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(storno_sum, 'f', 2), " ");
    }
        
    //---------------------------------------
        
    
    int state = 0;
    
    int pos_discount_count = 0;
    double pos_discount_sum = 0;
    
    int pos_charge_count = 0;
    double pos_charge_sum = 0;
    
    int check_discount_count = 0;
    double check_discount_sum = 0;
    
    int check_charge_count = 0;
    double check_charge_sum = 0;
    
    state = Tr.Get_Cashier_Discount_Info(seller_num, pos_discount_count, pos_discount_sum, pos_charge_count, pos_charge_sum, check_discount_count, check_discount_sum, check_charge_count, check_charge_sum);
    
    if ( state != 0 ) { Error(104); return 0; }
    
    if ( pos_discount_count > 0  ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("скидок на позиции")), GetCountFormat(pos_discount_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(pos_discount_sum, 'f', 2), " ");
    }
    
    if ( pos_charge_count > 0  ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("надбавок на позиции")), GetCountFormat(pos_charge_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(pos_charge_sum, 'f', 2), " ");
    }
    if ( check_discount_count > 0  ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("скидок на чек")), GetCountFormat(check_discount_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(check_discount_sum, 'f', 2), " ");
    }
    if ( check_charge_count > 0  ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("надбавок на чек")), GetCountFormat(check_charge_count, 4), " ");
	if ( res != 0 ) return res;
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(check_charge_sum, 'f', 2), " ");
    }
    
    //-----(+)-----Zuskin-----12/01/2012-----
    if ( pos_discount_sum + check_discount_sum > 0 ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("итог по скидкам")), QString::number(pos_discount_sum + check_discount_sum, 'f', 2), " ");
	if ( res != 0 ) return res;
    }
    if ( pos_charge_sum + check_charge_sum > 0 ) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("итог по надбавкам")), QString::number(pos_charge_sum + check_charge_sum, 'f', 2), " ");
	if ( res != 0 ) return res;
    } 
     //-------------------------------------------------
    
    return res;
}
//===================================================================//
int reportForm::PrintCashierInfo(KkmRegistrations kkm_reg,  const char * seller_num, int check_type)
{
    
    int count_check = 0;
    double sum = 0;
    int state = Tr.Get_Session_Info(QString::number(check_type), seller_num, count_check, sum);
    if ( state != 0 ) { Error(104); return 0; }
    
    //if ( count_check < 1 ) return 0;
    
    QString tmp1;
    QString tmp2 = QString::number(count_check);
    for(int i = 0; i < 4 - tmp2.length(); i++) tmp1 += "0";
    tmp1 += tmp2;
    
    if ( check_type == 0 ) res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("чеков продаж")), tmp1, " ");
    if ( check_type == 1 ) res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("чеков возвратов")), tmp1, " ");
    if ( check_type == 4 ) res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("внесений")), tmp1, " ");
    if ( check_type == 5 ) res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("выплат")), tmp1, " ");    
    
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")),  QString::number(sum, 'f', 2), " ");
    if ( res != 0 ) return res;
    
    if ( check_type != 0 && check_type != 1 ) return 0;
    
    double payment_sum1 = 0;
    double payment_sum2 = 0; 
    double payment_sum3 = 0;
    state = Tr.Get_Session_Payment_Info(QString::number(check_type), seller_num, payment_sum1, payment_sum2, payment_sum3);
    if ( state != 0 ) { Error(104); return 0; }
    
    //printf("payment_sum1 = %f \n payment_sum2 = %f \n payment_sum3 = %f \n", payment_sum1, payment_sum2, payment_sum3);
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("  наличными")), QString::number(payment_sum1, 'f', 2), " ");
    if ( res != 0 ) return res;	
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("  кредитом")), QString::number(payment_sum2, 'f', 2), " ");
    if ( res != 0 ) return res;
    
    if (check_type == 0) {
	res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("  тарой")), QString::number(payment_sum3, 'f', 2), " ");
	if ( res != 0 ) return res;
    }
    
    return 0;
}
//===================================================================//
int reportForm::PrintCashierReport()
{
   // puts("int reportForm::PrintCashierReport()");
    int res = 0;

    KkmRegistrations kkm_reg(hw, &regformlog);
    
    if (( hw->ECR_GetProtocol() != 4 )&&( hw->ECR_GetProtocol() != 5 )) 
		  res = kkm_reg.KkmPrintDocumentInfo();
    if ( res != 0 ) return res;	    
	    
    if (hw->ECR_GetEducationMode() == 1)  {	
	res = kkm_reg.PrintBoldCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("РЕЖИМ ОБУЧЕНИЯ")));
	if ( res != 0)    return res;  
    }
    
    res = kkm_reg.KkmPrintString( HwCodec->fromUnicode(DefCodec->toUnicode("ОТЧЕТ ПО КАССИРАМ")) );
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintSeparatorString( HwCodec->fromUnicode(DefCodec->toUnicode("за смену")),  QString("N") + QString::number(hw->ECR_GetSession()) , " ");
    if ( res != 0 ) return res;
    
    AccessRights acc(LINCASH_CONF_PATH);
    //acc.GetUsers(count_users, users);
    
    int count_users = 0;    
    char ** users = NULL;
    
    Tr.Get_Cashier_By_Session(users, count_users);
        
   // printf("count_users = %d \n", count_users);
    
    for ( int i = 0; i < count_users; i++ ) {	
	
	char * seller_name = NULL;
	
	acc.GetSellerName(users[i], seller_name);
	
	if ( seller_name == NULL ) break;
	
	res = kkm_reg.PrintCenterString("-----------------");
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	res = kkm_reg.KkmPrintSeparatorString( HwCodec->fromUnicode(DefCodec->toUnicode("ПО КАССИРУ")),  HwCodec->fromUnicode(DefCodec->toUnicode(seller_name)), " ");
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}

	res = PrintCashierInfo(kkm_reg, users[i], 0);
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	
	res = PrintCashierInfo(kkm_reg, users[i], 1);
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	
	res = PrintCashierInfo(kkm_reg, users[i], 4);
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	
	res = PrintCashierInfo(kkm_reg, users[i], 5);
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	
	res = PrintCashierDiscountInfo(kkm_reg, users[i]);
	if ( res != 0 ) {
		 if ( seller_name != NULL ) free(seller_name);
		 break;}
	
	if ( seller_name != NULL ) free(seller_name);
    }
    
    for ( int i = count_users - 1; i >= 0; i-- ) free(users[i]);
    free(users);
    
  //  printf("print cashier ----- ok");
    
    if ( res == 0 ) res = kkm_reg.PrintCenterString("-----------------");
    if ( res == 0 ) res = kkm_reg.KkmPrintString( HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ ПО КАССИРАМ :")));
    if ( res == 0 ) res = PrintCashierInfo(kkm_reg, "", 0);
    if ( res == 0 ) res = PrintCashierInfo(kkm_reg, "", 1);
    if ( res == 0 ) res = PrintCashierInfo(kkm_reg, "", 4);
    if ( res == 0 ) res = PrintCashierInfo(kkm_reg, "", 5);   
    if ( res == 0 ) res = PrintCashierDiscountInfo(kkm_reg, "");   
    
    PrintEndReport(kkm_reg);
    
    return res;
}
//===================================================================//
int reportForm::PrintWareReport() {
        
    int res = 0;
    
    int ware_count = 0;
    char** ware_codes = NULL;
    
    if ( Tr.GetSessionWare(ware_codes, ware_count) != 0 ) return 0;	// Выдать ошибку
    
    
    KkmRegistrations kkm_reg(hw, &regformlog);
    
    if (( hw->ECR_GetProtocol() != 4 )&&( hw->ECR_GetProtocol() != 5 )) res = kkm_reg.KkmPrintDocumentInfo();
    if ( res != 0 ) return res;	    
	    
    if (hw->ECR_GetEducationMode() == 1)  {	
	res = kkm_reg.PrintBoldCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("РЕЖИМ ОБУЧЕНИЯ")));
	if ( res != 0)    return res;  
    }
    
    res = kkm_reg.KkmPrintString( HwCodec->fromUnicode(DefCodec->toUnicode("ОТЧЕТ ПО ТОВАРАМ")) );
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintSeparatorString( HwCodec->fromUnicode(DefCodec->toUnicode("за смену")),  QString("N") + QString::number(hw->ECR_GetSession()) , " ");
    if ( res != 0 ) return res;
    
    
    Registration reg(&sp, LINCASH_CONF_PATH);
    
    char * shortcut = NULL;
    double quantity = 0;
    double sum = 0;
    
    //printf("ware_count = %d \n", ware_count);
    for ( int i = 0; i < ware_count; i++ ) {
	//printf("ware_codes[%d] = %s \n", i, ware_codes[i]);
	
	if ( shortcut != NULL ) {
	    free(shortcut);
	    shortcut = NULL;
	}
	
	reg.GetWareShortcut(ware_codes[i], shortcut);
	
	if ( shortcut == NULL) {
	    shortcut = (char*) calloc(strlen("Неизвестный товар") + 1, sizeof(char*));
	    strcpy(shortcut, "Неизвестный товар");
	}
	
	if (Tr.GetWareInfo(ware_codes[i], quantity, sum) != 0) break;	//Выдать ошибку к бд
	
	//printf("ware = %s shortcut = %s quantity = %f sum = %f \n", ware_codes[i], shortcut, quantity, sum);
	
	// печать товара 
	if ( res == 0 && quantity > 0  ) {
	    res = kkm_reg.KkmPrintString(HwCodec->fromUnicode(DbCodec->toUnicode(shortcut)), true, sp.cut_position_name);
	    //res = kkm_reg.KkmPrintPositionInfo(shortcut, quantity, price, sp.cut_position_name);
	    if ( res != 0 ) return res;
	    QString quantity_and_sum = "";
	    quantity_and_sum += "X" + QString::number(quantity, 'f', 3);
	    quantity_and_sum += " =" + QString::number(sum, 'f', 2);
	    res = kkm_reg.KkmPrintSeparatorString("", quantity_and_sum, " ");
	    //res = kkm_reg.KkmPrintPositionInfo(shortcut, quantity, price, sp.cut_position_name);
	    if ( res != 0 ) return res;
	}
	
	res = Tr.GetReturnWareInfo(ware_codes[i], quantity, sum);
	
	//printf(" return ware = %s shortcut = %s quantity = %f sum = %f \n", ware_codes[i], shortcut, quantity, sum);
	
	// печать возврата товара
	if ( res == 0 && quantity > 0  ) {
	    res = kkm_reg.KkmPrintString( HwCodec->fromUnicode(DefCodec->toUnicode("Возврат товара")) );
	    if ( res != 0 ) return res;
	    
	    res = kkm_reg.KkmPrintString(HwCodec->fromUnicode(DbCodec->toUnicode(shortcut)), true, sp.cut_position_name);
	    //res = kkm_reg.KkmPrintPositionInfo(shortcut, quantity, price, sp.cut_position_name);
	    if ( res != 0 ) return res;
	    QString quantity_and_sum = "";
	    quantity_and_sum += "X" + QString::number(quantity, 'f', 3);
	    quantity_and_sum += " =" + QString::number(sum, 'f', 2);
	    res = kkm_reg.KkmPrintSeparatorString("", quantity_and_sum, " ");
	    //res = kkm_reg.KkmPrintPositionInfo(shortcut, quantity, price, sp.cut_position_name);
	    if ( res != 0 ) return res;
	}
	
	res = kkm_reg.KkmPrintString("");
	if ( res != 0 ) return res;
	
    }
    
    for ( int i = ware_count - 1; i >= 0; i-- ) free(ware_codes[i]);
    free(ware_codes);
    
    res = kkm_reg.PrintCenterString( HwCodec->fromUnicode(DefCodec->toUnicode("----------------")) );
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintString( HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ ПО ТОВАРАМ:")) );
    if ( res != 0 ) return res;    
    
    double sale_sum = 0; // сумма продаж с учетом скидок на позиции и чеки
    double return_sum = 0; // сумма возвратов с учетом скидок в чеках возвратов
    int sale_count = 0;
    int return_count = 0;
    
    res = Tr.GetResultSumByWare(sale_count, sale_sum, return_count, return_sum);
    //if (res != 0)  	// выдаьт предупреждение 
    
    printf("[%f][%f][%f][%f]\n", sale_count, sale_sum, return_count, return_sum);
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("продаж")), ConvertToText(sale_count), " ");
    if ( res != 0 ) return res;	
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(sale_sum, 'f', 2), " ");
    if ( res != 0 ) return res;	
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("возвратов")), ConvertToText(return_count), " ");
    if ( res != 0 ) return res;	
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(return_sum, 'f', 2), " ");
    if ( res != 0 ) return res;	 
    
    int pos_discount_count = 0;
    double pos_discount_sum = 0;
    
    int check_discount_count = 0;
    double check_discount_sum = 0;
    
    double total_sale_discount_sum = 0;
    
    
    res = Tr.GetDiscountInfoByWare(pos_discount_count, check_discount_count, pos_discount_sum, check_discount_sum, total_sale_discount_sum);
    
    //if ( res != 0 )  выдать предупреждение
    
    //printf("pos_discount_sum = %f check_discount_sum = %f \n", pos_discount_sum, check_discount_sum);
    
    if ( (pos_discount_count + check_discount_count) > 0 ) {
	res = kkm_reg.KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("В Т.Ч. СКИДОК/НАДБАВОК")));
	if ( res != 0 ) return res;
	if ( pos_discount_count != 0  ) {
	    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на позиции")), ConvertToText(pos_discount_count), " ");
	    if ( res != 0 ) return res;	
	    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(pos_discount_sum, 'f', 2), " ");
	    if ( res != 0 ) return res;	
	}
	
	//printf("check_discount_count = %d check_discount_sum = %f \n", check_discount_count, check_discount_sum);
	if ( check_discount_count > 0  ) {
	    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на чек")), ConvertToText(check_discount_count), " ");
	    if ( res != 0 ) return res;	
	    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("на сумму")), QString::number(check_discount_sum, 'f', 2), " ");
	    if ( res != 0 ) return res;	
	}
    }
    
    res = kkm_reg.PrintCenterString( HwCodec->fromUnicode(DefCodec->toUnicode("----------------")) );
    if ( res != 0 ) return res;
    
    double sale_round_sum = 0;
    double return_round_sum = 0;
    
    res = Tr.GetResultRoundSum(sale_round_sum, return_round_sum);
    //if ( res != 0 )  выдать предупреждение
    
    printf("sale_round_sum = %f , return_round_sum = %f \n", sale_round_sum, return_round_sum);
    
    res = kkm_reg.KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("СУММА ОКРУГЛЕНИЙ")));
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("по продажам")), QString::number(sale_round_sum, 'f', 2), " ");
    if ( res != 0 ) return res;	
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("по возвратам")), QString::number(return_round_sum, 'f', 2), " ");
    if ( res != 0 ) return res;
    
    res = kkm_reg.PrintCenterString( HwCodec->fromUnicode(DefCodec->toUnicode("----------------")) );
    if ( res != 0 ) return res;
     
    double gain_sum = sale_sum - return_sum - sale_round_sum + return_round_sum;
    double subtotal = sale_sum - sale_round_sum;
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ВЫРУЧКА")), QString("=")+QString::number(gain_sum, 'f', 2), " ");
    if ( res != 0 ) return res;
    
    res = kkm_reg.KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СМЕННЫЙ ИТОГ")), QString("=")+QString::number(subtotal, 'f', 2), " ");
    if ( res != 0 ) return res;
    
    PrintEndReport(kkm_reg);
    
    return res;
}
//===================================================================//
QString reportForm::ConvertToText(int count) {
    QString tmp1;
    QString tmp2 = QString::number(count);
    for(int i = 0; i < 4 - tmp2.length(); i++) tmp1 += "0";
    tmp1 += tmp2;
    return tmp1;
}
//===================================================================//

int reportForm::PrintEndReport(KkmRegistrations kkm_reg) {
    int res = 0;
    
    switch(hw->ECR_GetProtocol())	 {
    case 1:{
	    //SHTRIH	
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = hw->ECR_PartialCut(); 
	    if ( res == 0 ) res = hw->ECR_PrintDocHeadLine();				
	    break;
	}
    case 2: {
	    //ATOL	
	    
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = hw->ECR_PrintDocHeadLine();
	    if ( res == 0 ) res = hw->ECR_PartialCut();
	    break;
	}
    case 3: {
	    //INCOTEX
	    if ( res == 0 ) res = kkm_reg.KkmPrintString(" ", false);
	    if ( res == 0 ) res = hw->ECR_CloseNotFiscalDoc();
	    if ( res == 0 ) res = hw->ECR_PartialCut(); 
	    break;
	}
    case 4: {
	    //CASBY
	    if ( res == 0 ) res = hw->ECR_PrintServiceDoc();
	    return res;
	}
    case 5:{
	    //PRIM	
	    if ( res == 0 ) res = hw->ECR_CloseNotFiscalDoc();
	    break;
	}
    //-----(+)-----Zuskin-----17/10/2011-----	
    case 8:{
	    //ORION-FR01K	
	    if ( res == 0 ) res = hw->ECR_CloseNotFiscalDoc();
	    break;
	}
    //--------------------------------------------------	
    }   
    return res;
    
}
//===================================================================//
int reportForm::PrintSectionReport() {
    int kkm_res = 0;
    
    KkmRegistrations kkm_reg(hw, &regformlog);
    
    QString s = "";
    QDate d( hw->ECR_GetYear(), hw->ECR_GetMonth(), hw->ECR_GetDay() );
    s += d.toString("dd.MM.yy") + " ";
    QTime t( hw->ECR_GetHour(), hw->ECR_GetMinute() );
    s += t.toString("hh:mm");
    kkm_res = kkm_reg.KkmPrintString(s.ascii());
    if ( kkm_res != 0 )	  return kkm_res;
    
    for ( int i = 0 ; i < sp.sections.size(); i++ ) {
	kkm_res = kkm_reg.KkmPrintSeveralStrings(sp.sections[i].head_text);
	if ( kkm_res != 0 ) return kkm_res;
	
	s = "";
	s += HwCodec->fromUnicode(DefCodec->toUnicode("СЕКЦИЯ №"));
	s +=  QString::number(sp.sections[i].number) + " ";
	s += HwCodec->fromUnicode(DefCodec->toUnicode(sp.sections[i].name.c_str()));
	
	kkm_res = kkm_reg.KkmPrintString(s);
	if ( kkm_res != 0 ) return kkm_res;
	
	//-----(+)-----Zuskin-----04/05/2011-----
	res = kkm_reg.PrintCenterString("-----------------");
	if ( kkm_res != 0 ) return kkm_res;
	//---------получим суммы внесений/выплат по секции---------
	double income_sum = 0;
        res = Tr.GetIncomeSumBySection((QString::number(sp.sections[i].number)).ascii(), income_sum);
	double outcome_sum = 0;
        res = Tr.GetOutcomeSumBySection((QString::number(sp.sections[i].number)).ascii(), outcome_sum);
	 //---------получим сумму продаж по секции----------
	double sales_sum = 0;
        res = Tr.GetSalesSumBySection((QString::number(sp.sections[i].number)).ascii(), sales_sum);
	//---------получим сумму возвратов---------	
	double return_sum = 0;
        res = Tr.GetReturnsSumBySection((QString::number(sp.sections[i].number)).ascii(), return_sum);
        //посчитаем выручку
	double gain_sum = sales_sum - return_sum;
	//посчитаем сумму наличных в начале смены
	double cash_at_start = sp.sections[i].cash - (sales_sum - return_sum + income_sum - outcome_sum);
	//---------печатаем высчитанную сумму наличных на начало смены-------	
	s = HwCodec->fromUnicode(DefCodec->toUnicode("НАЛ. НА НАЧАЛО"));
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(cash_at_start, 'f', 2));
	if ( kkm_res != 0 ) return kkm_res;
	//----------печатаем сумму внесений по секции---------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ВНЕСЕНИЙ"));	
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(income_sum, 'f', 2));
	 if ( kkm_res != 0 ) return kkm_res;
	//---------печатаем сумму выплат по секции---------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ВЫПЛАТ"));	
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(outcome_sum, 'f', 2));
	if ( kkm_res != 0 ) return kkm_res;
	//---------печатаем сумму выручки по секции---------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ВЫРУЧКА"));
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(gain_sum, 'f', 2));
	if ( kkm_res != 0 ) return kkm_res;		
	//---------печатаем сумму наличных по секции---------
	//--------------------------------------------------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("НАЛ. В КАССЕ"));
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(sp.sections[i].cash, 'f', 2));
	if ( kkm_res != 0 ) return kkm_res;
	//-----(+)-----Zuskin-----04/05/2011-----
	res = kkm_reg.PrintCenterString("-----------------");
	if ( kkm_res != 0 ) return kkm_res;
	//---------печатаем сумму продаж по секции---------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ПРОДАЖ"));	
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(sales_sum, 'f', 2));
	if ( kkm_res != 0 ) return kkm_res;
	//---------печатаем сумму возвратов по секции---------
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТОВ"));	
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(return_sum, 'f', 2));
	 if ( kkm_res != 0 ) return kkm_res;
	//---------печатаем сумму сторно по секции---------
	double storno_sum = 0;
                res = Tr.GetStornoSumBySection((QString::number(sp.sections[i].number)).ascii(), storno_sum);
	s = HwCodec->fromUnicode(DefCodec->toUnicode("СТОРНО"));	
	kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(storno_sum, 'f', 2));
	 if ( kkm_res != 0 ) return kkm_res;
	 //---------печатаем сумму отмен по секции---------
	double cancel_sum = 0;
                res = Tr.GetCanceledSumBySection((QString::number(sp.sections[i].number)).ascii(), cancel_sum);
	s = HwCodec->fromUnicode(DefCodec->toUnicode("ОТМЕН ЧЕКОВ"));		kkm_res = kkm_reg.KkmPrintSeparatorString(s, QString("=") + QString::number(cancel_sum, 'f', 2));
	 if ( kkm_res != 0 ) return kkm_res;
	 //------------------------------------------------------------------------
	  
	if ( i > 0 ) {
	    kkm_res = kkm_reg.KkmPrintSeveralStrings(sp.sections[i].bottom_text);
	    if ( kkm_res != 0 ) return kkm_res;
	}
	
	kkm_res = kkm_reg.KkmPrintString("");
	if ( kkm_res != 0 ) return kkm_res;
    }	 
    
    kkm_res = kkm_reg.KkmPartialCut();
    if (kkm_res != 0) return kkm_res;
    
    return kkm_res;    
}
//===================================================================//
void reportForm::TakeReport(int rep_type)
{
    //puts("reportForm::TakeReport(int rep_type)");
    printf("TAKEREPORT(%d)\n", rep_type);
    // Запоминаем три суммы(наличность , сменный итог, выручка) и номер текущей смены
    double cash_sum = 0;
    double gain_sum = 0;
    double total_sum = 0;
    int n_session = 0;
    
    int fr_state = hw->ECR_ReadStatus();	
  //  printf("read status = %d \n", fr_state);
    if ( fr_state != 0 ) return;
  //  printf("mode = %d, adv_mode = %d\n", hw->ECR_GetMode(), hw->ECR_GetAdvancedMode());
    
    n_session = hw->ECR_GetSession();
    
    hw->ECR_ReadCashInDrawer();			// наличность в кассе
    
    cash_sum = hw->ECR_GetSumm();
    
    hw->ECR_ReadSessionSumm(); 			// сменный итог
    total_sum = hw->ECR_GetSumm();
    
    hw->ECR_ReadGain(); 			//выручка
    gain_sum = hw->ECR_GetSumm();    
    //printf("hw->ECR_GetEducationMode() = %d\n", hw->ECR_GetEducationMode());
    if ( rep_type == CASH_REPORT_TYPE ) {	
	// отчет по кассирам
	if ( !((hw->ECR_GetProtocol() == 4 || hw->ECR_GetProtocol() == 5) && hw->ECR_GetEducationMode()) ) {
	   // puts("printf cashier report");
	    res = PrintCashierReport();
	//    printf("res = %d \n", res);	    
	}
    } else if ( rep_type == WARE_REPORT_TYPE ) {
	// отчет по товарам 
	if ( !((hw->ECR_GetProtocol() == 4 || hw->ECR_GetProtocol() == 5) && hw->ECR_GetEducationMode()) ) {
	    puts("Отчет по товарам");
	    res = PrintWareReport();
	}
    } else if ( rep_type == SEC_REPORT_TYPE && sp.sections.size() > 0 ) {
	// отчет по секциям
	if ( !((hw->ECR_GetProtocol() == 4 || hw->ECR_GetProtocol() == 5) && hw->ECR_GetEducationMode()) ) {
	    res = PrintSectionReport();
	}
    } else  {
	
	if ( rep_type == Z_REPORT_TYPE ) {
	    if ( sp.sections.size() > 0 ) {
		KkmRegistrations kkm_reg(hw, &regformlog);
		kkm_reg.KkmPrintSeveralStrings(sp.sections[0].head_text);
		
	    }
	}
	
	res = hw->ECR_Report(rep_type);
	//-----(+)-----Zuskin-----21/03/2011-----
	if ((res == 103) && (hw->ECR_GetProtocol() == 3)) { 
	    //для Меркурий-MSK в случае обрыва ленты надо (после вставки ленты) послать команду "печать копии документа"
	    QTextCodec* DefCodec = QTextCodec::codecForName(SYSCODEC);	
	    QMessageBox mb( DefCodec->toUnicode("ВНИМАНИЕ!"),
			    DefCodec->toUnicode("Обрыв чековой ленты! Вставьте чековую ленту\nи нажмите [OK] для повторной печати отчета"),
			    QMessageBox::Warning,
			    QMessageBox::Ok | QMessageBox::Default,
			    QMessageBox::NoButton,
			    QMessageBox::NoButton);
	    releaseKeyboard(); 
	    mb.exec();
	    grabKeyboard();
	    res = hw->ECR_ContinuePrinting();
	}
	//---------------------------------------
    }
 //  printf("res of printing = %d \n", res );

    if ( res != 0 )    Kkm_Error(res);
    else  {
	if ( rep_type == Z_REPORT_TYPE ) {
	    sp.zeroCashSumInSections();
	}    
	 // Для отчета с гашением открываем ящик
	if ( sp.open_box_by_clear_report && (rep_type == Z_REPORT_TYPE) )    {
	    res = hw->ECR_OpenDrawer();
	    if ( res != 0 )        Kkm_Error(res);
	}
	ins_tr->CreateDateTimeString();
	switch(rep_type)    {
	case Z_REPORT_TYPE : // z-отчет
	    ins_tr->InsertReportTransactions(rep_type, n_session, cash_sum, gain_sum, total_sum);
	    emit ZReport();
	    break;
	default : // x-отчет
		ins_tr->InsertReportTransactions(rep_type, n_session, cash_sum, gain_sum, total_sum);    }
	
	res = hw->ECR_SetMode(0);
	if ( res ) Kkm_Error(res);
    }
    
    
    // сделать цикл чтения статуса ФР. Варианты для ШТРИХ: идёт печать , обрыв ленты, 
    bool cont_loop = true; 
    int AMode = 0;
    
    if (hw->ECR_GetProtocol() == 1)  {
	do    {
	    res = hw->ECR_ReadStatus();
	    AMode = hw->ECR_GetAdvancedMode();
	    if (AMode!=5)  cont_loop = false;
	    pause(9);
	}
	while (cont_loop);
	
	// выдать сообщение "Обрыв ленты! Вставьте чековую ленту и нажмите клавишу [Продолжить печать] 
	if (AMode == 2) 	{
	    QTextCodec* DefCodec = QTextCodec::codecForName(SYSCODEC);	
	    QMessageBox mb( DefCodec->toUnicode("ВНИМАНИЕ!"),
			    DefCodec->toUnicode("Обрыв чековой ленты! Вставьте чековую ленту\nи нажмите клавишу [Продолжить печать] (F8)"),
			    QMessageBox::Warning,
			    QMessageBox::Ok | QMessageBox::Default,
			    QMessageBox::NoButton,
			    QMessageBox::NoButton);
	    releaseKeyboard(); 
	    mb.exec();
	    grabKeyboard();
	}
    }
    //-----(+)-----Zuskin-----06/06/2011-----        
    if (( rep_type == Z_REPORT_TYPE ) && ( sp.sections.size() > 0 )) {
	    hw->ECR_ReadCashInDrawer();			// наличность в кассе
            cash_sum = hw->ECR_GetSumm();
	    sp.deductCashSumInSection(0, sp.getCashSumInSection(0));
	    sp.addCashSumInSection(0, cash_sum);
    }    
    //--------------------------------------	
    
}
//====================================================================//
void reportForm::pause(int Nsec)
{
    struct timespec R1, R2;
    if (Nsec < 1 || Nsec > 99)           return;
    R1.tv_sec = 0;
    R1.tv_nsec = Nsec * 100000000;
    if (nanosleep(&R1,&R2)!=0)
	nanosleep(&R2,&R1);
    return;
}
//====================================================================//
void reportForm::Report()
{
    // puts("<<<<<<<<<<<reportForm::Report()");
    SetEnabledDialog(false);
    
    int rep_type = WhichIsChecked();
    
    printf("SELECT report = %d \n", rep_type);
    
    // запрос авторизации при недостатке прав
    bool right = true;
    if ( (rep_type == Z_REPORT_TYPE) || (rep_type == CLOSE_SESSION_TYPE) )  {
	right = Authorization::GetInstance()->GetRight(19);
	if ( !right )    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(19);
    }
    
    //Есть право или авторизация поддтверждена
    if ( right )   {
	if (sp.auto_take_report && (rep_type == CLOSE_SESSION_TYPE))      { AutoTakeReport(); }
	else    {	
	    if (rep_type == CLOSE_SESSION_TYPE) {
		rep_type = Z_REPORT_TYPE;
	    } 
	    
	    switch(rep_type)    {
	    case Z_REPORT_TYPE : // z-отчет
		res = hw->ECR_SetMode(3); 
		if(res != 0) Kkm_Error(res);
		break;
	    default :// не фискальные отчет
                                res = hw->ECR_SetMode(2);
	                if(res != 0) Kkm_Error(res);
		    }
	    TakeReport(rep_type);
	}
    }
    else Error(72);
    
    SetEnabledDialog(true);
}

//====================================================================//
void reportForm::Kkm_Error(int er)
{
    //puts("reportForm::Kkm_Error(int er)");
    //releaseKeyboard();
    //kkm_Error(er, f);  
  
    QString mes_id, mes_str;
    hw->ECR_ReadError(er);
    mes_id = hw->ECR_GetErrNum();
    mes_str = hw->ECR_GetErrDescription();
    kkm_Error(mes_id, mes_str);
    if ((er == -1) || (er == -2) || (er == 2))
		  f = false;
    hw->ECR_SetMode(0); 
    grabKeyboard(); 
}

//======================================================================//
void reportForm::Error(int er)
{
  //puts("void serviceForm::Error(int er)");
  QString er_id, er_str;
  RegistrationErrorText(er, er_str, er_id);
  
  releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  grabKeyboard();
}


//====================================================================//
void reportForm::CloseServiceForm()
{
//puts("reportForm::CloseServiceForm()");
  releaseKeyboard();
  close();
}
//====================================================================//

int reportForm::WhichIsChecked()
{
    if ( warereportRadioButton->isChecked() ) puts("TRUE");
    else puts("FALSE");
    
    if ( closesessionRadioButton->isChecked() ) puts(" close session TRUE");
    else puts("closesessionRadioButton FALSE");
    
    if ( closesessionRadioButton->isChecked() ) return CLOSE_SESSION_TYPE;  // закрытие смены
    if ( zreportRadioButton->isChecked() ) return Z_REPORT_TYPE;            // z -отчет
    if ( xreportRadioButton->isChecked() ) return X_REPORT_TYPE;            // x-отчет
    if ( sectionreportRadioButton->isChecked() ) return SEC_REPORT_TYPE;        // по секциям
    if ( cashierreportRadioButton->isChecked() ) return CASH_REPORT_TYPE;     // по кассирам
    if ( hourreportRadioButton->isChecked() ) return TIME_REPORT_TYPE;      // почасовой
    if ( countreportRadioButton->isChecked() ) return QUANT_REPORT_TYPE;  // количеств
    if ( warereportRadioButton->isChecked() ) return WARE_REPORT_TYPE;     // по товарам
    return 0;
}
//====================================================================//
QColor reportForm::GetDifColor()
{
 QColor origColor = this->paletteBackgroundColor();
 
  int Red = origColor.red();
  int Blue = origColor.blue();
  int Green = origColor.green();
     
  if ( Red < 40) Red = Red + 255;
  if ( Blue < 40) Blue = Blue + 255;
  if ( Green < 40) Green = Green + 255;
  
  return QColor(Red-40,Green-40,Blue-40);   
}

//====================================================================//
void reportForm::CreateUnloadFlag()
{
    int k = TestPath(fileflag_path);
    
    if ( k != 0 && k != 1 ) return;
    
    FILE * fp = fopen(fileflag_path,"w+");
    if (fp) fclose(fp);
    
}
//====================================================================//
void reportForm::ContinuePrinting()
{
    hw->ECR_ContinuePrinting();
}
//====================================================================//
void reportForm::SelectUp()
{
    if ( current_type > 0 ) 
	radio_buttons[current_type - 1]->setChecked(true);
    else 
	radio_buttons[7]->setChecked(true);
}
//====================================================================//
void reportForm::SelectDown()
{
    if ( current_type < 7 )
	radio_buttons[current_type + 1]->setChecked(true);
    else 
	radio_buttons[0]->setChecked(true);
}
//====================================================================//
void reportForm::offSelect(int r_num) {
    radio_buttons[r_num]->setPaletteBackgroundColor(this->paletteBackgroundColor());
}
//====================================================================//
void reportForm::onSelect(int r_num) {
    offSelect(current_type);
    current_type = r_num;
    radio_buttons[r_num]->setPaletteBackgroundColor(GetDifColor());
    radio_buttons[r_num]->setFocus();
}
//====================================================================//
void reportForm::closesessionRadioButton_toggled( bool )
{
    onSelect(0);
}
//====================================================================//
void reportForm::zreportRadioButton_toggled( bool )
{
    onSelect(1);
}
//====================================================================//
void reportForm::xreportRadioButton_toggled( bool )
{
    onSelect(2);
}
//====================================================================//
void reportForm::sectionreportRadioButton_toggled( bool )
{
    onSelect(3);
}
//====================================================================//
void reportForm::cashierreportRadioButton_toggled( bool )
{
    onSelect(4);
}
//====================================================================//
void reportForm::hourreportRadioButton_toggled( bool )
{
    onSelect(5);
}
//====================================================================//
void reportForm::countreportRadioButton_toggled( bool )
{
    onSelect(6);
}
//====================================================================//
void reportForm::warereportRadioButton_toggled( bool )
{
    onSelect(7);
}
//====================================================================//
