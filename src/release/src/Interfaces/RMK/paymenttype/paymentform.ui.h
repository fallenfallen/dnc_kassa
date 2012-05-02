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
***************************************************************************/

#include <qinputdialog.h>
#include "addon_func.h"
#include "math.h"
#include "hwsrv.h"
#include "const.h"
#include "npaspayform.h"


extern HWSRV *hw;    
extern SP sp;
extern BlockTimer *bt;

//==========================================================//
int paymentForm::GetCreditStatus()
{
    return CreditStatus;
}
//=========================================================//
void paymentForm::SetCheckNum(int check_num)
{
    CheckNum = check_num;
}
//=========================================================//
double paymentForm::GetMaxNalSum() {  return max_nal; }
double paymentForm::GetMaxCredSum() {  return max_credit; }
//=========================================================//
void paymentForm::SetMaxNalSum(double sum)
{
    max_nal = sum;
    QString str = tnalpaymentTextLabel->text();
    str.append(DefCodec->toUnicode("\nмакс ("));
    str.append(QString::number(sum, 'f', 2));
    str.append(")");
    tnalpaymentTextLabel->setText(str);
}
//=========================================================//
void paymentForm::SetMaxCredSum(double sum)
{
    //printf("SetMaxCredSum(%f)\n", sum);
    max_credit = sum;
    QString str = tcredpaymentTextLabel->text();
    str.append(DefCodec->toUnicode("\nмакс ("));
    str.append(QString::number(sum, 'f', 2));
    str.append(")");
    tcredpaymentTextLabel->setText(str);
}
//=======================================================//
void paymentForm::SetReturnType()
{
    //-----(*)-----Zuskin-----25/07/2011-----
   // lockcontPixmapLabel->setEnabled(false);
   // tcontpaymentTextLabel->setEnabled(false);
   // contpaymentLineEdit->setEnabled(false);
    //--------------------------------------------------
    setCaption( DefCodec->toUnicode("Введите суммы возврата") );
    tpaymentTextLabel->setText( DefCodec->toUnicode("К возврату") );
    return_flag = true;
}
//=======================================================//
void paymentForm::SetPaymentType()
{
    lockcontPixmapLabel->setEnabled(true);
    tcontpaymentTextLabel->setEnabled(true);
    contpaymentLineEdit->setEnabled(true);
    setCaption( DefCodec->toUnicode("Введите суммы оплаты") );
    tpaymentTextLabel->setText( DefCodec->toUnicode("К оплате") );
    return_flag = false;
}
//=======================================================//
void paymentForm::InitValidatorProperties()
{
    //puts("paymentForm::InitValidatorProperties()");
    nalpv = new PaymentSumValidator(99999999.99 - credsum - contsum,this);
    credpv = new PaymentSumValidator(paymentsum - nalsum - contsum, this);
    contpv = new PaymentSumValidator(paymentsum - nalsum - credsum, this);
    
    nalpaymentLineEdit->setValidator((const QValidator *)nalpv);
    credpaymentLineEdit->setValidator((const QValidator *)credpv);
    contpaymentLineEdit->setValidator((const QValidator *)contpv);
}

//================================================================//
void paymentForm::init()
{
    //puts("paymentForm::init()"); 
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    ready_flag = false;
    if ( sp.limit_client_sum ) max_kup = sp.max_money;
    else max_kup = 99999999.99;
    min_kup = 0.01;
    nalsum = 0;
    credsum = 0;
    contsum = 0;
    
    nalstr = "";
    credstr = "";
    contstr = "";
    str = "";
    
    editline = NULL;
    
    InitValidatorProperties();
    
    tostTextLabel->setText(DefCodec->toUnicode("Осталось по чеку"));
    
    InitRights();
    InitCurLineEdit();
    InitAccels();
    adjustSize(); 
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();   
    
    CreditStatus = -1;
    return_flag = false;
    max_nal =99999999.99;
    max_credit =99999999.99;
    
    CreditOperationType = 1;
    ReturnAdditSumm = 0;
    
    if ( sp.sections.size() > 0 ) {
	credpaymentLineEdit->setEnabled(false);
	contpaymentLineEdit->setEnabled(false);
	tcontpaymentTextLabel->setEnabled(false);
	tcredpaymentTextLabel->setEnabled(false);
    }
}

