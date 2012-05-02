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
#include "insert_transaction.h"
#include "const.h"
#include "addon_func.h"
//#include "authorization.h"
#include "reader_config_load_unload.h"
#include "const_conf.h"

#include <qstring.h>

#include <malloc.h>
#include <string.h>
#include <math.h>

int redtype[4] = {2, 1, 0, 3};

char * pc_nmb = NULL;

#define DOC_LEN 8
#define REC_LEN 4
#define PC_LEN 5
#define KKM_LEN 20
#define SELL_LEN 10
#define SESS_LEN 4
#define DATE_LEN 10
#define TIME_LEN 8
#define DT_LEN DATE_LEN + TIME_LEN + 1

//======================================================================//
void Read_N_PC()
{
    ReaderConf * rc;
    rc = new ReaderConf(RC_LOADUNLOAD_PATH);
    int len = rc->getLen("[N_PC]");
    //printf("len = %d \n", len);
    if (len == 0)    {
	pc_nmb = (char *) calloc (2,sizeof(char));
	strcpy(pc_nmb,"1");
    }	    else    {
	pc_nmb = (char*) calloc(len+1, sizeof(char));
	rc->GetNPC(pc_nmb);
	strrs(pc_nmb);
    }
    if (atoi(pc_nmb) <= 0) strcpy(pc_nmb, "1");
    delete rc;
}

//======================================================================//
InsertTransactions::InsertTransactions()
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "InsertTransactions::InsertTransactions()");
  Read_N_PC();
  hw = NULL;
  receipt = NULL;
  Tr = NULL;
  log = NULL;

  cur_receipt_datetime = NULL;

  init_flag = false;
  
  user_id = 0;
}


//======================================================================//
InsertTransactions::InsertTransactions(HWSRV * hw_prt, Receipt * rec_ptr, Trans * tr_ptr, DebugLevelLog * log_ptr)
{
  log = log_ptr;
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "InsertTransactions::InsertTransactions(HWSRV * hw_prt, Receipt * rec_ptr, Trans * tr_ptr, DebugLevelLog * log_ptr)");
  
  Read_N_PC();
  hw = hw_prt;
  receipt = rec_ptr;
  Tr = tr_ptr;
  
  cur_receipt_datetime = (char *) calloc(DT_LEN + 1, sizeof(char));

  init_flag = true;
  user_id = 0;
}

//======================================================================//
InsertTransactions::~InsertTransactions()
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "InsertTransactions::~InsertTransactions()");
  if (pc_nmb!=NULL) free (pc_nmb);
}
//======================================================================//
void InsertTransactions::SetUserId(int auser_id)
{
    user_id = auser_id;
}
//======================================================================//
void InsertTransactions::SetSP(SP * asp) 
{
    sp = asp;
}

//======================================================================//
void InsertTransactions::CreateDateTimeString()
{
    if ( sp->learn_mode && ! sp->write_transaction ) return;
    
    char * cur_receipt_date = (char *) calloc(DATE_LEN + 1, sizeof(char));
    char * cur_receipt_time = (char *) calloc(TIME_LEN + 1, sizeof(char));
    
    hw->ECR_ReadStatus();
    CreateDateString(cur_receipt_date);
    CreateTimeString(cur_receipt_time);
    
    strcpy(cur_receipt_datetime, cur_receipt_date);
    strcat(cur_receipt_datetime, cur_receipt_time);
    
    free(cur_receipt_date);
    free(cur_receipt_time);
}