//==================================================================//
void paymentForm::InitAccels()
{   
    
    QAccel *redj_accel = new QAccel(this); // выход
    redj_accel->connectItem(redj_accel->insertItem(Key_F12), this, SLOT(reject()));
    
    QAccel *clear_accel = new QAccel(this); // очистка поля
    clear_accel->connectItem(clear_accel->insertItem(Qt::Key_Escape), this, SLOT(ClearCurLineEdit()));
    
    QAccel *tab_accel = new QAccel(this); // табуляция
    tab_accel->connectItem(tab_accel->insertItem(Qt::Key_Tab), this, SLOT(SwitchDownFocus()));
    
    QAccel *revtab_accel = new QAccel(this); // обратная табуляция
    revtab_accel->connectItem(revtab_accel->insertItem(Qt::Key_Tab + SHIFT), this, SLOT(SwitchUpFocus()));
    
    QAccel *tab1_accel = new QAccel(this); //  Enter = Tab
    tab1_accel->connectItem(tab1_accel->insertItem(Qt::Key_Enter), this, SLOT(AcceptedClosePaymentForm()));
    
    QAccel *accept2_accel = new QAccel(this); //  Return = Tab
    accept2_accel->connectItem(accept2_accel->insertItem(Qt::Key_Return), this, SLOT(AcceptedClosePaymentForm()));
    
    QAccel *up_accel = new QAccel(this); //  курсор вверх
    up_accel->connectItem(up_accel->insertItem(Qt::Key_Up), this, SLOT(SwitchUpFocus()));
    
    QAccel *down_accel = new QAccel(this); //  курсор вниз
    down_accel->connectItem(down_accel->insertItem(Qt::Key_Down), this, SLOT(SwitchDownFocus()));
    
    QAccel *dblzero_accel = new QAccel(this); //  курсор вниз
    dblzero_accel->connectItem(dblzero_accel->insertItem(CTRL + Key_0), this, SLOT(AddDoubleZero()));
}

//==============================================================//
void paymentForm::InitRights()
{
    //puts("void paymentForm::InitRights()");
    bool r;
    r = Authorization::GetInstance()->GetRight(15);      // право текущего пользователя на оплату наличными                    
    nalrequestauthflag = !r && sp.query_for_authorization;  // true, если права нет и есть запрос на авторизацию
    if(r) // Есть право на оплату наличными
	locknalPixmapLabel->hide();
    else if(!r && !sp.query_for_authorization)
    {
	nalpaymentLineEdit->setEnabled(false);
	editline = NULL;
    }  
    // if(!r)
    // nalLayout->removeItem(nalSpacer);
    
    r = Authorization::GetInstance()->GetRight(16);      // право текущего пользователя на оплату кредитом                    
    credrequestauthflag = !r && sp.query_for_authorization;  // true, если права нет и есть запрос на авторизацию
    if(r) lockcredPixmapLabel->hide();
    else if(!r && !sp.query_for_authorization)
	credpaymentLineEdit->setEnabled(false);
    //  if(!r)
    //    credLayout->removeItem(credSpacer);
    
    r = Authorization::GetInstance()->GetRight(17);      // право текущего пользователя на оплату тарой                    
    contrequestauthflag = !r && sp.query_for_authorization;  // true, если права нет и есть запрос на авторизацию
    if(r) lockcontPixmapLabel->hide();
    else if(!r && !sp.query_for_authorization)
	contpaymentLineEdit->setEnabled(false);
    //  if(!r)
    //   contLayout->removeItem(contSpacer);  
}

//===========================================================//
void paymentForm::InitCurLineEdit()
{
    if(nalpaymentLineEdit->isEnabled())
	editline = nalpaymentLineEdit;
    else if(credpaymentLineEdit->isEnabled())
	editline = credpaymentLineEdit;
    else if(contpaymentLineEdit->isEnabled())
	editline = contpaymentLineEdit;
}
//============================================================//
void paymentForm::destroy()
{
    
}
//==================================================================//
void paymentForm::SetReadyFlag(bool fl)
{
    //puts("paymentForm::SetReadyFlag(bool fl)");
    ready_flag = fl;
}
//===============================================================//
void paymentForm::SetCreditOperationType(int type)
{
    CreditOperationType = type;
}
//===============================================================//
void paymentForm::SetReturnAdditSumm(double s)
{
    ReturnAdditSumm = s;
}
//===============================================================//
void paymentForm::SetPaymentSum(double s)
{
    //puts("paymentForm::SetPaymentSum(double s)");
    QString str;
    str.setNum(s,'f', 2);
    paymentTextLabel->setText(str);
    paymentsum = s;
    SetValidatorProperties();
}
//==============================================================//
void paymentForm::SetCredPaymentSum(double s)
{
    //puts("paymentForm::SetNalPaymentSum(double s)");
    QString str;
    if ( Authorization::GetInstance()->GetRight(16) || credrequestauthflag )  {
	str.setNum(s,'f', 2);
	credpaymentLineEdit->setText(str);
	kredit = str;
	
    }  else  {
	if (s > 0)      {
	    s = 0;
	    str.setNum(s,'f', 2);
	    Message(4);       
	}
    }
    credsum = s;
    credstr = str;
    
    //    nalsum = s;
    //    nalstr = str;
    
    allTextLabel->setText(str);
    double ost_sum = paymentsum - nalsum - credsum - contsum;
    // Сдача
    if ( ost_sum <= 0 )     {
	if ( return_flag ) tostTextLabel->setText(DefCodec->toUnicode("Избыток по чеку"));
	else    tostTextLabel->setText(DefCodec->toUnicode("Сдача"));
	ost_sum = -ost_sum;
    }
    str.setNum(ost_sum, 'f', 2);
    ostTextLabel->setText(str);
    SetValidatorProperties();
}
//===================================================================//
int paymentForm::SetNalPaymentSum(double s)
{
    //puts("paymentForm::SetNalPaymentSum(double s)");
    QString str;
    if ( Authorization::GetInstance()->GetRight(15) || nalrequestauthflag )    {
	str.setNum(s,'f', 2);	
	//printf("sp.max_money = %f \n", sp.max_money);
	//printf("paymentsum = %f \n", paymentsum);
	if ( sp.limit_client_sum && (s > sp.max_money + paymentsum ) ) {
	    Message(5);
	    return -1;
	} else {	
	    nalpaymentLineEdit->setText(str);  
	    nal = str;
	    //printf("nal = |%s|\n",nal.ascii());
	}
    }    else    {
	if (s > 0)	{
	    s = 0;
	    str.setNum(s,'f', 2);
	    Message(4);       
	}
    }
    
    
    nalsum = s;
    nalstr = str;
    allTextLabel->setText(str);
    double ost_sum = paymentsum - nalsum - credsum - contsum;
    // Сдача
    if ( ost_sum <= 0 )     {
	if ( ! return_flag ) tostTextLabel->setText(DefCodec->toUnicode("Сдача"));
	else tostTextLabel->setText(DefCodec->toUnicode("Избыток по чеку"));
	ost_sum = -ost_sum;
    }
    str.setNum(ost_sum, 'f', 2);
    ostTextLabel->setText(str);
    SetValidatorProperties();
    return 0;
}