//======================================================================//
int InsertTransactions::InsertCheckRegistrationTransactions(const char * check_type, const char * card, double nal, double cred, double cont, bool closed_session_flag, bool close_check, string manager_id)
{
    //printf("InsertCheckRegistrationTransactions(check_type = %s, card = %s, nal = %f, cred = %f , cont = %f) \n", check_type, card, nal, cred, cont);
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    
    int er = 1;
    if ( init_flag )  {
	
	//printf("user_id = %d \n", user_id);
	char * seller = itoa(user_id);
	
	 //-----(+)-----Zuskin-----13/01/2012-----
	 int CheckNumber = receipt->GetCheckNumber();
	 //------------------------------------------------- 
	//-----(*)-----Zuskin-----11/10/2011-----	
	 char * chtype = NULL;
	 int ct = atoi(check_type);
	 if (ct > 97) {
	     chtype = itoa(99-ct);
	     //-----(+)-----Zuskin-----13/01/2012-----
	     CheckNumber = Tr->GetPutOutCheckNumber();
	     //--------------------------------------------------
	 }
	 //-------------------------------------------------
	 
	Tr->BeginTransaction();
	
	er = InsertOpenChangeTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), closed_session_flag);

	if ( er != 1 ) {
	    Tr->RollbackTransaction();
	    if ( seller ) free(seller);
	    //-----(*)-----Zuskin-----11/10/2011-----	 
	    if (chtype) free(chtype);
	    //-------------------------------------------------
	    return er;
	}
	
	if (ct > 97) {
     	    //-----(*)-----Zuskin-----13/01/2012-----
	    //er = InsertRegistrationsTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), chtype);
	    er = InsertRegistrationsTransactions(itoa(CheckNumber), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), chtype);
	    //---------------------------------------------------
	}
	else
	//-----(*)-----Zuskin-----13/01/2012-----
	    //er = InsertRegistrationsTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type);
	    er = InsertRegistrationsTransactions(itoa(CheckNumber), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type);
	//--------------------------------------------------
	//printf("InsertRegistrationsTransactions return - %d \n", er);
	if ( er != 1 ) {
	    Tr->RollbackTransaction();
	    if ( seller ) free(seller);
	    //-----(*)-----Zuskin-----11/10/2011-----	
	    if (chtype) free(chtype);
	    //-------------------------------------------------
	    return er;
	}
	//-----(*)-----Zuskin-----11/10/2011-----	
	if (chtype) free(chtype);
	//-------------------------------------------------
	
	//-----(*)-----Zuskin-----13/01/2012-----
	//er = InsertPaymentRegistrationTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type, nal, cred, cont);
	er = InsertPaymentRegistrationTransactions(itoa(CheckNumber), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type, nal, cred, cont);
	//-------------------------------------------------
	//printf("InsertPaymentRegistrationTransactions return - %d \n", er);
	if ( er != 1 ) {
	    Tr->RollbackTransaction();
	    if ( seller ) free(seller);
	    return er;
	}
	//-----(*)-----Zuskin-----18/11/2011-----
	//er = InsertCloseCheckTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type, card, nal, cred, cont, close_check);
	//-----(*)-----Zuskin-----13/01/2012-----
	//er = InsertCloseCheckTransactions(itoa(receipt->GetCheckNumber()), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type, card, nal, cred, cont, close_check, manager_id);
	er = InsertCloseCheckTransactions(itoa(CheckNumber), itoa(receipt->GetDocumentNumber()), pc_nmb, receipt->GetKkmSerial(), seller, itoa(receipt->GetSession()), check_type, card, nal, cred, cont, close_check, manager_id);
	//---------------------------------------
	//---------------------------------------
	//printf("InsertCloseCheckTransactions return - %d \n", er);
	if ( er != 1 ) {
	    Tr->RollbackTransaction();
	    if ( seller ) free(seller);
	    return er;
	}
	
	Tr->CommitTransaction();
	
	if ( seller ) free(seller);
    }
    strcpy(cur_receipt_datetime, "");
    return er;
}

//======================================================================//
// oper_type = 1 - внесение, oper_type = 2 - выплата
int InsertTransactions::InsertEnteringGettingTransaction(int oper_type, const char * sum_str, bool closed_session_flag, int section_num)
{
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    
    int er = 1;
    if ( init_flag )  {
	//char * n_pc = (char *) calloc(PC_LEN + 1, sizeof(char));
	char * n_kkm = (char *) calloc(KKM_LEN + 1, sizeof(char));
	char * n_session = NULL;
	char * seller = NULL;
	char * n_rec = NULL;
	char * n_doc = NULL;
	
	hw->ECR_ReadStatus();
	int r = hw->ECR_ReadDocumentNumber();
	if ( r  != 0 ) return r;
	
	if (hw->ECR_GetProtocol() == 5){
		 n_rec = itoa(hw->ECR_GetCheckNumber());
		 n_doc = itoa(hw->ECR_GetDocumentNumber());
	}
	else{
		 n_rec = itoa(hw->ECR_GetCheckNumber() - 1);
		 n_doc = itoa(hw->ECR_GetDocumentNumber() - 1);
	}
	n_session = itoa(hw->ECR_GetSession());
	seller = itoa(user_id);
	//strcpy(n_pc, "1");
	strcpy(n_kkm, hw->ECR_GetSerialNumber());
	er = InsertOpenChangeTransactions(n_rec, n_doc, pc_nmb, n_kkm, seller, n_session, closed_session_flag);
	if ( er == 1 )    {
	    //-----(*)-----Zuskin-----17/05/2011-----
	    //QString section_number = QString::number(sp->sections[section_num].number);
	    QString section_number = QString("0");
	    if (sp->sections.size() > 0) {
		section_number = QString::number(sp->sections[section_num].number);
	    }	
	    //---------------------------------------
	    if ( oper_type == 1 )      er = Tr->Insert_InOutcome("50", cur_receipt_datetime, pc_nmb, n_kkm, seller, n_session, n_rec, n_doc, "4", sum_str, section_number.ascii());
	    else      er = Tr->Insert_InOutcome("51", cur_receipt_datetime, pc_nmb, n_kkm, seller, n_session, n_rec, n_doc, "5", sum_str, section_number.ascii());
	}
	//if ( n_pc ) free(n_pc);
	if ( n_kkm ) free(n_kkm);
	if ( n_session ) free(n_session);
	if ( seller ) free(seller);
	if ( n_rec ) free(n_rec);
	if ( n_doc ) free(n_doc);
    }
    strcpy(cur_receipt_datetime, "");
    return er;
}
//======================================================================//
int InsertTransactions::InsertReportTransactions(int rep_type, int session, double cash_sum, double gain_sum, double total_sum)
{
//  if(init_flag) //log->PrintString(DEBLEVEL_REGFORM, INFO, "int InsertTransactions::InsertReportTransactions(int rep_type, int session, double cash_sum, double gain_sum, double total_sum)\n");
    
     if ( sp->learn_mode && ! sp->write_transaction ) return 1;
     
    int er = 1;
  if(init_flag)
  {
    char * tr_type = (char *) calloc(3, sizeof(char));
    //char * n_pc = (char *) calloc(PC_LEN + 1, sizeof(char));
    char * n_kkm = (char *) calloc(KKM_LEN + 1, sizeof(char));
    char * n_session = NULL;
    char * seller = NULL;
    char * n_doc = NULL;
    char * gain_sum_str = NULL;
    char * cash_sum_str = NULL;
    char * total_sum_str = NULL;

    int r = hw->ECR_ReadDocumentNumber();
    if ( r != 0 ) return r;
    if (hw->ECR_GetProtocol() == 5)
	 n_doc = itoa(hw->ECR_GetDocumentNumber());
    else
      n_doc = itoa(hw->ECR_GetDocumentNumber() - 1);
    n_session = itoa(session);
    seller = itoa(user_id);

/*    gain_sum_str = ftoa(gain_sum, 2);
    cash_sum_str = ftoa(cash_sum, 2);
    total_sum_str = ftoa(total_sum, 2);*/
   gain_sum_str = NumberToStr(gain_sum,2);
    cash_sum_str = NumberToStr(cash_sum,2);
    total_sum_str = NumberToStr(total_sum,2);
    
    //strcpy(n_pc, "1");
    strcpy(n_kkm, hw->ECR_GetSerialNumber());
	
    if(rep_type == 1)
      strcpy(tr_type, "63");
    else
      strcpy(tr_type, "60");

    er = Tr->Insert_Change_Data(tr_type, cur_receipt_datetime, pc_nmb, n_kkm, seller, n_session, n_doc, gain_sum_str, cash_sum_str, total_sum_str);
    if((er == 1) && (rep_type == 1))
    {
      strcpy(tr_type, "61");
      er = Tr->Insert_Change_Data(tr_type, cur_receipt_datetime, pc_nmb, n_kkm, seller, n_session, n_doc, gain_sum_str, cash_sum_str, total_sum_str);
    }
		
    //if(n_pc) free(n_pc);
    if(n_kkm) free(n_kkm);
    if(n_session) free(n_session);
    if(seller) free(seller);
    if(n_doc) free(n_doc);
    if(tr_type) free(tr_type);
    if(gain_sum_str) free(gain_sum_str);
    if(total_sum_str) free(total_sum_str);
    if(cash_sum_str) free(cash_sum_str);
  }
  strcpy(cur_receipt_datetime, "");
  return er;
}

//#########################################################################################################//

//======================================================================//
int InsertTransactions::InsertOpenChangeTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, bool closed_session_flag)
{	
    //printf("InsertOpenChangeTransactions(n_rec = %s, n_doc = %s, n_pc = %s, n_kkm = %s, seller = %s, n_session = %s) \n", n_rec, n_doc, n_pc, n_kkm, seller, n_session);
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    int er = 1;
    // Если смена не открыта необходимо вставить транзакцию открытия смены
    if ( closed_session_flag )   {
	char * cash_sum = NULL;
	hw->ECR_ReadCashInDrawer();
	cash_sum = NumberToStr(hw->ECR_GetSumm(),2);
	er = Tr->Insert_Change_Data("62", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_doc, cash_sum, "0", "0");
	if ( cash_sum ) free(cash_sum);
    }
    return er;
}