//==========================================================//
void paymentForm::SetNalIntoNal(double s)
{
    QString str;
    str.setNum(s,'f', 2);
    nalpaymentLineEdit->setText(str);
}
//==============================================================//
void paymentForm::SetValidatorProperties()
{  
    /*if ( return_flag ) {	
	if ( paymentsum - credsum < max_nal ) nalpv->SetMaxSum(paymentsum - credsum);
	else nalpv->SetMaxSum(max_nal);
	
	if ( paymentsum - nalsum < max_credit ) credpv->SetMaxSum(paymentsum - nalsum);
	credpv->SetMaxSum(max_credit);
    } else {
	if ( paymentsum<max_kup ) {
	    nalpv->SetMaxSum(max_kup + paymentsum - min_kup - credsum - contsum);
	}    else {
	    int col_max_kup = (int)paymentsum/max_kup;
	    nalpv->SetMaxSum( max_kup*col_max_kup+max_kup - credsum - contsum );
	} 

	credpv->SetMaxSum(paymentsum - nalsum - contsum);
	contpv->SetMaxSum(paymentsum - nalsum - credsum);
    }*/
    
    /*printf("paymentsum = %f \n", paymentsum);
    printf("nalsum = %f \n", nalsum);
    printf("credsum = %f \n", credsum);
    printf("contsum = %f \n", contsum);
    
    printf("Cred SetMax(%f)\n", paymentsum - nalsum - contsum);
    printf("Cont SetMax(%f)\n", paymentsum - nalsum - credsum);*/
    
    /*printf("paymentsum = %f \n", paymentsum);
    printf("nalsum = %f \n", nalsum);
    printf("max of nalsum = %f \n", max_nal);
    printf("credsum = %f \n", credsum);
    printf("max_credit = %f \n", max_credit);
    printf("max_credit = %f \n", max_credit);*/
    
    
    
    if ( return_flag ) {	
	//printf("1 nal ogr = %f \n", paymentsum - credsum);
	//printf("2 nal ogr = %f \n", max_nal);
	
	//-----(*)-----Zuskin-----25/07/2011-----
	//if ( (paymentsum - credsum) < max_nal ) nalpv->SetMaxSum(paymentsum - credsum);
	if ( (paymentsum - credsum - contsum) < max_nal ) nalpv->SetMaxSum(paymentsum - credsum - contsum);
	else nalpv->SetMaxSum(max_nal);
	//---------------------------------------
	
	//printf("1 cred ogr = %f \n", paymentsum - nalsum);
	//printf("2 cred ogr = %f \n", max_credit);
	//-----(*)-----Zuskin-----25/07/2011-----
	//if ( (paymentsum - nalsum) < max_credit ) credpv->SetMaxSum(paymentsum - nalsum);
	if ( (paymentsum - nalsum - contsum) < max_credit ) credpv->SetMaxSum(paymentsum - nalsum - contsum);
	else credpv->SetMaxSum(max_credit);
	//---------------------------------------
	
	//-----(+)-----Zuskin-----25/07/2011-----
	if ( (paymentsum - nalsum - credsum) < max_cont ) contpv->SetMaxSum(paymentsum - nalsum - credsum);
	else contpv->SetMaxSum(max_cont);
	//--------------------------------------------------
    } else {
	if (paymentsum > credsum + contsum) {
	    // будет не логично если сумма наличными будет больше суммы чека, те сдача будет равна макс купюре!
	    // поэтому - 1 копейка
	    nalpv->SetMaxSum(paymentsum + max_kup - credsum - contsum - 0.01); 	
	}  else {
	    nalpv->SetMaxSum(0); 
	}	
	
	/*printf("paymentsum = %f \n", paymentsum);
	printf("nalsum = %f \n", nalsum);
	printf("credsum = %f \n", credsum);
	printf("contsum = %f \n", contsum);
	
	printf("Cred SetMax(%f)\n", paymentsum - nalsum - contsum);
	printf("Cont SetMax(%f)\n", paymentsum - nalsum - credsum);*/
	
	credpv->SetMaxSum(paymentsum - nalsum - contsum);
	contpv->SetMaxSum(paymentsum - nalsum - credsum);   
    }
    
    
}
//================================================================//
void paymentForm::DeleteValidatorProperties()
{
    delete(nalpv);
    delete(credpv);
    delete(contpv);
}
//================================================================//
double paymentForm::GetNalSum() { return nalsum;   }
double paymentForm::GetCredSum() { return credsum;   }
double paymentForm::GetContSum() { return contsum;   }
//================================================================//
void paymentForm::GetSumParameters(double &nal, double &cred, double &cont, double &psum)
{
    nal = nalsum;
    cred = credsum;
    cont = contsum;
    psum = paymentTextLabel->text().toDouble();
}