//======================================================================//
int InsertTransactions::InsertRegistrationsTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type)
{
    //printf("InsertRegistrationsTransactions(n_rec = %s, n_doc = %s, n_pc = %s, n_kkm = %s, seller = %s, n_session = %s, check_type = %s)\n", n_rec, n_doc, n_pc, n_kkm, seller, n_session, check_type);
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    int er = 1;
    QString q_str;
    QString price;
    
    //puts("=====0======");
    //printf("");
    for ( int i = 0; i < receipt->record_num; i++ )   {
	//printf("InsertRegistrationsTransactions() i - %d receipt->GetPosElemRef(i)->GetCode() - %s \n", i, receipt->GetPosElemRef(i)->GetCode());
	//Регистрация позиций чека	
	//if ( receipt->GetPosElemRef(i)->enable_flag )      {
	    
	    //printf("%d price = %f quantity = %f \n", i, receipt->GetPosElemRef(i)->price, receipt->GetPosElemRef(i)->quantity);
	    
	    q_str = QString::number(receipt->GetPosElemRef(i)->quantity, 'f', 3); //NumberToStr(receipt->GetPosElemRef(i)->quantity,3);
	    price = QString::number(receipt->GetPosElemRef(i)->price, 'f', 2);	//NumberToStr(receipt->GetPosElemRef(i)->price,2);
	    
	    
	    //printf("cur_receipt_datetime = %s, GetCode() = %s, GetBarcode() = %s, price.ascii() = %s, q_str.ascii() = %s\n", cur_receipt_datetime, receipt->GetPosElemRef(i)->GetCode(), receipt->GetPosElemRef(i)->GetBarcode(), price.ascii(), q_str.ascii());
	    char add_id[10] = "null";
	    //printf("receipt->GetPosElemRef(i)->GetAdditionId() = %s \n", receipt->GetPosElemRef(i)->GetAdditionId());
	    if ( atoi(receipt->GetPosElemRef(i)->GetAdditionId()) > 0 ) 
		strcpy(add_id,receipt->GetPosElemRef(i)->GetAdditionId());
	    //printf("in result add_id = %s \n", add_id);
	    QString result_position_discount = QString::number(receipt->GetPosElemRef(i)->GetPositionResultSum(), 'f', 2);
	    QString section = QString::number(receipt->GetPosElemRef(i)->section_number);
	    
	    QString tr_type = "11";
	    
	    if ( receipt->GetPosElemRef(i)->enable_flag )  {
		tr_type = "11";
		er = Tr->Insert_Registration(tr_type.ascii(), cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type,  receipt->GetPosElemRef(i)->GetCode(), receipt->GetPosElemRef(i)->GetBarcode(), price.ascii(), q_str.ascii(), add_id, result_position_discount.ascii(), section.ascii());
		//printf("Tr->Insert_Registration return - %d \n", er);
		if ( er != 1 )      break;
		//-----(+)-----Zuskin-----29/08/2011-----
		for ( int j = 0; j < 4; j++ )         {
		    // Регистрация скидок на позиции чека
		    er = InsertReductionTransactions(n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, redtype[j], i, j);
		}
		if ( er != 1 ) break;
	    
		//printf("n_pc = %s , n_kkm = %s , seller = %s , n_session = %s, n_rec = %s, n_doc = %s, check_type %s \n", n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type);
		er = InsertTaxTransactions(n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, i);
	    
		if ( er != 1 ) break;
		//---------------------------------------
		
	    } else {
		tr_type = "12";
		er = Tr->Insert_Registration(tr_type.ascii(), cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type,  receipt->GetPosElemRef(i)->GetCode(), receipt->GetPosElemRef(i)->GetBarcode(), price.ascii(), q_str.ascii(), add_id, result_position_discount.ascii(), section.ascii());
		if ( er != 1 )      break;
	    }
	    //-----(-)-----Zuskin-----29/08/2011-----
	    /*for ( int j = 0; j < 4; j++ )         {
		    // Регистрация скидок на позиции чека
		    er = InsertReductionTransactions(n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, redtype[j], i, j);
		}
		if ( er != 1 ) break;
	    
		//printf("n_pc = %s , n_kkm = %s , seller = %s , n_session = %s, n_rec = %s, n_doc = %s, check_type %s \n", n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type);
		er = InsertTaxTransactions(n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, i);
	    
		if ( er != 1 ) break;*/
	    //---------------------------------------
	//}
    }
    
    for ( int j = 0; j < 4; j++)   {
	// Регистрация скидок на чек
	er = InsertReductionTransactions(n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, redtype[j], -1, j);
	if ( er != 1 )      break;
    }
    return er;
}

//======================================================================//
int InsertTransactions::InsertReductionTransactions(const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * n_rec, const char * n_doc, const char * check_type, int reduction_type, int i, int j)
{	
    
    //printf("InsertReductionTransactions(n_pc = %s, n_kkm = %s, seller = %s, n_session = %s, n_rec = %s, n_doc = %s, check_type = %s, int reduction_type = %d, int i = %d, int j = %d)\n", n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, reduction_type, i, j);
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;    
    
    int er = 1;
    //char * red = NULL; //(char *) calloc(12, sizeof(char));
    QString discount_sum = "";
    QString discount_percent = "";
    QString recount_dis_sum = "";
    QString recount_dis_percent = "";
    
    //char * rate = NULL; //(char *) calloc(4, sizeof(char));
    char * type = (char *) calloc(3, sizeof(char));
    char * card = (char *) calloc(500 + 1, sizeof(char));
    char * rtype = NULL; //(char *) calloc(2, sizeof(char));
    char * asch = NULL; //(char *) calloc(11, sizeof(char));
    char * wcode = (char *) calloc(200, sizeof(char));
    
    strcpy(wcode, "0");
    
    //  i != -1 - скидки на позицию
    // Скидки на позицию
    if ( (i != -1) && receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->df )   {
	//strcpy(wcode, receipt->GetPosElemRef(i)->GetCode());
	//printf("dicsount code in insert transacrtion must be 41 = %s \n" , receipt->GetPosElemRef(0)->GetDiscountElemRef(1)->GetCode());
	if ( receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent != 0 )  {
	    if ( receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent < 0 ) 
		//rate = NumberToStr(-receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent,2);  	
		discount_percent = QString::number(-receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent, 'f', 2);
	    else 	
		//rate = NumberToStr(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent,2);  	
		discount_percent = QString::number(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent, 'f', 2); 
	}	
	// rate = ftoa(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->percent, 2);
	else  discount_percent = "0";   //rate = itoa(0);
	
	if ( receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum < 0 )     {
	    //Надбавки
	    if ( discount_percent == "0" )      strcpy(type, "16");             //Суммой
	    else        strcpy(type, "18");             //Процентная
	    discount_sum = QString::number(- receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum, 'f', 2);
	    //      red = ftoa( - receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum, 2);
	    //red = NumberToStr( - receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum,2);
	    
	}    else      {
	    // Скидка
	    if ( discount_percent == "0" )  {   strcpy(type, "15");   }          //Суммой
	    else  {  strcpy(type, "17");     }        //Процентная
	    //  red = ftoa(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum, 2);
	    //red = NumberToStr(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum,2);
	    discount_sum = QString::number(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->dsum, 'f', 2);
	}
	
	rtype = itoa(reduction_type);
	
	// для фиксированной скидки по дисконтной карте
	if ( j == 1 )  {
	    //printf("receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->Get_Internal_Code()) = %s \n");
	    if ( strlen(receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->Get_Internal_Code()) > 0 )
		strcpy(wcode, receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->Get_Internal_Code());
	    strcpy(card, receipt->GetPosElemRef(i)->GetDiscountElemRef(j)->Get_Card_Code());
	}
	
	// для автоматической скидки
	if ( j == 0 )       asch = itoa(receipt->GetPosElemRef(i)->auto_scheme);
	else      asch = itoa(0);
	
	//printf("fix_red code = %s, cardcode = %s \n", wcode, card);
	
	er = Tr->Insert_Reduction(type, cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, wcode, rtype, card, asch, discount_sum.ascii(), discount_percent.ascii(), discount_sum.ascii(), discount_percent.ascii());
	
    }  else if ( (i == -1) && receipt->GetDiscountElemRef(j)->df )  {
	
	//  i == -1 - скидки на чек
	strcpy(wcode, "0");
	
	if ( receipt->GetDiscountElemRef(j)->percent != 0 ) {
	    if ( receipt->GetDiscountElemRef(j)->percent < 0 ) {
		//rate = NumberToStr(-receipt->GetTransactionDiscountPercent(), 2);
		discount_percent = QString::number(-receipt->GetTransactionDiscountPercent(j), 'f', 2);
		recount_dis_percent = QString::number(-receipt->GetRecountTransactionDiscountPercent(j), 'f', 2);
	    }  else  {
		//rate = NumberToStr(receipt->GetTransactionDiscountPercent(), 2);
		discount_percent = QString::number(receipt->GetTransactionDiscountPercent(j), 'f', 2);		
		recount_dis_percent = QString::number(receipt->GetRecountTransactionDiscountPercent(j), 'f', 2);
	    }
	}
	else {     
	    //rate = itoa(0);
	    discount_percent = "0";
	    recount_dis_percent = "0";
	}
	
	if ( receipt->GetDiscountElemRef(j)->dsum < 0 )    {
	    if ( discount_percent == "0" )	strcpy(type, "36");
	    else      strcpy(type, "38");
	    //  red = ftoa(- receipt->GetDiscountElemRef(j)->dsum, 2);
	    //red = NumberToStr(- receipt->GetDiscountElemRef(j)->dsum,2);
	    discount_sum = QString::number(-receipt->GetTransactionDiscountSum(j), 'f', 2);
	    recount_dis_sum = QString::number(-receipt->GetRecountTransactionDiscountSum(j), 'f', 2);
	    //red = dsum_str.ascii();
	}    else    {
	    if ( discount_percent == "0" )  strcpy(type, "35");
	    else        strcpy(type, "37");
	    //    red = ftoa(receipt->GetDiscountElemRef(j)->dsum, 2);
	    //red = NumberToStr(receipt->GetDiscountElemRef(j)->dsum,2);
	    discount_sum = QString::number(receipt->GetTransactionDiscountSum(j), 'f', 2);
	    recount_dis_sum = QString::number(receipt->GetRecountTransactionDiscountSum(j), 'f', 2);   
	    //red = dsum_str.ascii();	    
	}
	
	rtype = itoa(reduction_type);
	
	// для фиксированной скидки по дисконтной карте // ПОТОМ ДОБАВИТЬ ДЛЯ АВТОМАТИЧЕСКОЙ ПО КАРТЕ
	if ( j == 1 ) {
	    if ( strlen(receipt->GetDiscountElemRef(j)->Get_Internal_Code()) > 0 )
		strcpy(wcode, receipt->GetDiscountElemRef(j)->Get_Internal_Code());
	    strcpy(card, receipt->GetDiscountElemRef(j)->Get_Card_Code());
	}
	
	asch = itoa(0);
	
	//printf("Insert_Reduction(type = <%s>, dt, n_pc, seller, n_session, n_rec, n_doc, ct, wcode = <%s>, rtype, card = <%s>, asch, red, rate)");
	er = Tr->Insert_Reduction(type, cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, wcode, rtype, card, asch, discount_sum.ascii(), discount_percent.ascii(), recount_dis_sum.ascii(), recount_dis_percent.ascii());
    }
    
    //if ( red ) free(red);
    //if ( rate  ) free(rate);
    if ( type ) free(type);
    if ( wcode ) free(wcode);
    if ( card ) free(card);
    if ( rtype ) free(rtype);
    if ( asch ) free(asch);
    
    return er;
}