//================================================================//
void paymentForm::GetRightsParameters(int *& mas_index, int & num)
{
    //puts("void paymentForm::GetRightsParameters(int *& mas_index, int & num)");
    int ind1 = -1, ind2 = -1, ind3 = -1;
    num = 0;
    if ( nalrequestauthflag && (nalsum > 0) )    {
	//puts("nal");
	num++;
	ind1 = 15;
    }
    if ( credrequestauthflag && (credsum > 0) )    {
	//puts("cred");
	num++;
	ind2 = 16;
    }
    if ( contrequestauthflag && (contsum > 0) )    {
	//puts("cont");
	num++;
	ind3 = 17;
    }
    //printf("num = %d\n", num);
    // int indexes[num];
    mas_index = (int *) malloc(num * sizeof(int));
    int i = 0;
    if ( ind1 != -1 )    {
	mas_index[i] = ind1;
	i++;
    }
    if ( ind2 != -1 )    {
	mas_index[i] = ind2;
	i++;
    }
    if ( ind3 != -1 )    {
	mas_index[i] = ind3;
	i++;
    }  
    //for(int i = 0; i < num; i ++)
    //printf("mas_index[%d] = %d\n", i, mas_index[i]);
}

//===============================================================//
void paymentForm::SaveChecked(QString s)
{
    //puts("paymentForm::SaveChecked(QString s)");
    QWidget *le;
    le = (QWidget *) nalpaymentLineEdit;
    if(le->hasFocus())
    {
	nalstr = editline->text();
	nalsum = s.toDouble();
	//printf("nalsum = %f\n", nalsum);
	return;
    }
    le = (QWidget *) credpaymentLineEdit;
    if(le->hasFocus())
    {
	credstr = editline->text();
	credsum = s.toDouble();
	//printf("credsum = %f\n", credsum);
	return;
    }
    le = (QWidget *) contpaymentLineEdit;
    if(le->hasFocus())
    {
	contstr = editline->text();
	contsum = s.toDouble();
	//printf("contsum = %f\n", contsum);
	return;
    }
}
//==============================================================//
void paymentForm::CommaTest( const QString & kstr)
{
    //    printf("kstr = %s \n", kstr.ascii());
    int k = kstr.length();  // длина строки
    
    char* str = (char*) calloc(k+1, sizeof(char));
    strcpy(str, kstr.ascii());
    
    if ( *(str+k-1) == ',' ) {
	*(str+k-1) = '.';	
	QWidget * focus_widget = this->focusData()->focusWidget();
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) { 
	    ((QLineEdit*)focus_widget)->setText(str);	
	}	
    }
    
    free(str);
}
//==============================================================//
void paymentForm::CheckValue()
{        
    plus_flag=false;
    
    if ( ready_flag ) { 
	ready_flag = false;   //чтобы одновременно два действия не выполнялись
	QString s = editline->text();
	if ( s.find("+") != -1 )	{
	    s.remove("+");
	    editline->setText(s);
	    plus_flag = true;
	}    else    {
	    if (editline == nalpaymentLineEdit)	    nal = nalpaymentLineEdit->text();
	    if (editline == credpaymentLineEdit)	    kredit = credpaymentLineEdit->text();
	    if (editline == contpaymentLineEdit)	    tara = contpaymentLineEdit->text();  
	}
	
	SaveChecked(s);
	QString ost_str;
	if ( paymentsum - nalsum - contsum - credsum > 0 )    {
	    ost_str.setNum(paymentsum - nalsum - contsum - credsum, 'f', 2);
	    ostTextLabel->setText(ost_str);
	    tostTextLabel->setText(DefCodec->toUnicode("Осталось по чеку"));
	}  else	{
	    ost_str.setNum(-paymentsum + nalsum + contsum + credsum, 'f', 2);
	    ostTextLabel->setText(ost_str);
	    if ( ! return_flag ) tostTextLabel->setText(DefCodec->toUnicode("Сдача"));
	    else tostTextLabel->setText(DefCodec->toUnicode("Избыток по чеку"));
	}
	
	SetValidatorProperties();
	str.setNum(nalsum + credsum + contsum, 'f', 2);
	allTextLabel->setText(str);
	ready_flag = true;
	if ( plus_flag == true )	  AcceptedClosePaymentForm();
    }
}

//===============================================================//
void paymentForm::RejectedClosePaymentForm()
{
    //puts("void paymentForm::RejectedClosePaymentForm()");
    nalsum = -1;
    credsum = -1;
    contsum = -1;
    DeleteValidatorProperties();
    reject();
}