//======================================================================//
int InsertTransactions::InsertTaxTransactions(const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * n_rec, const char * n_doc, const char * check_type, int i)
{
    
    //printf("InsertTaxTransactions(n_pc = %s, n_kkm = %s, seller = %s, n_session = %s, n_rec = %s, n_doc = %s, check_type = %s, i = %d)\n", n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, i);
    
    
    if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    int er = 1;
    if(receipt->GetTaxElemRef(i)->tax_group_id != 0)  {
	char * tax_group = NULL; //(char *) calloc(11, sizeof(char));
	char * tax_rate = NULL; //(char *) calloc(11, sizeof(char));
	QString tax_sum = "";
	
	tax_group = itoa(receipt->GetTaxElemRef(i)->tax_group_id);
	//printf("tax_group = %s \n", tax_group);
	//printf("receipt->GetTaxElemRef(i) = %d \n", receipt->GetTaxElemRef(i));
	//printf("receipt->GetTaxElemRef(i)->num_rates = %d \n", receipt->GetTaxElemRef(i)->num_rates);
	
	if ( receipt->GetTaxElemRef(i) == NULL ) return 0;
	
	for(int j = 0; j < receipt->GetTaxElemRef(i)->num_rates; j ++)    {
	    //printf("receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j) = %d \n", receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j));
	    //printf("receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id = %d \n", receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id);    
	    if ( receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j) == NULL ) break;
	    tax_rate = itoa(receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id);
	    //printf("receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate = %f \n", receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate);
	    tax_sum = QString::number(receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate, 'f', 2);
	    //printf("%d tax_rate = %s tax_sum = %s \n", j, tax_rate, tax_sum.ascii());
	    er = Tr->Insert_Tax("14", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, receipt->GetPosElemRef(i)->GetCode(), tax_group, tax_rate, tax_sum);
	    //printf("Tr->Insert_Tax return -- %d \n", er);
	}
	
	if(tax_group) free(tax_group);
	if(tax_rate) free(tax_rate);
    }
    return er; 
}

//======================================================================//
char * InsertTransactions::NumberToStr(double Nmb, int Factor)
{
 //log->PrintString(DEBLEVEL_REGFORM, INFO, "char * InsertTransactions::NumberToStr(double Nmb=<%4f>)\n",Nmb);   
 
 double Fctr = pow(10,(double) Factor);
 
 Nmb = Round( Nmb*Fctr) / Fctr;
 
 ////printf("After Round Nmb=<%4f>)\n",Nmb);   
 
 int dec, sign, len = 0;
 char * buf = NULL;
 char * res = NULL;
  
 buf = fcvt(Nmb, Factor, &dec, &sign); // пример [-123.45] Nmb = [12345] dec=3 sign=1

 len =strlen(buf); // для примера = 5
 if (len == 0)
 {
     res = (char *) calloc (Factor+3, sizeof(char));
     strcpy(res,"0.");
     for (int i=0;i<Factor;i++)
            strcat(res,"0");
     return res;
 }
 len ++; //для символа [.]  strlen = 6
 len = len + ( Factor - (strlen(buf) - dec)); // для примера strlen = 6 + (2-(5-3)) = 6
 if (sign == 1) 
      len++; // для знака [-] , для примера strlen = 7
 res = (char *) calloc (len + 2, sizeof(char));
 
 len = 0;
 if (sign == 1) 
   {
     strcpy(res,"-");
     len ++;
 }
 
 int i;

 for (i=0; i<dec; i++)
 {
     *(res + len) = *(buf+i);
     len++;
 } 
 if ((len == 0) && (sign == 0) || ((len == 1) && (sign == 1)))
 {
     *(res+len) = '0';
     len++;
 }
*(res+len) = '.';
len++;
if (dec>=0)
{
  for (i=dec; i<strlen(buf); i++)
  {
      *(res + len) = *(buf+i);
      len++;
  }
}
else
{
   for (i=0;i>dec;i--)
    {
       *(res+len) = '0';
       len++;
   }
   for (i=0; i<strlen(buf); i++)
  {
      *(res + len) = *(buf+i);
      len++;
  }
}
 for (i=0; i<(Factor-(strlen(buf)-dec)); i++)
 {
     strcat(res,"0");
 }
 
 ////printf(" не Получилось: [%s]; dec=[%d]; sign=[%d]; res=[%s]\n",buf,dec,sign,res);
    
 return res;
 }

//======================================================================//
int InsertTransactions::InsertPaymentRegistrationTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, double nal, double cred, double cont)
{
 //log->PrintString(DEBLEVEL_REGFORM, INFO, "int InsertTransactions::InsertPaymentRegistrationTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, double nal, double cred, double cont)\n");
   if ( sp->learn_mode && ! sp->write_transaction ) return 1;
    int er = 1;

  char *client_sum = NULL; //(char *) calloc(14, sizeof(char));
  char *change_sum = NULL; //(char *) calloc(14, sizeof(char)); 
  if(cont != 0)   ////// ОПЛАТА ТАРОЙ
  {
////puts("cont");
    //client_sum = ftoa(cont, 2);
    client_sum = NumberToStr(cont,2);
    er = Tr->Insert_Payment("41", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, "3"/*payment_type*/, client_sum/*sum*/, "0"/*change*/);
    if(client_sum) free(client_sum);
    if(er != 1)
      return er;
  }
  if(cred != 0) /////// ОПЛАТА КРЕДИТОМ
  {
////puts("cred");
    //client_sum = ftoa(cred, 2);
   client_sum = NumberToStr(cred,2);
    er = Tr->Insert_Payment("41", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, "2"/*payment_type*/, client_sum/*sum*/, "0"/*change*/);
    if(client_sum) free(client_sum);
    if(er != 1)
      return er;
  }
  if(nal != 0) /////// ОПЛАТА НАЛИЧНЫМИ
  {
//puts("nal");
    double change = nal + cred + cont - (receipt->GetResultSum());
   //  client_sum = ftoa(nal, 2);
    client_sum = NumberToStr(nal,2);        
    //printf("client_sum [%s]\n",client_sum);
   // change_sum = ftoa(change, 2);
    change_sum = NumberToStr(change,2);
    //printf("change_sum [%s]\n",change_sum);
    if(change != 0)
      er = Tr->Insert_Payment("40", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, "1"/*payment_type*/, client_sum, change_sum);
    else
      er = Tr->Insert_Payment("41", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, "1"/*payment_type*/, client_sum, "0");
    if(client_sum) free(client_sum);
    if(change_sum)  free(change_sum);
    if(er != 1)
      return er;
  }
//puts("END OF PAYMENT TRANSACTION");
  return er;
}