//===============================================================//
void paymentForm::AcceptedClosePaymentForm()
{    
    nalsum = nal.toDouble();
    contsum = tara.toDouble();
    credsum = kredit.toDouble();
    
    // inpas
    if ( credsum > 0 ) {	
	if ( sp.learn_mode && hw->PS_GetType() != 0 ) {
	    // данная функция в обучающем режиме не доступна
	    Message(6);   
	    return;
	} 
	
	if (( hw->PS_GetType() == 0 ) || (hw->ECR_GetPortNumber() == 0)){ 
	    // Платежная система не подключена
	    CreditStatus = 0;
	} else { 
	    npaspayForm* pf = new npaspayForm();
	    
	    pf->setOperationType(CreditOperationType);
	    
	    pf->setCheckSumm(credsum);
	    pf->setNumCheck(CheckNum);
	    //------------------24/02/2012--------------- Mirabell changes for Sber
	    int TypeSystem = hw->PS_GetTypeSystem();
	    if (( CreditOperationType == 3 ) && ( TypeSystem!=2 )) {
		QString RRN_str = QInputDialog::getText(
			DefCodec->toUnicode("Введите номер ссылки c чека (RRN)"), 
			DefCodec->toUnicode("Введите номер ссылки c чека (RRN)")
			);
		//printf("RRN = %s \n", RRN_str.ascii());
		if ( RRN_str == NULL ) { delete pf; return;  }
		pf->setRRN(RRN_str);
	    }
	    if ( return_flag ) pf->setAdditSumm(ReturnAdditSumm);	
	    CreditStatus = pf->Payment();
	    //printf("CreditStatus = %d \n", CreditStatus);
	    
	    if ( pf->exec() == QDialog::Rejected ) {  delete pf;   return;   }
	    delete pf;
	    
	    //printf("CreditStatus = %d \n", CreditStatus);
	    if ( CreditStatus != 1 ) return;
	}
	
    }
    
    /*
  puts("печать");
  printf("paymentsum = %f \n", paymentsum);
  printf("nalsum = %f \n",nalsum);
  printf("contsum = %f \n", contsum);
  printf("credsum = %f \n", credsum);*/
    
    if ( Round(paymentsum * 100) > Round((nalsum + contsum + credsum)*100) ) return;
    DeleteValidatorProperties();
    accept();
    //close();
}

//===============================================================//
void paymentForm::ClearCurLineEdit()
{
    //puts("void paymentForm::ClearCurLineEdit()");  
    editline->setText("");
    nalsum = nal.toDouble();
    contsum = tara.toDouble();
    credsum = kredit.toDouble();
}

//=============================================================//
void paymentForm::AddSufixString(QLineEdit *le)
{
    //puts("paymentForm::AddSufixString(QLineEdit *le)");
    QString s = le->text();
    QString end;
    if(s != "")
    {
	end =  s.right(3);
	if ( end.left(1) != "." )  {
	    if ( end.right(2).left(1) == "." ) s = s + "0";
	    else if ( end.right(1) == "." ) s = s + "00";
	    else s = s + ".00";
	}
	le->setText(s);
    }
}

//================================================================/
void paymentForm::SwitchUpFocus()
{
    //puts("paymentForm::SwitchUpFocus()");
    QWidget * le;
    le = (QWidget*) nalpaymentLineEdit;
    if(le->hasFocus())
    {
	AddSufixString((QLineEdit *)le);
	if (!selectContEdit())
	    selectCredEdit();
	return;
    }
    le = (QWidget*) credpaymentLineEdit;
    if(le->hasFocus())
    {
	AddSufixString((QLineEdit *)le);
	if (!selectNalEdit())
	    selectContEdit();
	return;
    }
    le = (QWidget*) contpaymentLineEdit;
    if(le->hasFocus()) 
    {
	AddSufixString((QLineEdit *)le);
	if (!selectCredEdit())
	    selectNalEdit();
	return;
    }
}