//======================================================================//
//-----(*)-----Zuskin-----18/11/2011-----
//int InsertTransactions::InsertCloseCheckTransactions(const char *n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, const char * card, double nal, double cred, double cont, bool close_check)
int InsertTransactions::InsertCloseCheckTransactions(const char *n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, const char * card, double nal, double cred, double cont, bool close_check, string manager_id)
//---------------------------------------
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "int InsertTransactions::InsertCloseCheckTransactions(const char *n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, const char * card, double nal, double cred, double cont)\n");
  if ( sp->learn_mode && ! sp->write_transaction ) return 1;
  int er = 1;
    
  QString client_sum = QString::number(nal + cont + cred, 'f', 2);
  QString check_sum = QString::number(receipt->GetResultSum(), 'f', 2);
  QString round_sum = QString::number(receipt->round_sum, 'f', 2);
  
  //-----(+)-----Zuskin-----10/10/2011-----
  int ct = atoi(check_type);
  if (ct == 99) {
      //-----(*)-----Zuskin-----18/11/2011-----
      //er = Tr->Insert_Close_Check("57", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, "0", card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii());
      er = Tr->Insert_Close_Check("57", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, "0", card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii(), manager_id);
      //---------------------------------------
      return er;
  }
  if (ct == 98) {
      //-----(*)-----Zuskin-----18/11/2011-----
      //er = Tr->Insert_Close_Check("57", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, "1", card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii());
      er = Tr->Insert_Close_Check("57", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, "1", card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii(), manager_id);
      //---------------------------------------
      return er;
  }

  //---------------------------------------

  if ( close_check ) {
      //-----(*)-----Zuskin-----18/11/2011-----
      //er = Tr->Insert_Close_Check("55", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii());
      er = Tr->Insert_Close_Check("55", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii(), manager_id);
      //---------------------------------------
      }
  else {
      //-----(*)-----Zuskin-----18/11/2011-----
      //er = Tr->Insert_Close_Check("56", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii());
      er = Tr->Insert_Close_Check("56", cur_receipt_datetime, n_pc, n_kkm, seller, n_session, n_rec, n_doc, check_type, card, client_sum.ascii(), check_sum.ascii(), round_sum.ascii(), manager_id);
      //---------------------------------------
  }
  
  return er;
}
//======================================================================//
void InsertTransactions::CreateDateString(char * cur_receipt_date)
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "void InsertTransactions::CreateDateString(char * cur_receipt_date)\n");
  hw->ECR_ReadStatus();
  char * str = NULL;
  switch(hw->ECR_GetYear())
  {
    case 98 :
      strcpy(cur_receipt_date, "1998");
      break;
    case 99 :
      strcpy(cur_receipt_date, "1999");
      break;
    default :
    {
      if(hw->ECR_GetYear() < 10)
        strcpy(cur_receipt_date, "200");
      else
        strcpy(cur_receipt_date, "20");
      str = itoa(hw->ECR_GetYear());
      strcat(cur_receipt_date, str);
      if(str) free(str);
    }
  }
	
  strcat(cur_receipt_date, "-");

  if(hw->ECR_GetMonth() < 10) strcat(cur_receipt_date, "0");
  str = itoa(hw->ECR_GetMonth());
  strcat(cur_receipt_date, str);
  if(str) free(str);
  strcat(cur_receipt_date, "-");
  
  if(hw->ECR_GetDay() < 10)
    strcat(cur_receipt_date, "0");
  str = itoa(hw->ECR_GetDay());
  strcat(cur_receipt_date, str);
  if(str) free(str);

  strcat(cur_receipt_date, " ");
  ////printf("<%s>\n", cur_receipt_date);
}

//======================================================================//
void InsertTransactions::CreateTimeString(char * cur_receipt_time)
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "void InsertTransactions::CreateTimeString(char * cur_receipt_time)");
  char * str = NULL;
  hw->ECR_ReadStatus();
  if(hw->ECR_GetHour() < 10)
    strcpy(cur_receipt_time, "0");
  else
    strcpy(cur_receipt_time, "");
  str = itoa(hw->ECR_GetHour());
  strcat(cur_receipt_time, str);
  if(str) free(str);

  strcat(cur_receipt_time, ":");

  if(hw->ECR_GetMinute() < 10)
    strcat(cur_receipt_time, "0");
  str = itoa(hw->ECR_GetMinute());
  strcat(cur_receipt_time, str);
  if(str) free(str);

  strcat(cur_receipt_time, ":");
  
  if(hw->ECR_GetSecond() < 10)
    strcat(cur_receipt_time, "0");
  str = itoa(hw->ECR_GetSecond());
  strcat(cur_receipt_time, str);
  if(str) free(str);
  
 // //printf("<%s>\n",cur_receipt_time);
}
//======================================================================//