//=====================================================================//
void paymentForm::SwitchDownFocus()
{
    //puts("paymentForm::SwitchDownFocus()");
    QWidget * le;
    le = (QWidget*) nalpaymentLineEdit;
    if(le->hasFocus())
    {
	AddSufixString((QLineEdit *)le);
	if (!selectCredEdit())
	    selectContEdit();
	return;
    }
    le = (QWidget*) credpaymentLineEdit;
    if(le->hasFocus())
    {
	AddSufixString((QLineEdit *)le);
	if (!selectContEdit())
	    selectNalEdit();
	return;
    }
    le = (QWidget*) contpaymentLineEdit;
    if(le->hasFocus()) 
    {
	AddSufixString((QLineEdit *)le);
	if (!selectNalEdit())
	    selectCredEdit();
	return;
    }
}

//======================================================================//
void paymentForm::Message(int mes)
{
    //puts("void paymentForm::Message(int mes)");
    //  releaseKeyboard();
    QString mes_id, mes_str;
    RegistrationMessageText(mes, mes_str, mes_id);
    InterfaceErrorWin(mes_str, mes_id);
    
}
//======================================================================//
bool paymentForm::selectNalEdit()
{   
    if (!nalpaymentLineEdit->isEnabled()) return false;
    QWidget * le;
    le = (QWidget *) nalpaymentLineEdit;
    le->setFocus();
    nalpaymentLineEdit->selectAll();
    editline = nalpaymentLineEdit;
    str = nalstr;
    return true;
}
//======================================================================//

bool paymentForm::selectCredEdit()
{  
    if (!credpaymentLineEdit->isEnabled()) return false;
    QWidget * le;
    le = (QWidget *) credpaymentLineEdit;
    le->setFocus();
    credpaymentLineEdit->selectAll();
    editline = credpaymentLineEdit;
    str = credstr;
    return true;
}
//======================================================================//

bool paymentForm::selectContEdit()
{
    if (!contpaymentLineEdit->isEnabled()) return false;
    QWidget * le;
    le = (QWidget *) contpaymentLineEdit;
    le->setFocus();
    contpaymentLineEdit->selectAll();
    editline = contpaymentLineEdit;
    str = contstr;
    return true;
}
//======================================================================//

void paymentForm::AddDoubleZero()
{
    QKeyEvent kev(QEvent::QEvent::KeyPress, Qt::Key_0, Qt::Key_0, 0, "0");
    QApplication::sendEvent( editline, &kev );
    QApplication::sendEvent( editline, &kev );
}
//======================================================================//

void paymentForm::mouseSelect()
{
    if (nalpaymentLineEdit->hasFocus())  {
	selectNalEdit();
	return;
    }	
    if (credpaymentLineEdit->hasFocus()) {
	selectCredEdit();
	return;
    }	
        if (contpaymentLineEdit->hasFocus()) {
	selectContEdit();
	return;
    }	
    return;	
}
//======================================================================//

//-----(+)-----Zuskin-----25/07/2011-----
void paymentForm::SetMaxContSum( double sum )
{
    max_cont = sum;
    QString str = tcontpaymentTextLabel->text();
    str.append(DefCodec->toUnicode("\nмакс ("));
    str.append(QString::number(sum, 'f', 2));
    str.append(")");
    tcontpaymentTextLabel->setText(str);
}

void paymentForm::SetContPaymentSum( double s )
{
    QString str;
    if ( Authorization::GetInstance()->GetRight(17) || contrequestauthflag )  {
	str.setNum(s,'f', 2);
	contpaymentLineEdit->setText(str);
	tara = str;
	
    }  else  {
	if (s > 0)      {
	    s = 0;
	    str.setNum(s,'f', 2);
	    Message(4);       
	}
    }
    contsum = s;
    contstr = str;
    
    //    nalsum = s;
    //    nalstr = str;
    
    allTextLabel->setText(str);
    double ost_sum = paymentsum - nalsum - credsum - contsum;
    // Сдача
    if ( ost_sum <= 0 )     {
	if ( return_flag ) tostTextLabel->setText(DefCodec->toUnicode("Избыток по чеку"));
	else    tostTextLabel->setText(DefCodec->toUnicode("Сдача"));
	ost_sum = -ost_sum;
    }
    str.setNum(ost_sum, 'f', 2);
    ostTextLabel->setText(str);
    SetValidatorProperties();
}
//----------------------------------------------------
