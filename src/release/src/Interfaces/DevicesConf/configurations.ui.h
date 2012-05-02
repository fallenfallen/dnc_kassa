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
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


#define  max_terminal_id "9999999999999999"
using namespace std;
QIntValidator * PrefSuf;
QValidator * AccP;
QIntValidator * AdmP;
QIntValidator * PsTime;
QIntValidator * PsTermId;
QIntValidator * PsIpPort;
QIntValidator * vc;
QIntValidator * FRMBoxImp;
QIntValidator * FRMBoxIcnt;
QValidator *validator;
QValidator * CardMask;

pid_t ParentPid;
pid_t ChildPid;

int newVariable;
int OldNameLength;
int DSN;
int ErrorNum;
int DL;
int CodePage;
int PortFileDescr;
int num_blink;
int PortNumber;
int BaudRate;
int Model;
int ScrollSpeed;
int NumberProt;

QTextCodec* CP1251Codec;

bool BCSSufPref;
bool SaveFlag;
bool change;
bool conf_file_exist;
bool stop_flag;
bool CaptionFlag;
bool flag;
bool delay_flag;
bool StartSt;
bool changeflag;
bool FlagInLoading;
bool PS_on;
bool show_message;

QTimer* qtimer;
DialogNewScheme * nscheme;
NewPerecodirovka *n_p;

char* way;
char Brightness;

QString UsbCom;
QString Com;
QString Usb;
QString Acm;
QString errmes;

DEBUG_LEVEL_ENUM DebugLevel;
DebugLevelLog *dl;

extern QTextCodec* DefCodec;
extern int p1;
extern HWSRV *hw;
extern int p;
extern int klava;
extern int AutoScanner;

DedicationComboBox * BCSPortNum;
DedicationComboBox * BCSBaudRate;
DedicationComboBox * CDSPPortNum;
DedicationComboBox * CDSPBaudRate;
DedicationComboBox * CDSPNumberProt;
DedicationComboBox * CDSPPerec;
DedicationComboBox * CDSPBrightness;
DedicationComboBox * CDSPScrollSpeed;
DedicationComboBox * CDSPLengthStr;
DedicationComboBox * CDSP_UP_Align_comboBox;
DedicationComboBox * CDSP_DOWN_Align_comboBox;
DedicationComboBox * FRTypeDevice;
DedicationComboBox * FRPortNum;
DedicationComboBox * FRBaudRate;
DedicationComboBox * FRProtocol;
DedicationComboBox * FRMBoxCNL;
DedicationComboBox * MCRPortNum;
DedicationComboBox * MCRBaudRate;
DedicationComboBox * EWPortNum;
DedicationComboBox * EWBaudRate;
DedicationComboBox * EWModel;
DedicationComboBox * PSType;
DedicationComboBox * PSTypeScheme;
DedicationComboBox * PSTypeExchange;
DedicationComboBox * PSPortVerifone;
DedicationComboBox * PSBaudRateVerifone;
DedicationComboBox * DebugLevelComboBox;


struct USBStruct{
	 int usbNum;
	 int idProd;
	 int idVend;
};

vector <USBStruct> usb_struct;


struct Protocol{
	 QString nameProtocol;//название протокола
	 int numProtocol;//его номер в соответствии с драйвером
};


vector <Protocol> FR_Protocol;

//======================================================//
//заполнить вектор соответствий имен протоколов и номеров
void Configurations::InsertProtocol(int type_device){
	 FR_Protocol.clear();
	 if (type_device == 0){
		  Protocol prot = {DefCodec->toUnicode("ШТРИХ"),1};
		  FR_Protocol.push_back(prot);
		  Protocol prot1 = {DefCodec->toUnicode("АТОЛ"),2};
		  FR_Protocol.push_back(prot1);
		  Protocol prot2 = {DefCodec->toUnicode("Меркурий"),3};
		  FR_Protocol.push_back(prot2);
		  Protocol prot3 = {DefCodec->toUnicode("Касби-ФР"),4};
		  FR_Protocol.push_back(prot3);
		  Protocol prot4 = {DefCodec->toUnicode("ПРИМ"),5};
		  FR_Protocol.push_back(prot4);
		  Protocol prot5 = {DefCodec->toUnicode("Касби-02"),6};
		  FR_Protocol.push_back(prot5);
		  Protocol prot6 = {DefCodec->toUnicode("Касби-04"),7};
		  FR_Protocol.push_back(prot6);
		  //-----(+)-----Zuskin-----24/08/2011-----
		  Protocol prot7 = {DefCodec->toUnicode("ОРИОН-ФР"),8};
		  FR_Protocol.push_back(prot7);
		  //--------------------------------------------------
	 }
	 else{
		  Protocol prot5 = {DefCodec->toUnicode("ESC_POS"),1};
		  FR_Protocol.push_back(prot5);
	 }
}	 
	 
//======================================================//
//получить номер протокола по имени и типу устройства
int Configurations::GetNumByNameAndType(QString name, int Type){
  // printf("GetNumByNameAndType(%s, %d)\n",name.ascii(),Type);
  switch(Type){
    case 1:{//FR_Protocol
      for (unsigned int i = 0; i<FR_Protocol.size();i++){
        QString str = DefCodec->fromUnicode(FR_Protocol[i].nameProtocol);
        if (name == str){ 
          //  printf("str = %s\n",str.ascii());
          //printf(" GetNumByNameAndType return= %d\n", FR_Protocol[i].numProtocol);
          return FR_Protocol[i].numProtocol;
        }
      }
      break;
    }
    default:{
      return 0;
    }
  }
  return 0;
}


//======================================================//
//добавление комбо-боксов на форму по закладкам
void Configurations::AddComboBoxes()
{
    //----------------------------------------BCS----------------------------------------------------------//  
    BCSPortNum = new DedicationComboBox( BCSPortNum1->parentWidget(), "r");
    QPoint p18 = BCSPortNum1->pos();
    layout1_1->add(BCSPortNum);
    delete(BCSPortNum1);
    BCSPortNum->move(p18);
    
    BCSBaudRate = new DedicationComboBox(BCSBaudRate1->parentWidget(), "r");
    QPoint p17 = BCSBaudRate1->pos();
    layout1_2->add(BCSBaudRate);
    delete(BCSBaudRate1);
    BCSBaudRate->insertItem("2400");
    BCSBaudRate->insertItem("4800");
    BCSBaudRate->insertItem("9600");
    BCSBaudRate->insertItem("19200");
    BCSBaudRate->insertItem("38400");
    BCSBaudRate->insertItem("57600");
    BCSBaudRate->insertItem("115200"); 
    BCSBaudRate->move(p17);
   //---------------------------------------CDSP----------------------------------------------------------//     
    CDSPPortNum = new DedicationComboBox(CDSPPortNum1->parentWidget(), "r");
    QPoint p15 = CDSPPortNum1->pos();
    layout2_1->add(CDSPPortNum);
    delete(CDSPPortNum1);
    CDSPPortNum->move(p15);
    
    CDSPBaudRate = new DedicationComboBox(CDSPBaudRate1->parentWidget(), "r");
    QPoint p16 = CDSPBaudRate1->pos();
    layout2_2->add(CDSPBaudRate);
    delete(CDSPBaudRate1);
    CDSPBaudRate->insertItem("2400");
    CDSPBaudRate->insertItem("4800");
    CDSPBaudRate->insertItem("9600");
    CDSPBaudRate->insertItem("19200");
    CDSPBaudRate->insertItem("38400");
    CDSPBaudRate->insertItem("57600");
    CDSPBaudRate->insertItem("115200"); 
    CDSPBaudRate->move(p16);
    
    CDSPNumberProt = new DedicationComboBox(CDSPNumberProt1->parentWidget(), "r");
    QPoint p14 = CDSPNumberProt1->pos();
    layout2_3->add(CDSPNumberProt);
    delete(CDSPNumberProt1);
    CDSPNumberProt->insertItem("Aedex");
    CDSPNumberProt->insertItem("CD5220");
    CDSPNumberProt->insertItem("Epson");
    CDSPNumberProt->insertItem("DSP800");
    CDSPNumberProt->insertItem("Promag Protocol");
    CDSPNumberProt->move(p14);
    
    CDSPPerec = new DedicationComboBox(CDSPPerec1->parentWidget(), "r");
    QPoint p13 = CDSPPerec1->pos();
    layout2_4->add(CDSPPerec);
    delete(CDSPPerec1);
    CDSPPerec->move(p13);
   
    CDSPBrightness = new DedicationComboBox(CDSPBrightness1->parentWidget(), "r");
    QPoint p12 = CDSPBrightness1->pos();
    layout2_5->add(CDSPBrightness);
    delete(CDSPBrightness1);
    CDSPBrightness->insertItem("1");
    CDSPBrightness->insertItem("2");
    CDSPBrightness->insertItem("3");
    CDSPBrightness->insertItem("4");
    CDSPBrightness->move(p12);
    
    CDSPScrollSpeed = new DedicationComboBox(CDSPScrollSpeed1->parentWidget(), "r");
    QPoint p11 = CDSPScrollSpeed1->pos();
    layout2_6->add(CDSPScrollSpeed);
    delete(CDSPScrollSpeed1);
    CDSPScrollSpeed->insertItem("1");
    CDSPScrollSpeed->insertItem("2");
    CDSPScrollSpeed->insertItem("3");
    CDSPScrollSpeed->insertItem("4");
    CDSPScrollSpeed->move(p11);
	 
	 
	CDSPLengthStr = new DedicationComboBox(CDSPLengthStr1->parentWidget(),"r");
	QPoint p28 = CDSPLengthStr1->pos();
	layout2_6_2->add(CDSPLengthStr);
	delete(CDSPLengthStr1);
	QString ss = "";
	for(unsigned int i = 1; i<41;i++){
		 ss.setNum(i);
		 CDSPLengthStr->insertItem(ss);
	}
	CDSPLengthStr->move(p28);
	 
    
    CDSP_UP_Align_comboBox = new DedicationComboBox(CDSP_UP_Align_comboBox1->parentWidget(), "r");
    QPoint p10 = CDSP_UP_Align_comboBox1->pos();
    layout2_11->add(CDSP_UP_Align_comboBox);
    delete(CDSP_UP_Align_comboBox1);
    CDSP_UP_Align_comboBox->insertItem(DefCodec->toUnicode("по левому краю"));
    CDSP_UP_Align_comboBox->insertItem(DefCodec->toUnicode("по центру"));
    CDSP_UP_Align_comboBox->insertItem(DefCodec->toUnicode("по правому краю"));
    CDSP_UP_Align_comboBox->move(p10);
    
    CDSP_DOWN_Align_comboBox = new DedicationComboBox(CDSP_DOWN_Align_comboBox1->parentWidget(), "r");
    QPoint p9 = CDSP_DOWN_Align_comboBox1->pos();
    layout2_12->add(CDSP_DOWN_Align_comboBox);
    delete(CDSP_DOWN_Align_comboBox1);
    CDSP_DOWN_Align_comboBox->insertItem(DefCodec->toUnicode("по левому краю"));
    CDSP_DOWN_Align_comboBox->insertItem(DefCodec->toUnicode("по центру"));
    CDSP_DOWN_Align_comboBox->insertItem(DefCodec->toUnicode("по правому краю"));
    CDSP_DOWN_Align_comboBox->move(p9);
    //------------------------------------FR---------------------------------------------------------------//  
	 
    FRPortNum = new DedicationComboBox(FRPortNum1->parentWidget(), "r");
    QPoint p8 = FRPortNum1->pos();
    layout3_1->add(FRPortNum);
    delete(FRPortNum1);
    FRPortNum->move(p8);
    
    FRBaudRate = new DedicationComboBox(FRBaudRate1->parentWidget(), "r");
    QPoint p7 = FRBaudRate1->pos();
    layout3_2->add(FRBaudRate);
    delete(FRBaudRate1);
    FRBaudRate->insertItem("2400");
    FRBaudRate->insertItem("4800");
    FRBaudRate->insertItem("9600");
    FRBaudRate->insertItem("19200");
    FRBaudRate->insertItem("38400");
    FRBaudRate->insertItem("57600");
    FRBaudRate->insertItem("115200"); 
    FRBaudRate->move(p7);
	 
    FRTypeDevice = new DedicationComboBox(FRTypeDevice1->parentWidget(), "r");
    QPoint p_fr_type = FRTypeDevice1->pos();
    layout3_9->add(FRTypeDevice);
    delete(FRTypeDevice1);
    FRTypeDevice->insertItem(DefCodec->toUnicode("Фискальный регистратор")); 
    FRTypeDevice->insertItem(DefCodec->toUnicode("Принтер чеков")); 
    FRTypeDevice->move(p_fr_type);
	 
	 
    InsertProtocol(0);
    FRProtocol = new DedicationComboBox( FRProtocol1->parentWidget(), "r");
    QPoint p6 = FRProtocol1->pos();
    layout3_3->add(FRProtocol);
    delete(FRProtocol1);
	 
	 for (unsigned int i = 0; i<FR_Protocol.size();i++){
		FRProtocol->insertItem(FR_Protocol[i].nameProtocol);
	 }
    FRProtocol->move(p6);
	 
    FRMBoxCNL = new DedicationComboBox(FRMBoxCNL1->parentWidget(), "r");
    QPoint p19 = FRMBoxCNL1->pos();
    layout3_10->add(FRMBoxCNL); 
    delete(FRMBoxCNL1);
    FRMBoxCNL->insertItem("      1");
    FRMBoxCNL->insertItem("      2");
    FRMBoxCNL->move(p19);
	 
   //---------------------------------------------MCR-----------------------------------------------------//  
    MCRPortNum = new DedicationComboBox(MCRPortNum1->parentWidget(), "r");
    QPoint p4 = MCRPortNum1->pos(); 
    layout4_1->add(MCRPortNum);
    delete(MCRPortNum1);
    MCRPortNum->move(p4);
    
    MCRBaudRate = new DedicationComboBox( MCRBaudRate1->parentWidget(), "r");
    QPoint p3 =MCRBaudRate1->pos();
    layout4_2->add(MCRBaudRate);
    delete(MCRBaudRate1);
    MCRBaudRate->insertItem(DefCodec->toUnicode("1200"));
    MCRBaudRate->insertItem(DefCodec->toUnicode("2400"));
    MCRBaudRate->insertItem(DefCodec->toUnicode("9600"));
    MCRBaudRate->insertItem(DefCodec->toUnicode("19200"));
    MCRBaudRate->move(p3);
    //----------------------------------------------EW----------------------------------------------------//  
    EWPortNum = new DedicationComboBox(EWPortNum1->parentWidget(), "r");
    QPoint p20 = EWPortNum1->pos();
    layout6_1->add(EWPortNum);
    delete(EWPortNum1);
    EWPortNum->move(p20);
    
    EWBaudRate =  new DedicationComboBox(EWBaudRate1->parentWidget(), "r");
    QPoint p1 = EWBaudRate1->pos();
    layout6_2->add(EWBaudRate);
    delete(EWBaudRate1);
    EWBaudRate->insertItem("2400");
    EWBaudRate->insertItem("4800");
    EWBaudRate->insertItem("9600");
    EWBaudRate->insertItem("19200");
    EWBaudRate->insertItem("38400");
    EWBaudRate->insertItem("57600");
    EWBaudRate->insertItem("115200"); 
    EWBaudRate->move(p1);
    
    EWModel =  new DedicationComboBox( EWModel1->parentWidget(), "r");
    QPoint p2 =EWModel1->pos();
    layout6_3->add(EWModel);
    delete(EWModel1);
    EWModel->insertItem(DefCodec->toUnicode("Масса-К протокол №2"));
    //EWModel->insertItem(DefCodec->toUnicode("Масса-К протокол №8"));
    EWModel->insertItem(DefCodec->toUnicode("Меркурий"));
    EWModel->insertItem(DefCodec->toUnicode("CAS"));
//    EWModel->insertItem(DefCodec->toUnicode("Штрих-Принт"));
//    EWModel->insertItem(DefCodec->toUnicode("Штрих АС мини"));
    EWModel->move(p2); 
   
       
    //------------------------------------------------PS-----------------------------------------------------//
	 
  PSType = new DedicationComboBox(PSType1->parentWidget(),"r");
  QPoint p21 = PSType1->pos();
  layout5_1->add(PSType);
  delete(PSType1);
  PSType->insertItem(DefCodec->toUnicode("INPAS PULSAR System"));
  PSType->insertItem(DefCodec->toUnicode("INPAS SMARTSALE"));
  //-----------Mirabell 19/12/11
  PSType->insertItem(DefCodec->toUnicode("СБЕРБАНК"));
  //----------------------------------
  PSType->move(p21);
	
  PSTypeExchange = new DedicationComboBox(PSTypeExchange1->parentWidget(),"r");
  QPoint p22 = PSTypeExchange1->pos();
  layout5_2->add(PSTypeExchange);
  delete(PSTypeExchange1);
  PSTypeExchange->move(p22);
  PSTypeExchange->insertItem(DefCodec->toUnicode("Файловый"));
  PSTypeExchange->insertItem(DefCodec->toUnicode("TCP/IP"));
	 
  PSPortVerifone = new DedicationComboBox(PSPortVerifone1->parentWidget(),"r");
  QPoint p23 = PSPortVerifone1->pos();
  layout5_12->add(PSPortVerifone);
  delete(PSPortVerifone1);
  PSPortVerifone->move(p23);
	
  PSBaudRateVerifone = new DedicationComboBox(PSBaudRateVerifone1->parentWidget(),"r");
  QPoint p24 = PSBaudRateVerifone1->pos();
  layout5_14->add(PSBaudRateVerifone);
  delete(PSBaudRateVerifone1);
  PSBaudRateVerifone->move(p24);
  PSBaudRateVerifone->insertItem("2400");
  PSBaudRateVerifone->insertItem("4800");
  PSBaudRateVerifone->insertItem("9600");
  PSBaudRateVerifone->insertItem("19200");
  PSBaudRateVerifone->insertItem("38400");
  PSBaudRateVerifone->insertItem("57600");
  PSBaudRateVerifone->insertItem("115200"); 
	
  PSTypeScheme = new DedicationComboBox(PSTypeScheme1->parentWidget(),"r");
  QPoint p27 = PSTypeScheme1->pos();
  layout5_22->add(PSTypeScheme);
  delete(PSTypeScheme1);
  PSTypeScheme->move(p27);
  PSTypeScheme->insertItem(DefCodec->toUnicode("с пин-падом"));
  PSTypeScheme->insertItem(DefCodec->toUnicode("с POS-терминалом"));
	
	
    //----------------------------------------------Общие--------------------------------------------------//  
    DebugLevelComboBox =  new DedicationComboBox( DebugLevelComboBox_21->parentWidget(), "r");
    QPoint pos = DebugLevelComboBox_21->pos();
    layout7_1->add(DebugLevelComboBox);
    delete(DebugLevelComboBox_21);
    QString t = "Запись в ";
    t = t+LOG_WAY;
    DebugLevelComboBox->insertItem(DefCodec->toUnicode("Нет отладочных сообщений"));
    DebugLevelComboBox->insertItem(DefCodec->toUnicode(t));
    DebugLevelComboBox->insertItem(DefCodec->toUnicode("Вывод сообщений в консоль"));
    DebugLevelComboBox->move(pos);
}

//======================================================//
//установить порядок обхода элементов на форме
void Configurations::SetTabOrderafterCreateComboBox()
{
  setTabOrder(SchemeName,DefaultScheme);
  setTabOrder(DefaultScheme,tabWidget2);
  setTabOrder(tabWidget2,CDSPcheckBox);
  setTabOrder(CDSPcheckBox,toolBox1);
  setTabOrder(toolBox1,CDSPPortNum);
  setTabOrder(CDSPPortNum,CDSPBaudRate);
  setTabOrder(CDSPBaudRate,CDSPNumberProt);
  setTabOrder(CDSPNumberProt,NewPerecCreate);
  setTabOrder(NewPerecCreate,CDSPPerec);
  setTabOrder(CDSPPerec,CDSPBrightness);
  setTabOrder(CDSPBrightness,CDSPScrollSpeed);
  setTabOrder(CDSPScrollSpeed,CDSPLengthStr);
  setTabOrder(CDSPLengthStr,CDSP_UP);
  setTabOrder(CDSP_UP,CDSP_DOWN);
  setTabOrder(CDSP_DOWN,CDSP_UP_Align_comboBox);
  setTabOrder(CDSP_UP_Align_comboBox,CDSP_DOWN_Align_comboBox);
  setTabOrder(CDSP_DOWN_Align_comboBox,CDSP_UP_ScrollMode_checkBox);
  setTabOrder(CDSP_UP_ScrollMode_checkBox,CDSP_DOWN_ScrollMode_checkBox);
  setTabOrder(CDSP_DOWN_ScrollMode_checkBox,DisplayTestButton);
  setTabOrder(DisplayTestButton,BCScheckBox);
  setTabOrder(BCScheckBox,BCSPortNum);
  setTabOrder(BCSPortNum,BCSBaudRate);
  setTabOrder(BCSBaudRate,BCSPREF1);
  setTabOrder(BCSPREF1,BCSPREF2);
  setTabOrder(BCSPREF2,BCSSUF1);
  setTabOrder(BCSSUF1,BCSSUF2);
  //-----------Mirabell 16/12/11
  setTabOrder(BCSSUF2,BCSInterSymbolTimeout);
  setTabOrder(BCSInterSymbolTimeout,ClearPrefSuf_pushButton);
  //-------------------------------------
  setTabOrder(ClearPrefSuf_pushButton,ScannerTestButton);
  setTabOrder(ScannerTestButton,FRcheckBox);
  setTabOrder(FRcheckBox,FRTypeDevice);
  setTabOrder(FRTypeDevice, FRPortNum);
  setTabOrder(FRPortNum,FRBaudRate);
  setTabOrder(FRBaudRate,FRProtocol);
  setTabOrder(FRProtocol,UseAccessPassword);
  setTabOrder(UseAccessPassword,AccessPassword);
  setTabOrder(AccessPassword,AdminPassword);
  setTabOrder(AdminPassword,FRTestButton);
  setTabOrder(FRTestButton,CurrentDocNum);
  setTabOrder(CurrentDocNum,SaveCurrentDocNumber);
  setTabOrder(SaveCurrentDocNumber,UseMoneyBox);
  setTabOrder(UseMoneyBox,FRMBoxCNL);
  setTabOrder(FRMBoxCNL,FRMBoxI1);
  setTabOrder(FRMBoxI1,FRMBoxI2);
  setTabOrder(FRMBoxI2,FRMBoxICnt);	
  setTabOrder(FRMBoxICnt,OpenMoneyBoxButton);
  setTabOrder(OpenMoneyBoxButton,MCRcheckBox);
  setTabOrder(MCRcheckBox,MCRPortNum);
  setTabOrder(MCRPortNum,MCRBaudRate);
  setTabOrder(MCRBaudRate,MCRPREF1);
  setTabOrder(MCRPREF1,MCRPREF2);
  setTabOrder(MCRPREF2,MCRSUF1);
  setTabOrder(MCRSUF1,MCRSUF2);
  setTabOrder(MCRSUF2,MCRTestButton);
  setTabOrder(MCRTestButton,PScheckBox);
  setTabOrder(PScheckBox,toolBox2);
  setTabOrder(toolBox2,PSType); 
  setTabOrder(PSType,PSTypeScheme);
  setTabOrder(PSTypeScheme,PSTypeExchange);
  setTabOrder(PSTypeExchange,PSTerminalId); 	  
  setTabOrder(PSTerminalId,PSTimeout1); 
  setTabOrder(PSTimeout1,PSTimeout2); 
  setTabOrder(PSTimeout2,PSTimeout3); 
  setTabOrder(PSTimeout3,Open_Dir);
  setTabOrder(Open_Dir,PSIPAddress);
  setTabOrder(PSIPAddress,PSIPPort);
  setTabOrder(PSIPPort,PSIPAddressVerifone); 
  setTabOrder(PSIPAddressVerifone,PSIPPortVerifone); 
  setTabOrder(PSPortVerifone,PSBaudRateVerifone);
  setTabOrder(PSBaudRateVerifone,CheckConnectWithPinPad); 
  setTabOrder(CheckConnectWithPinPad,Encryption0); 
  setTabOrder(Encryption0,Encryption1); 
  setTabOrder(Encryption1,PSPrintCanselCheck);
  setTabOrder(PSPrintCanselCheck,PSBank);
  setTabOrder(PSBank,PSTradeName);
  setTabOrder(PSTradeName,PSTownTrade);
  setTabOrder(PSTownTrade,PSAddress);	
  setTabOrder(PSAddress,PSCardMask);
  setTabOrder(PSCardMask,TestPSButton);
  setTabOrder(TestPSButton,EWcheckBox);
  setTabOrder(EWcheckBox,EWPortNum);
  setTabOrder(EWPortNum,EWBaudRate);
  setTabOrder(EWBaudRate,EWModel);
  setTabOrder(EWModel,EWTestButton);
  //-----------Mirabell 16/12/11
  setTabOrder(EWTestButton,EDScheckBox);
  setTabOrder(EDScheckBox,EDS_IpAddr);
  setTabOrder(EDS_IpAddr,EDS_IpPort);
  setTabOrder(EDS_IpPort,DebugLevelComboBox);
  //-------------------------------------
  setTabOrder(DebugLevelComboBox,InsertSerialNumProd);
  setTabOrder(InsertSerialNumProd,KodActivization_lineEdit);
  setTabOrder(KodActivization_lineEdit,InsertActivKod);
  setTabOrder(InsertActivKod,UpdateKodActivation);
  setTabOrder(UpdateKodActivation,NewButton);
  setTabOrder(NewButton,SaveButton);
  setTabOrder(SaveButton,DeleteButton);
  setTabOrder(DeleteButton,SchemeName);			
}

//======================================================//
//конструктор по умолчанию для заполнения формы
void Configurations::init()
{   
	 // puts("==================INIT=============");
	 
	 dl = new DebugLevelLog("CONFGUI",LOG_WAY,true); 
	 dl->SetMode(false);	 	// печатать время или не печатать
	 DebugLevel = LOGFILE; 
	 //DebugLevel = NODEBUG;	 // DEBUG - без лога LOGFILE - вести лог файл  STDOUT - вывод в консоль
	 dl->PrintString(DebugLevel,INFO,"void Configurations::init()\n");    
	 Usb = "USB";
	 UsbCom = "USB<->COM";
	 Com = "COM";
	 Acm = "ACM";
	 DefCodec = QTextCodec::codecForName(SYSCODEC);
	 CP1251Codec = QTextCodec::codecForName(HWCODEC);
	 delay_flag = false;
	 num_blink = 0;
	 conf_file_exist = true;
	 PS_on  = false; 
	 show_message = false;
	 char * m_str = (char *)calloc(17,sizeof(char));
	 strcpy(m_str,"[ *]{0,16}");
	 QRegExp m_rx(m_str);
	 QValidator * CardMask = new QRegExpValidator(m_rx,this);
	 free(m_str);	 
	 PSCardMask->setValidator(CardMask);
	 
	 char* tmp_str = (char*) calloc(100, sizeof(char));
	 strcpy(tmp_str, "^[A-Za-zА-Яа-я]\[A-Za-zА-Яа-я0-9 _]{0,19}$");
	 QRegExp rx(DefCodec->toUnicode(tmp_str)); 
	 QValidator * validator = new QRegExpValidator(rx, this);
	 free(tmp_str);
	 SchemeName->lineEdit()->setValidator(validator);
    
	 vc = new QIntValidator(1,9999,CurrentDocNum);
	 CurrentDocNum->setValidator(vc);
	 //-----------Mirabell 16/12/11
	 BCSInterSymbolTimeout->setMinValue(10);
	 BCSInterSymbolTimeout->setMaxValue(1000);
	 EDS_IpPort->setMinValue(1);
	 EDS_IpPort->setMaxValue(65535);
	   //-------------------------------------
	 AddComboBoxes(); //добавить комбо-боксы, на которых видно позиционирование
	 SetTabOrderafterCreateComboBox();//установить порядок перемещения по ним по клавише tab
	 StartSt = false;
	 CaptionFlag = false;
	 initForm();//сделать компоненты формы неактивными
	 InsertComPort();//заполнить комбо-боксы портов номерами портов
	 CDSPPerec->clear();
	 Codes();//заполнить  комбо-бокс перекодировок из файл codes.tab если он существует
	 SchemeName->setFocus(); 
	 StartSt = false;
	 CaptionFlag = false;
	 QString vvv ="[A-aZ-z0-9]{0,4}";
	 QRegExp reggexp(vvv);
	 AccP = new QRegExpValidator(reggexp,AccessPassword);
	 AdmP = new QIntValidator(0,99999999,AdminPassword);
	 PrefSuf = new QIntValidator(0,126,this);
	 FRMBoxImp = new QIntValidator(1,127,this);
	 FRMBoxIcnt  = new QIntValidator(1,255,this);
	 PsTime = new QIntValidator(1,500,PSTimeout1);
	 PsTermId = new QIntValidator(1,atoi(max_terminal_id),PSTerminalId);
	 PsIpPort = new QIntValidator(1,9999,PSIPPort);
	 PsIpPort = new QIntValidator(1,9999,PSIPPort);
	 AccessPassword -> setValidator(AccP);
	 AdmP -> setRange (0, 99999999);
	 AdminPassword -> setValidator(AdmP);
	 PrefSuf -> setRange (0,126);
	 BCSPREF1 -> setValidator(PrefSuf); 
	 BCSPREF2 -> setValidator(PrefSuf); 
	 BCSSUF1 -> setValidator(PrefSuf);
	 BCSSUF2 -> setValidator(PrefSuf); 
	 MCRPREF1 -> setValidator(PrefSuf); 
	 MCRPREF2 -> setValidator(PrefSuf); 
	 MCRSUF1 -> setValidator(PrefSuf);
	 MCRSUF2 -> setValidator(PrefSuf); 
	 FRMBoxImp->setRange(1,127);
	 FRMBoxI1->setValidator(FRMBoxImp);
	 FRMBoxI2->setValidator(FRMBoxImp);
	 FRMBoxIcnt ->setRange(1,255);
	 FRMBoxICnt->setValidator(FRMBoxIcnt);
	 PsTime -> setRange (0,500);
	 PSTimeout1->setValidator(PsTime);
	 PSTimeout2->setValidator(PsTime);
	 PSTimeout3->setValidator(PsTime);
	 PsTermId -> setRange (0,atoi(max_terminal_id));
	 PSTerminalId->setValidator(PsTermId);
	 PsIpPort ->setRange(0,9999);
	 PSIPPortVerifone->setValidator(PsIpPort);
	 char* tmp = (char*) calloc(100, sizeof(char));
	 strcpy(tmp, "[A-Za-zА-Яа-я0-9 _,.-()+='*@#$%&<>~`;:!?\"]{0,50}$");
	 QRegExp prstr(DefCodec->toUnicode(tmp));
	 QValidator * val_for_printing_strings = new QRegExpValidator(prstr, this);
	 free(tmp); 
	 PSBank->setValidator(val_for_printing_strings);
	 PSTradeName->setValidator(val_for_printing_strings);
	 PSTownTrade->setValidator(val_for_printing_strings);
	 PSAddress->setValidator(val_for_printing_strings);
	 
	 //привязываем события к комбо-боксам
	 QObject::connect (SchemeName, SIGNAL(activated(int)),this, SLOT(SchemeNameChanged()));
	 QObject::connect (BCSPortNum, SIGNAL(activated(int)),this, SLOT(BCSPortNumChanged()));
	 QObject::connect (MCRPortNum, SIGNAL(activated(int)),this, SLOT(MCRPortNumChanged()));
	 QObject::connect (FRTypeDevice, SIGNAL(activated(int)),this, SLOT(FRTypeDeviceproperties()));
	 QObject::connect (FRProtocol, SIGNAL(activated(int)),this, SLOT(FRModelChanged()));
	 QObject::connect (FRProtocol, SIGNAL(activated(int)),this, SLOT(UseMoneyBoxMercurii()));
	 QObject::connect (PSTypeExchange, SIGNAL(activated(int)),this, SLOT(PSModelChanged()));
	 QObject::connect (FRMBoxCNL, SIGNAL(activated(int)),this, SLOT(FRMBoxCNLChanged())); 
	 QObject::connect (FRProtocol, SIGNAL(activated(int)),this, SLOT(FRMBoxCNLChanged()));
	 QObject::connect (EWModel, SIGNAL(activated(int)),this, SLOT(EWModelCh()));
	 QObject::connect (PSType, SIGNAL(activated(int)),this, SLOT(PSTypeChanged()));
	 QObject::connect (PSTypeScheme, SIGNAL(activated(int)),this, SLOT(PSTypeChanged()));
	 
	 //--------------------ParamChanged()--------------
	 QObject::connect (BCSPortNum, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (BCSBaudRate, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPPortNum, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPBaudRate, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPNumberProt, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPPerec, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPBrightness, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPScrollSpeed, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPLengthStr, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSP_UP_Align_comboBox, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSP_DOWN_Align_comboBox, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (FRPortNum, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (FRBaudRate, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (FRProtocol, SIGNAL(activated(int)),this, SLOT(ParamChanged()));	 
	 QObject::connect (FRMBoxCNL, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 
	 QObject::connect (MCRPortNum, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (MCRBaudRate, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 
	 QObject::connect (EWPortNum, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (EWBaudRate, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (EWModel, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 
	 QObject::connect (PSType, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (PSTypeExchange, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (PSPortVerifone, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (PSBaudRateVerifone, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (PSTypeScheme, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 QObject::connect (Open_Dir, SIGNAL(clicked()),this, SLOT(ParamChanged()));
	 QObject::connect (DebugLevelComboBox, SIGNAL(activated(int)),this, SLOT(ParamChanged()));
	 //--------------------ParamChanged()--------------
	 
	  //привязываем события к кнопкам
	 QObject::connect (NewButton, SIGNAL(clicked()),this, SLOT(New()));
	 QObject::connect (SaveButton, SIGNAL(clicked()),this, SLOT(Save()));
	 QObject::connect (DeleteButton, SIGNAL(clicked()),this, SLOT(Delete()));
	 QObject::connect (CheckConnectWithPinPad, SIGNAL(clicked()),this, SLOT(CheckConnect()));
	 QObject::connect (DisplayTestButton, SIGNAL(clicked()),this, SLOT(EditDisplayTest()));
	 QObject::connect (NewPerecCreate, SIGNAL(clicked()),this, SLOT(NewPerecCreate_clicked()));
	 QObject::connect (ClearPrefSuf_pushButton, SIGNAL(clicked()),this, SLOT(ClearPrefSuf_pushButton_clicked()));		
	 QObject::connect (ScannerTestButton, SIGNAL(clicked()),this, SLOT(EditScannerTest()));		
	 QObject::connect (FRTestButton, SIGNAL(clicked()),this, SLOT(EditFRTest()));		
	 QObject::connect (SaveCurrentDocNumber, SIGNAL(clicked()),this, SLOT(SaveCurrentDoc()));		
	 QObject::connect (OpenMoneyBoxButton, SIGNAL(clicked()),this, SLOT(OpenMoneyBoxButton_clicked()));		
	 QObject::connect (MCRTestButton, SIGNAL(clicked()),this, SLOT(EditMCRTest()));		
	 QObject::connect (EWTestButton, SIGNAL(clicked()),this, SLOT(EditEWTest()));		
	 QObject::connect (Open_Dir, SIGNAL(clicked()),this, SLOT(Open_Dir_clicked()));		
	 QObject::connect (TestPSButton, SIGNAL(clicked()),this, SLOT(TestPS()));		
	 QObject::connect (InsertSerialNumProd, SIGNAL(clicked()),this, SLOT(InsertSerialNumProd_clicked()));		
	 QObject::connect (InsertActivKod, SIGNAL(clicked()),this, SLOT(InsertActivKod_clicked()));		
	 QObject::connect (UpdateKodActivation, SIGNAL(clicked()),this, SLOT(UpdateKodActivation_clicked()));
	 
	 //привязываем события к checkBox
	 
	 QObject::connect (DefaultScheme, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPcheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));
	 QObject::connect (CDSPcheckBox, SIGNAL(toggled(bool)),this, SLOT(CDSPproperties(bool)));	
	 QObject::connect (CDSP_UP_ScrollMode_checkBox, SIGNAL(toggled(bool)),this, SLOT(CheckLengthStr()));	
	 QObject::connect (CDSP_UP_ScrollMode_checkBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 QObject::connect (CDSP_DOWN_ScrollMode_checkBox, SIGNAL(toggled(bool)),this, SLOT(CheckLengthStr()));	
	 QObject::connect (CDSP_DOWN_ScrollMode_checkBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 
	 QObject::connect (BCScheckBox, SIGNAL(toggled(bool)),this, SLOT(BCSproperties(bool)));	
	 QObject::connect (BCScheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 
	 QObject::connect (FRcheckBox, SIGNAL(toggled(bool)),this, SLOT(FRproperties(bool)));	
	 QObject::connect (FRcheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 QObject::connect (UseAccessPassword, SIGNAL(toggled(bool)),this, SLOT(Passwordproperties(bool)));	 
	 QObject::connect (UseAccessPassword, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 QObject::connect (UseMoneyBox, SIGNAL(toggled(bool)),this, SLOT(UseMoneyBoxMercurii()));	
	 QObject::connect (UseMoneyBox, SIGNAL(toggled(bool)),this, SLOT(FRMBoxCNLChanged()));	
	 QObject::connect (UseMoneyBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));
	 
	 QObject::connect (MCRcheckBox, SIGNAL(toggled(bool)),this, SLOT(MCRproperties(bool)));	
	 QObject::connect (MCRcheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));
	 
	 QObject::connect (PScheckBox, SIGNAL(toggled(bool)),this, SLOT(PSproperties(bool)));	
	 QObject::connect (PScheckBox, SIGNAL(toggled(bool)),this, SLOT(CheckRights(bool)));	
	 QObject::connect (PScheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 QObject::connect (PSPrintCanselCheck, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 
	 QObject::connect (EWcheckBox, SIGNAL(toggled(bool)),this, SLOT(EWproperties(bool)));	
	 QObject::connect (EWcheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));
	 
	 //-----(+)-----Zuskin-----2011-06-24-----
	 QObject::connect (EDScheckBox, SIGNAL(toggled(bool)),this, SLOT(EDSproperties(bool)));	
 	 QObject::connect (EDScheckBox, SIGNAL(toggled(bool)),this, SLOT(ParamChanged()));	
	 //-------------------------------------------------
	 Encryption0->setChecked(TRUE);
	 if (MakeSchemeList() == -1) //заполнить список существующих схем из файла и загрузить активную
	 {
	      exit(-1);
	 }
	 if (BCSPortNum->currentItem() == 0)
	 {
	      BCSBaudRatetextLabel->setEnabled(FALSE);
                      BCSBaudRate->setEnabled(FALSE);	  	  
	 }

	 if (MCRPortNum->currentItem() == 0)
	 {
		  MCRBaudRatetextLabel->setEnabled(FALSE);
		  MCRBaudRate->setEnabled(FALSE);
	 }
	 change = false;  
	 changeflag = false;  
	 Manage();
	 
	 QCursor qc;
	 qc.setShape(Qt::ArrowCursor);
	 DefaultScheme->setCursor(qc);
	 FRcheckBox->setCursor(qc);
	 CDSPcheckBox->setCursor(qc);
	 BCScheckBox->setCursor(qc);
	 MCRcheckBox->setCursor(qc);
	 EWcheckBox->setCursor(qc);
	 PScheckBox->setCursor(qc); 
	 UseMoneyBox->setCursor(qc); 
	 if(hw->ECR_GetPortNumber() == 0)
		  hw->ECR_CreateFileWorkWithoutFR();
	 FlagInLoading = false; 
	 CheckOnUsb();
	 show_message = true;
	 
}


//======================================================//
//при закрытии формы
void Configurations::closeEvent( QCloseEvent* e )
{
    dl->PrintString(DebugLevel,INFO,"void Configurations::closeEvent( QCloseEvent* e )\n");
    if (changeflag == true)
    {
	int t =  QMessageBox::information( this, DefCodec->toUnicode("Cообщение"),
					   DefCodec->toUnicode("Некоторые параметры схемы были изменены.\nСохранить изменения?"),DefCodec->toUnicode("Да"),DefCodec->toUnicode("Нет"),0,0);
	if (t==0)
	{
	    Save();
	}      
	else
	{
	    changeflag = false;
	}   
    }
    if (SchemeName->isEnabled()==0)
    {
	Delete();
    }
    if (delay_flag) delete(qtimer);
   //delete(dkb_for_rus_symbol);
   delete(hw);
 //  puts("after delete(hw) in confGUI");
   e->accept();
   return;
}


//======================================================//
//при включении весов
void Configurations::EWproperties( bool Var )
{
  EWPortNum -> setEnabled(Var);
  EWBaudRate -> setEnabled(Var);
  EWModel -> setEnabled(Var);
  EWPortNumtextLabel -> setEnabled(Var);
  EWBaudRatetextLabel -> setEnabled(Var);
  EWModeltextLabel -> setEnabled(Var);
  EWTestButton -> setEnabled(Var);
  ModelDescr -> setEnabled(Var);
}


//======================================================//
//при включении дисплея
void Configurations::CDSPproperties( bool Var )
{ 
  CDSPPortNum -> setEnabled(Var);
  CDSPBaudRate -> setEnabled(Var);
  CDSPNumberProt -> setEnabled(Var);
  CDSPPortNumtextLabel -> setEnabled(Var);
  CDSPBaudRatetextLabel -> setEnabled(Var);
  CDSPNumberProttextLabel-> setEnabled(Var);  
  CDSPBrightness -> setEnabled(Var);  
  CDSPPerectextLabel->setEnabled(Var);
  CDSPScrollSpeed -> setEnabled(Var);  
  CDSPBrightnesstextLabel -> setEnabled(Var);  
  CDSPScrollSpeedtextLabel -> setEnabled(Var);  
  CDSPLengthStrtextLabel -> setEnabled(Var);
  CDSPLengthStr -> setEnabled(Var); 
  DisplayTestButton -> setEnabled(Var);  
  CDSPPerec -> setEnabled(Var); 
  NewPerecCreate->setEnabled(Var);
  CDSP_DOWN->setEnabled(Var);
  CDSP_UP->setEnabled(Var);
  CDSP_DOWN_textLabel->setEnabled(Var);
  CDSP_UP_textLabel->setEnabled(Var);
  CDSP_UP_Align_comboBox->setEnabled(Var);
  CDSP_UP_Align_textLabel->setEnabled(Var);
  CDSP_UP_ScrollMode_checkBox->setEnabled(Var);
  CDSP_DOWN_Align_comboBox->setEnabled(Var);
  CDSP_DOWN_Align_textLabel->setEnabled(Var);
  CDSP_DOWN_ScrollMode_checkBox->setEnabled(Var);
}


//======================================================//
//при включении фискальных регистраторов
void Configurations::FRproperties( bool Var )
{
	// printf("\n\nFRproperties(%d)\n\n",Var);
	 if(Var){
		  AccessPassword ->show();
		  AccessPasswordtextLabel ->show();
		  FRTypeDevicetextLabel->show();
		  FRTypeDevice->show();
		  FRPortNum -> show();
		  FRBaudRate -> show(); 
		  UseMoneyBox ->show();
		  UseAccessPassword -> show();
		  AdminPassword -> show();
		  FRPortNumtextLabel ->show();
		  FRBaudRatetextLabel ->show();
		  FRProtocoltextLabel -> show();
		  FRProtocol -> show();
		  AdminPasswordtextLabel -> show();
		  FRTestButton -> show();
		  FRTesttextLabel ->show();
		  FRTestButton ->  setEnabled(TRUE);  
		  FRTesttextLabel -> setEnabled(TRUE); 
		  AccessPassword -> setEnabled(TRUE);  
		  AccessPasswordtextLabel -> setEnabled(TRUE);  
		  FRPortNum -> setEnabled(TRUE);  
		  FRBaudRate ->  setEnabled(TRUE);  
		  UseMoneyBox -> setEnabled(TRUE);  
		  UseAccessPassword ->  setEnabled(TRUE);  
		  AdminPassword ->  setEnabled(TRUE);  
		  FRPortNumtextLabel -> setEnabled(TRUE);  
		  FRBaudRatetextLabel -> setEnabled(TRUE);  
		  FRProtocoltextLabel -> setEnabled(TRUE);  
		  FRProtocol ->  setEnabled(TRUE);  
		  AdminPasswordtextLabel ->  setEnabled(TRUE);  
		  if (FRProtocol->isEnabled())
				FRModelChanged();
		  CurrentDocNum->hide();
		  CurrentDocNum_textLabel->hide();
		  SaveCurrentDocNumber->hide();
		  UseMoneyBoxMercurii(); 
    }
    else {
		  FRTypeDevicetextLabel->hide();
		  FRTypeDevice->hide();
		  AccessPassword -> hide();
		  AccessPasswordtextLabel -> hide();
		  FRPortNum -> hide();  
		  FRBaudRate -> hide();
		  UseMoneyBox -> setEnabled(FALSE);
		  UseAccessPassword -> hide();  
		  AdminPassword ->  hide(); 
		  FRPortNumtextLabel -> hide();
		  FRBaudRatetextLabel -> hide(); 
		  FRProtocoltextLabel -> hide();  
		  FRProtocol -> hide();
		  AdminPasswordtextLabel -> hide();
		  FRTestButton -> hide(); 
		  FRTesttextLabel -> hide();
		  CurrentDocNum->show();
		  CurrentDocNum_textLabel->show();
		  SaveCurrentDocNumber->show();
	 } 
}

//======================================================//
//при выборе типа устройства(принтера чеков или ФР)
void Configurations::FRTypeDeviceproperties()
{
	// puts("FRTypeDeviceproperties()");
	 if (FRcheckBox->isChecked()){
		  bool Var = FRTypeDevice->currentItem();
		  if (Var){
				UseAccessPassword->hide();
				AccessPassword ->hide();
				AccessPasswordtextLabel ->hide();
				AdminPasswordtextLabel ->hide();
				AdminPassword->hide();
				FRProtocol->clear();
				InsertProtocol(1);
		  }
		  else{
				UseAccessPassword->show();
				AccessPassword ->show();
				AccessPasswordtextLabel ->show();
				AdminPasswordtextLabel ->show();
				AdminPassword->show();
				FRProtocol->clear();
				InsertProtocol(0);
				FRModelChanged();
		  
		  }
		  for (unsigned int i = 0; i<FR_Protocol.size();i++){
				FRProtocol->insertItem(FR_Protocol[i].nameProtocol);
		  }
	 }
}

//======================================================//
//при включении использования пароля фискальному регистатору
void Configurations::Passwordproperties( bool Var )
{
	 AccessPassword -> setEnabled(Var);  
	 AccessPasswordtextLabel -> setEnabled(Var);  
}

//======================================================//
//при включении сканера
void Configurations::BCSproperties( bool Var )
{
  BCSPortNum -> setEnabled(Var);
  BCSPortNumtextLabel -> setEnabled(Var);
  //-----------Mirabell 16/12/11
  int type = CheckBCSType();
  if (type == 2){
      BCSBaudRate -> setEnabled(FALSE);
      BCSBaudRatetextLabel -> setEnabled(FALSE);
  }
  else{
      BCSBaudRate -> setEnabled(Var);
      BCSBaudRatetextLabel -> setEnabled(Var);
  }
  if ((Var == TRUE) && ((type == 2) || (type == 3))){
      BCSInterSymbolTimeouttextLabel->setEnabled(TRUE);
      BCSInterSymbolTimeout->setEnabled(TRUE);
  }
  else{
      BCSInterSymbolTimeouttextLabel->setEnabled(FALSE);
      BCSInterSymbolTimeout->setEnabled(FALSE);
  }
  //--------------------------------------

  BCSPREF1 -> setEnabled(Var);
  BCSPREF2 -> setEnabled(Var); 
  BCSPref2textLabel -> setEnabled(Var);
  BCSSUF1 -> setEnabled(Var);
  BCSSUF2 -> setEnabled(Var);
  BCSSuf2textLabel -> setEnabled(Var);
  BCSPref1textLabel -> setEnabled(Var);
  BCSSuf1textLabel -> setEnabled(Var);
  ScannerTestButton -> setEnabled(Var);
  ClearPrefSuf_pushButton->setEnabled(Var);
}

//======================================================//
//при включении ридера
void Configurations::MCRproperties( bool Var )
{
    MCRPortNum -> setEnabled(Var);
    MCRPortNumtextLabel -> setEnabled(Var);
    int type = CheckMCRType();
    if (type != 2)
    {
	MCRBaudRate -> setEnabled(Var);
	MCRBaudRatetextLabel -> setEnabled(Var); 
	MCRPREF1 -> setEnabled(Var);  
	MCRPREF2 -> setEnabled(Var);  
	MCRSUF1 -> setEnabled(Var);  
	MCRSUF2 -> setEnabled(Var);  
	MCRPref1textLabel -> setEnabled(Var);  
	MCRPref2textLabel -> setEnabled(Var);  
	MCRSuf1textLabel -> setEnabled(Var);  
	MCRSuf2textLabel -> setEnabled(Var);
    }
    else 
    {
	MCRBaudRate -> setEnabled(FALSE);
	MCRBaudRatetextLabel -> setEnabled(FALSE);  
	MCRPREF1 -> setEnabled(FALSE);  
	MCRPREF2 -> setEnabled(FALSE);  
	MCRSUF1 -> setEnabled(FALSE);  
	MCRSUF2 -> setEnabled(FALSE);  
	MCRPref1textLabel -> setEnabled(FALSE);  
	MCRPref2textLabel -> setEnabled(FALSE);  
	MCRSuf1textLabel -> setEnabled(FALSE);  
	MCRSuf2textLabel -> setEnabled(FALSE);  
	
    }
 MCRTestButton -> setEnabled(Var);
 if (MCRcheckBox->isChecked() == TRUE)
    MCRPortNumChanged();
}


//======================================================//
//при включении платежной системы
void Configurations::PSproperties( bool Var)
{
	 //printf("PSproperties(%d)\n",Var);
	 PSType_textLabel->setEnabled(Var);
	 PSType->setEnabled(Var);
	 PSTypeScheme->setEnabled(Var);
	 PSTypeSchemetextLabel->setEnabled(Var);
	 PSTypeExchange_textLabel->setEnabled(Var);
	 PSTypeExchange->setEnabled(Var);
	 PSTerminalId_textLabel->setEnabled(Var);
	 PSTerminalId->setEnabled(Var);
	 PSTimeout1_textLabel->setEnabled(Var);
	 PSTimeout1->setEnabled(Var);  
	 PSTimeout2_textLabel->setEnabled(Var);
	 PSTimeout2->setEnabled(Var);
	 PSTimeout3_textLabel->setEnabled(Var);
	 PSTimeout3->setEnabled(Var);
	 PSNameDir_textLabel->setEnabled(Var);
	 PSNameDir->setEnabled(Var);
	 Open_Dir->setEnabled(Var);
	 PSIPAddress_textLabel->setEnabled(Var);
	 PSIPAddress->setEnabled(Var);
	 PSIPPort_textLabel->setEnabled(Var);
	 PSIPPort->setEnabled(Var);
	 PSIPAddressVerifone_textLabel->setEnabled(Var);
	 PSIPAddressVerifone->setEnabled(Var);
	 PSIPPortVerifone_textLabel->setEnabled(Var);
	 PSIPPortVerifone->setEnabled(Var);
	 PSPrintCanselCheck->setEnabled(Var);
	 PSBank_textLabel->setEnabled(Var);
	 PSBank->setEnabled(Var);
	 PSTradeName_textLabel->setEnabled(Var);
	 PSTradeName->setEnabled(Var);
	 PSTownTrade_textLabel->setEnabled(Var);
	 PSTownTrade->setEnabled(Var);
	 PSAddress_textLabel->setEnabled(Var);
	 PSAddress->setEnabled(Var);
	 TestPSButton->setEnabled(Var);
	 PSMessage->setEnabled(Var);
	 PSPortVerifone_textLabel->setEnabled(Var);
	 PSPortVerifone->setEnabled(Var);
	 PSBaudRateVerifone_textLabel->setEnabled(Var);
	 PSBaudRateVerifone->setEnabled(Var);
	 CheckConnectWithPinPad->setEnabled(Var);
	 PSCardMask_textLabel->setEnabled(Var);
	 PSCardMask->setEnabled(Var);
	 Encryption0->setEnabled(Var);
	 Encryption1->setEnabled(Var);
	 if (Var == false){
		  Message_CheckConnection->setText(DefCodec->toUnicode("<font color=black>Если пин-пад/POS-терминал загорелся - значит связь установлена!</font>"));
	 }
	 else{
		  Message_CheckConnection->setText(DefCodec->toUnicode("<font color=blue>Если пин-пад/POS-терминал загорелся - значит связь установлена!</font>"));
	 }
}


//======================================================//
//заполнение формы значениями по умолчанию
void Configurations::initForm(void)
{
  BCSPortNum -> setEnabled(FALSE);
  BCSBaudRate -> setEnabled(FALSE);
  BCSPortNumtextLabel -> setEnabled(FALSE);
  BCSBaudRatetextLabel -> setEnabled(FALSE);
  //-----------Mirabell 16/12/11
  BCSInterSymbolTimeouttextLabel->setEnabled(FALSE);
  BCSInterSymbolTimeout->setEnabled(FALSE);
  //----------------------------------------
  BCSPref1textLabel -> setEnabled(FALSE);
  BCSPref2textLabel -> setEnabled(FALSE);
  BCSSuf1textLabel -> setEnabled(FALSE);
  BCSSuf2textLabel -> setEnabled(FALSE);
  BCSPREF1 -> setEnabled(FALSE);
  BCSPREF2 -> setEnabled(FALSE);
  BCSSUF1 -> setEnabled(FALSE);
  BCSSUF2 -> setEnabled(FALSE);
  ClearPrefSuf_pushButton-> setEnabled(FALSE);
  ScannerTestButton -> setEnabled(FALSE);
  EWPortNum -> setEnabled(FALSE);
  EWBaudRate -> setEnabled(FALSE);
  EWModel -> setEnabled(FALSE);
  EWPortNumtextLabel -> setEnabled(FALSE);
  EWBaudRatetextLabel -> setEnabled(FALSE);
  EWModeltextLabel -> setEnabled(FALSE);
  EWTestButton -> setEnabled(FALSE);
  ModelDescr -> setEnabled(FALSE);
  NewPerecCreate->setEnabled(FALSE);
  CDSPPortNum -> setEnabled(FALSE);
  CDSPBaudRate -> setEnabled(FALSE);
  CDSPNumberProt -> setEnabled(FALSE);
  CDSPPerec -> setEnabled(FALSE);
  CDSPPerectextLabel ->setEnabled(FALSE);
  CDSPPortNumtextLabel -> setEnabled(FALSE);
  CDSPBaudRatetextLabel -> setEnabled(FALSE);
  CDSPNumberProttextLabel -> setEnabled(FALSE);
  CDSPBrightness -> setEnabled(FALSE);
  CDSPScrollSpeed -> setEnabled(FALSE);
  CDSPLengthStrtextLabel -> setEnabled(FALSE);
  CDSPLengthStr -> setEnabled(FALSE);
  CDSPBrightnesstextLabel -> setEnabled(FALSE);
  CDSPScrollSpeedtextLabel -> setEnabled(FALSE);
  CDSP_DOWN->setEnabled(FALSE);
  CDSP_UP->setEnabled(FALSE);
  CDSP_DOWN_textLabel->setEnabled(FALSE);
  CDSP_UP_textLabel->setEnabled(FALSE);
  CDSP_UP_Align_comboBox->setEnabled(FALSE);
  CDSP_UP_Align_textLabel->setEnabled(FALSE);
  CDSP_UP_ScrollMode_checkBox->setEnabled(FALSE);
  CDSP_DOWN_Align_comboBox->setEnabled(FALSE);
  CDSP_DOWN_Align_textLabel->setEnabled(FALSE);
  CDSP_DOWN_ScrollMode_checkBox->setEnabled(FALSE);
  DisplayTestButton -> setEnabled(FALSE);
  FRTypeDevicetextLabel->hide();
  FRTypeDevice->hide();
  FRPortNum -> hide();
  FRBaudRate -> hide();
  FRProtocoltextLabel -> hide();
  FRProtocol -> hide();
  UseMoneyBox -> setEnabled(FALSE);
  UseAccessPassword -> hide();
  AccessPassword -> hide();
  AdminPassword -> hide();
  FRPortNumtextLabel -> hide();
  FRBaudRatetextLabel -> hide();
  AccessPasswordtextLabel -> hide();
  AdminPasswordtextLabel -> hide();
  FRTestButton -> hide();     
  CurrentDocNum->show();
  CurrentDocNum_textLabel->show();
  int doc_num = (int)hw->ReadParam("CHECKNUMBER");
  QString s;
  CurrentDocNum->setText(s.setNum(doc_num));
  SaveCurrentDocNumber->show();
  FRTesttextLabel -> hide();
  MCRPortNum->setEnabled(FALSE);
  MCRBaudRate->setEnabled(FALSE);
  MCRBaudRatetextLabel->setEnabled(FALSE);
  MCRPortNumtextLabel->setEnabled(FALSE);
  MCRPREF1 -> setEnabled(FALSE);
  MCRPREF2 -> setEnabled(FALSE);
  MCRSUF1 -> setEnabled(FALSE);
  MCRSUF2 -> setEnabled(FALSE);
  MCRPref1textLabel -> setEnabled(FALSE);
  MCRPref2textLabel -> setEnabled(FALSE);
  MCRSuf1textLabel -> setEnabled(FALSE);
  MCRSuf2textLabel -> setEnabled(FALSE);
  MCRTestButton -> setEnabled(FALSE);
  PSproperties(FALSE);
  UseMoneyBoxMercurii();
  UpdateKodActivation->hide();
}

//======================================================//
//при изменении имени схемы
void Configurations::SchemeNameChanged()
{  
	 dl->PrintString(DebugLevel,INFO,"void Configurations::SchemeNameChanged()\n");
	 if (change == true)
	 {
		  if (SaveFlag == false) 
		  {  
				int t =  QMessageBox::information( this, DefCodec->toUnicode("Cообщение"), DefCodec->toUnicode("В схеме были произведены изменения\nсохранить схему с изменениями?"),DefCodec->toUnicode("сохранить"),DefCodec->toUnicode("отмена"),0,0);
				if (t == 1)
					 change = false;
				else
					 Save();  
		  }
		  else
				SaveFlag = false;
	 }
	 int SchemeNum = SchemeName -> currentItem()+1;
	 dl->PrintString(DebugLevel,INFO,"Выбрана схема: %d\n",SchemeNum);
	 FRTesttextLabel->setText(""); 
	 LoadScheme(SchemeNum);
	 OldNameLength = SchemeName -> currentText().length(); 
	 change = false;
}

//======================================================//
//создать строку, ктр бы выравнивала наименование параметра и его начало на опред длину для красивой записи в лог
QString Configurations::AlignParam(QString str1, QString str2)
{
	 int MinColSymbols = 60;
	 QString s = "";
	 s = str1;
	 int length2 = str2.length()+1;
	 while(MinColSymbols-length2-str1.length()!=0){
		  s = s+"-";
		  length2++;
	 }
      s = s+str2;
	 s = s+"\n";	
	 return s;
}


//======================================================//
//заполнить элементы формы значениями из конфигурационного файла
void Configurations::LoadScheme( int SchemeNum )
{  
   dl->PrintString(DebugLevel,INFO,"void Configurations::LoadScheme(%d)\n",SchemeNum); 
   FILE * fp = OpenFile(CONF_WAY);
   if (fp == 0){ 
       dl->PrintString(DebugLevel,INFO,"CAN'T LOAD SCHEME PARAM - NO ACCESS TO FILE %s\n",CONF_WAY);
       return;
   }
   FlagInLoading = true;
   int n; 
   int  * Param = (int *) calloc (1,sizeof(int));
   char * StrParam = (char*) calloc(501,sizeof(char));
   if (SchemeNum == DSN)
   {
      DefaultScheme -> setChecked(true);
      DefaultScheme -> setEnabled(false); 
   }
   else
   {
      DefaultScheme -> setChecked(false);
      DefaultScheme -> setEnabled(true);
   }

//----------------------BCS----------------------
   n = GetSchemeParam(SchemeNum,(char *)"BCSTYPE",Param, fp);
   switch (n)
   {
      case(1):
      { 
         dl->PrintString(DebugLevel, INFO, AlignParam("BCSTYPE","OK"));
         int TypeBCS = *Param;
         if (TypeBCS == 0)
         {
             BCScheckBox -> setChecked(FALSE);
         }
         else 
         {
            BCScheckBox -> setChecked(TRUE);
         }
	 
         n = GetSchemeParam(SchemeNum,(char *)"BCSLOGICALNUMBER",Param, fp);
         switch (n)
         {
            case 1: 
            {
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSLOGICALNUMBER","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSLOGICALNUMBER","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSLOGICALNUMBER","INCORRECT VALUE"));
               break;
            }
         }
				
				
         n = GetSchemeParam(SchemeNum,(char *)"BCSPORTNUM",Param, fp); 
         int PortNumBCS = *Param;
         switch (n)
         {	    	      
         case 1: 
            {
               if (PortNumBCS>0){
                  QString SearchingPortNum = "";
                  QString sss = "";
                  sss.setNum(PortNumBCS);
                  switch(TypeBCS){
                  case 1:{SearchingPortNum = Com+sss;break;}
                  case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                  case 3:{SearchingPortNum = UsbCom+sss;break;}
                  case 4:{SearchingPortNum = Acm+sss;break;}	
               }
               int kk = GetNumSearchingTextInComboBox(SearchingPortNum,2);
               if (kk== -1){ 	
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in BCSPortNum\n",SearchingPortNum.ascii());
                  BCScheckBox -> setChecked(FALSE);
                  break;
               }
               BCSPortNum -> setCurrentItem(kk);
            }
            dl->PrintString(DebugLevel,INFO,AlignParam("BCSPORTNUM","OK"));
            break;
         }
         case 0: 	
         {
            ErrorNum = 1;
            dl->PrintString(DebugLevel,INFO,AlignParam("BCSPORTNUM","NOT FOUND"));
            BCScheckBox -> setChecked(FALSE);
            break;
         }
         case -2:
         {
            ErrorNum = 1;
            dl->PrintString(DebugLevel,INFO,AlignParam("BCSPORTNUM","INCORRECT VALUE"));
            BCScheckBox -> setChecked(FALSE);
            break;
            } 
         }
            
         n = GetSchemeParam(SchemeNum,(char *)"BCSBAUDRATE",Param, fp);
         switch (n)
         {
            case 1: 
            {
               BCSBaudRate -> setCurrentItem(GetItemNum(*Param));
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSBAUDRATE","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSBAUDRATE","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSBAUDRATE","INCORRECT VALUE"));
               break;
            }
         }
	//-----------Mirabell 16/12/11 
         n = GetSchemeParam(SchemeNum,(char *)"BCSINTERSYMBOLTIMEOUT",Param, fp);
         switch (n)
         {
            case 1: 
            { 
               BCSInterSymbolTimeout->setValue(*Param);
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSINTERSYMBOLTIMEOUT","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               BCSInterSymbolTimeout->setValue(200);
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSINTERSYMBOLTIMEOUT","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               BCSInterSymbolTimeout->setValue(200);
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSINTERSYMBOLTIMEOUT","INCORRECT VALUE"));
               break;
            }
         }	 
	//--------------------------------- 
	 
         n = GetSchemeParam(SchemeNum,(char *)"BCSPREF1",Param, fp);
         switch (n)
         {
            case 1: 
            {
               BCSPREF1 -> setText(QString::number (*Param,10));
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF1","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF1","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF1","INCORRECT VALUE"));
               break;
            }
         }
	    
         n = GetSchemeParam(SchemeNum,(char *)"BCSPREF2",Param, fp);
         switch (n)
         {
            case 1: 
            {
               BCSPREF2 -> setText(QString::number (*Param,10));
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF2","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF2","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSPREF2","INCORRECT VALUE"));
               break;
            }
         }
	    
         n = GetSchemeParam(SchemeNum,(char *)"BCSSUF1",Param, fp);
         switch (n)
         {
            case 1: 
            {
               BCSSUF1 -> setText(QString::number (*Param,10));
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF1","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF1","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF1","INCORRECT VALUE"));
               break;
            }
         }
	    
         n = GetSchemeParam(SchemeNum,(char *)"BCSSUF2",Param, fp);
         switch (n)
         {
            case 1: 
            {
               BCSSUF2 -> setText(QString::number (*Param,10));
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF2","OK"));
               break;
            }
            case 0: 
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF2","NOT FOUND"));
               break;
            }
            case -2:
            {
               ErrorNum = 1;
               dl->PrintString(DebugLevel,INFO,AlignParam("BCSSUF2","INCORRECT VALUE"));
               break;
            }
         }
         break;
      }
      case  0: 
      {
         ErrorNum = 1;
         dl->PrintString(DebugLevel,INFO,AlignParam("BCSTYPE","NOT FOUND"));
         break;
      }
      case -2: 
      {
         ErrorNum = 1;
         dl->PrintString(DebugLevel,INFO,AlignParam("BCSTYPE","INCORRECT VALUE"));
         break;
         }  
      }
 
      //--------------EW------------------------
      n = GetSchemeParam(SchemeNum,(char *)"EWTYPE",Param, fp); 
      switch (n)
      {
             case 1:
            { 
                  dl->PrintString(DebugLevel,INFO,AlignParam("EWTYPE","OK"));
                  int TypeEW = *Param;
                  if (TypeEW == 0)
                        EWcheckBox -> setChecked(FALSE);       
                  else
                        EWcheckBox -> setChecked(TRUE);
				
                  n = GetSchemeParam(SchemeNum,(char *)"EWLOGICALNUMBER",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWLOGICALNUMBER","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWLOGICALNUMBER","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWLOGICALNUMBER","INCORRECT VALUE"));
                              break;
                        }
                  }
                  n = GetSchemeParam(SchemeNum,(char *)"EWPORTNUM",Param, fp); 
                  int PortNumEW = *Param;
                  switch (n)
                  {	    	      
                        case 1: 
                        {
                              if (PortNumEW>0){
                                    QString SearchingPortNum = "";
                                    QString sss = "";
                                    sss.setNum(PortNumEW);
                                    switch(TypeEW){
                                          case 1:{SearchingPortNum = Com+sss;break;}
                                          case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                          case 3:{SearchingPortNum = UsbCom+sss;break;}
                                          case 4:{SearchingPortNum = Acm+sss;break;}	
                                    }
                                    int kk = GetNumSearchingTextInComboBox(SearchingPortNum,5);
                                    if (kk== -1){ 	
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in EWPortNum\n",SearchingPortNum.ascii());	
                                          EWcheckBox -> setChecked(FALSE);   
                                          break;
                                    }
                                    EWPortNum -> setCurrentItem(kk);
                              }
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWPORTNUM","OK"));
                              break;
                        }
                        case 0: 	
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWPORTNUM","NOT FOUND"));
                              EWcheckBox -> setChecked(FALSE);   
                              break;
                        }
                        case -2:
                        {
                               ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWPORTNUM","INCORRECT VALUE"));
                              EWcheckBox -> setChecked(FALSE);   
                              break;
                        } 
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"EWBAUDRATE",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              EWBaudRate -> setCurrentItem (GetItemNum(*Param));
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWBAUDRATE","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWBAUDRATE","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWBAUDRATE","INCORRECT VALUE"));
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"EWMODEL",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              EWModel -> setCurrentItem (GetModel(*Param));
                              this->EWModelCh();
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWMODEL","OK"));
                              break;
                        }
                        case 0:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWMODEL","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("EWMODEL","INCORRECT VALUE"));
                              break;
                        }  
                  } 
                  break;
            }
            case  0: 
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("EWTYPE","NOT FOUND"));
                  break;
            }
            case -2: 
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("EWTYPE","INCORRECT VALUE"));
                  break;
            }  
      } 
      //---------------------CDSP-------------------------
      n = GetSchemeParam(SchemeNum,(char *)"CDSPTYPE",Param, fp); 
      switch (n)
      {
            case 1:
            { 
                  dl->PrintString(DebugLevel,INFO,AlignParam("CDSPTYPE","OK"));
                  int TypeCDSP = *Param;
                  if (TypeCDSP == 0)
                        CDSPcheckBox -> setChecked(FALSE);
                  else   
                        CDSPcheckBox -> setChecked(TRUE);
				
                  n = GetSchemeParam(SchemeNum,(char *)"CDSPLOGICALNUMBER",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLOGICALNUMBER","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLOGICALNUMBER","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLOGICALNUMBER","INCORRECT VALUE"));
                              break;
                        }
                  }
				
                  CheckOnUsb();
				
                  int idProd = 0;
                  int idVend = 0;

                  if (TypeCDSP == 5){
                        //USB DEVICE
                        n = GetSchemeParam(SchemeNum,(char *)"CDSPIDVENDOR",Param,fp); 
                        switch (n)
                        {
                              case 1: 	
                              {
                                    idVend = *Param;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDVENDOR","OK"));
                                    break;
                              }
                              case 0: 
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDVENDOR","NOT FOUND"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                              case -2:
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDVENDOR","INCORRECT VALUE"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                        }
						
                        n = GetSchemeParam(SchemeNum,(char *)"CDSPIDPRODUCT",Param,fp); 
                        switch (n)
                        {
                              case 1: 	
                              {
                                    idProd = *Param;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDPRODUCT","OK"));
                                    break;
                              }
                              case 0: 
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDPRODUCT","NOT FOUND"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                              case -2:
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPIDPRODUCT","INCORRECT VALUE"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                        }	
					 
                        string name_device = hw->GetProductUsbDevice(idProd,idVend).substr(0,20);
                        int pos_last_probel = 0;
                        pos_last_probel = name_device.find_last_of(" ");
                        if (pos_last_probel!=0){
                              name_device = name_device.substr(0,pos_last_probel);
                        }
					 
                        for (int k = 0; k<CDSPPortNum->count();k++){
                              string cur_item = CDSPPortNum->text(k);
                              int t = cur_item.find(name_device);
                              if (t!=-1)
                              {
                                    CDSPPortNum -> setCurrentItem(k);
                              }
                        }
                  }
                  else{
                        n = GetSchemeParam(SchemeNum,(char *)"CDSPPORTNUM",Param,fp); 
                        int PortNumCDSP = *Param;
                        switch (n)
                        {
                              case 1: 	
                              {
                                    if (PortNumCDSP>0){
                                          QString SearchingPortNum = "";
                                          QString sss = "";
                                          sss.setNum(PortNumCDSP);
                                          switch(TypeCDSP){
                                                case 1:{SearchingPortNum = Com+sss;break;}
                                                case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                case 3:{SearchingPortNum = UsbCom+sss;break;}
                                                case 4:{SearchingPortNum = Acm+sss;break;}	
                                          }
                                          int kk = GetNumSearchingTextInComboBox(SearchingPortNum,1);
                                          if (kk== -1){ 	
                                                ErrorNum = 1;
                                                dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in CDSPPortNum\n",SearchingPortNum.ascii());	
                                                CDSPcheckBox -> setChecked(FALSE);
                                                break;
                                          }
                                          CDSPPortNum -> setCurrentItem(kk);
                                    }
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPORTNUM","OK"));
                                    break;
                              }
                              case 0: 
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPORTNUM","NOT FOUND"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                              case -2:
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPORTNUM","INCORRECT VALUE"));
                                    CDSPcheckBox -> setChecked(FALSE);
                                    break;
                              }
                        }
					 
                        n = GetSchemeParam(SchemeNum,(char *)"CDSPBAUDRATE",Param, fp);
                        switch(n)
                        {
                              case 1: 
                              {
                                    CDSPBaudRate -> setCurrentItem (GetItemNum(*Param));
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBAUDRATE","OK"));
                                    break;
                              }
                              case 0: 
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBAUDRATE","NOT FOUND"));
                                    break;
                              }
                              case -2:
                              {
                                    ErrorNum = 1;
                                    dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBAUDRATE","INCORRECT VALUE"));
                                    break;
                              }
                        }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"CDSPNUMBERPROT",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              CDSPNumberProt -> setCurrentItem (*Param-1);
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPNUMBERPROT","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPNUMBERPROT","NOT FOUND"));
                              break;
                         }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPNUMBERPROT","INCORRECT VALUE"));
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"CDSPPERECODIROVKA",Param, fp); 
                  switch (n)
                  {
                        case 1: 	
                        {
                              CDSPPerec -> setCurrentItem(*Param-1);
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPERECODIROVKA","OK"));
                              break;
                         }
                        case 0:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPERECODIROVKA","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CDSPPERECODIROVKA","INCORRECT VALUE"));
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"CDSPBRIGHTNESS",Param, fp);
                 switch (n)
                {
                     case 1: 
                     {
                          CDSPBrightness -> setCurrentItem(*Param-1);
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBRIGHTNESS","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBRIGHTNESS","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPBRIGHTNESS","INCORRECT VALUE"));
                          break;
                     }
                }
	   
                n = GetSchemeParam(SchemeNum,(char *)"CDSPSCROLLSPEED",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                          CDSPScrollSpeed -> setCurrentItem(*Param-1);
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPSCROLLSPEED","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPSCROLLSPEED","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPSCROLLSPEED","INCORRECT VALUE"));
                          break;
                     }
                } 
				
                n = GetSchemeParam(SchemeNum,(char *)"CDSPLENGTHSTR",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                          CDSPLengthStr -> setCurrentItem(*Param-1);
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLENGTHSTR","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLENGTHSTR","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("CDSPLENGTHSTR","INCORRECT VALUE"));
                          break;
                     }
                } 
				
				
                strcpy(StrParam,"");
                n = GetStrSchemeParam(SchemeNum,(char *)"DEMONUPSTRING",StrParam, fp);
                switch (n)
                { 
                     case 1:
                     {
                          CDSP_UP -> setText(CP1251Codec->toUnicode(StrParam));
                          strcpy(StrParam,"");
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSTRING","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSTRING","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSTRING","INCORRECT VALUE"));
                          break;
                     }
                } 

                n = GetStrSchemeParam(SchemeNum,(char *)"DEMONDOWNSTRING",StrParam, fp);
                switch (n)
                {
                     case 1: 
                     {
                          CDSP_DOWN -> setText(CP1251Codec->toUnicode(StrParam));
                          strcpy(StrParam,"");
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSTRING","OK"));
                         break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSTRING","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSTRING","INCORRECT VALUE"));
                          break;
                     }
                }
	   
                n = GetSchemeParam(SchemeNum,(char *)"DEMONUPALIGN",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                          CDSP_UP_Align_comboBox -> setCurrentItem(*Param-1);
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPALIGN","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPALIGN","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPALIGN","INCORRECT VALUE"));
                          break;
                     }
                }
	   
                n = GetSchemeParam(SchemeNum,(char *)"DEMONDOWNALIGN",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                          CDSP_DOWN_Align_comboBox -> setCurrentItem(*Param-1);
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNALIGN","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNALIGN","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNALIGN","INCORRECT VALUE"));
                          break;
                     }
                }

                n = GetSchemeParam(SchemeNum,(char *)"DEMONUPSCROLLMODE",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                          if (*Param == 0) CDSP_UP_ScrollMode_checkBox -> setChecked(FALSE);
                          else  CDSP_UP_ScrollMode_checkBox -> setChecked(TRUE);	
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSCROLLMODE","OK"));
                          break;
                     }
                     case 0:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSCROLLMODE","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONUPSCROLLMODE","INCORRECT VALUE"));
                          break;
                     }
                }
	   
                n = GetSchemeParam(SchemeNum,(char *)"DEMONDOWNSCROLLMODE",Param, fp);
                switch (n)
                {
                     case 1: 
                     {
                           if (*Param == 0)  CDSP_DOWN_ScrollMode_checkBox -> setChecked(FALSE);
                          else  CDSP_DOWN_ScrollMode_checkBox -> setChecked(TRUE);
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSCROLLMODE","OK"));
                          break;
                     }
                     case 0: 
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSCROLLMODE","NOT FOUND"));
                          break;
                     }
                     case -2:
                     {
                          ErrorNum = 1;
                          dl->PrintString(DebugLevel,INFO,AlignParam("DEMONDOWNSCROLLMODE","INCORRECT VALUE"));                                break;
                     }
                }
                break;
             }
             case  0: 
             {	
                ErrorNum = 1;
                dl->PrintString(DebugLevel,INFO,AlignParam("CDSPTYPE","NOT FOUND"));
               break;
             }
             case -2: 
             {
                ErrorNum = 1;
                dl->PrintString(DebugLevel,INFO,AlignParam("CDSPTYPE","INCORRECT VALUE"));
                break;
             }  
      }
      //-----------------------------ECR-----------------------
      n = GetSchemeParam(SchemeNum,(char *)"ECRTYPE",Param, fp); 
      int TypeFR = *Param;
      switch (n)
      {
            case 1:
            { 
                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRTYPE","OK"));
                  if (TypeFR == 0){
                        FRcheckBox -> setChecked(FALSE);
                        // puts("FRcheckBox -> setChecked(FALSE);");
                  }
                  else{
	        FRcheckBox -> setChecked(TRUE);
	        // puts("FRcheckBox -> setChecked(TRUE);");
                  }
                  n = GetSchemeParam(SchemeNum,(char *)"ECRLOGICALNUMBER",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRLOGICALNUMBER","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRLOGICALNUMBER","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRLOGICALNUMBER","INCORRECT VALUE"));
                              break;
                        }
                  }
                  n = GetSchemeParam(SchemeNum,(char *)"ECRPORTNUM",Param, fp); 
                  int PortNumFR = *Param;
                  switch(n)
                  {
                        case 1:
                        {
                              if (PortNumFR>0){
                                    QString SearchingPortNum = "";
                                    QString sss = "";
                                    sss.setNum(PortNumFR);
                                    switch(TypeFR){
                                          case 1:{SearchingPortNum = Com+sss;break;}
                                          case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                          case 3:{SearchingPortNum = UsbCom+sss;break;}
                                          case 4:{SearchingPortNum = Acm+sss;break;}
                                          default:{break;}
                                    }
		   
                                    int kk = GetNumSearchingTextInComboBox(SearchingPortNum,3);
                                    if (kk == -1){
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in FRPortNum\n",SearchingPortNum.ascii());	
                                          FRcheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    FRPortNum -> setCurrentItem(kk);
                              }
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUM","OK"));
                              break;
                        } 
                        case 0: 
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUM","NOT FOUND"));
                              n = GetSchemeParam(SchemeNum,(char *)"ECRPORTNUMBER",Param, fp); 
                              int PortNumFR = *Param;
                              switch(n)
                              {
                                    case 1:
                                    {
                                          if (PortNumFR>0){
                                                QString SearchingPortNum = "";
                                                QString sss = "";
                                                sss.setNum(PortNumFR);
                                                switch(TypeFR){
                                                       case 1:{SearchingPortNum = Com+sss;break;}
                                                       case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                       case 3:{SearchingPortNum = UsbCom+sss;break;}
                                                       case 4:{SearchingPortNum = Acm+sss;break;}
                                                       default:{break;}
                                                }
		   
                                                int kk = GetNumSearchingTextInComboBox(SearchingPortNum,3);
                                                if (kk == -1){
                                                       ErrorNum = 1;
                                                       dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in FRPortNum\n",SearchingPortNum.ascii());	
                                                       FRcheckBox -> setChecked(FALSE);
                                                       break;
                                                }
                                                FRPortNum -> setCurrentItem(kk);
                                          }
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","OK"));
                                          break;
                                    } 
                                    case 0: 
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","NOT FOUND"));
                                          FRcheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    case -2:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","INCORRECT VALUE"));
                                          FRcheckBox -> setChecked(FALSE);
                                          break;
                                    }
                              }
                              break;
                        }
                        case -2:
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUM","INCORRECT VALUE"));
                              n = GetSchemeParam(SchemeNum,(char *)"ECRPORTNUMBER",Param, fp); 
                              int PortNumFR = *Param;
                              switch(n)
                              {
                                    case 1:
                                    {
                                          if (PortNumFR>0){
                                                QString SearchingPortNum = "";
                                                QString sss = "";
                                                sss.setNum(PortNumFR);
                                                switch(TypeFR){
                                                       case 1:{SearchingPortNum = Com+sss;break;}
                                                       case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                       case 3:{SearchingPortNum = UsbCom+sss;break;}
                                                       case 4:{SearchingPortNum = Acm+sss;break;}
                                                }
		   
                                                int kk = GetNumSearchingTextInComboBox(SearchingPortNum,3);
                                                if (kk == -1){
                                                       ErrorNum = 1;
                                                       dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in FRPortNum\n",SearchingPortNum.ascii());	
                                                       FRcheckBox -> setChecked(FALSE);
                                                       break;
                                                }
                                                FRPortNum -> setCurrentItem(kk);
                                          }
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","OK"));
                                          break;
                                    } 
                                    case 0: 
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","NOT FOUND"));
                                          FRcheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    case -2:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("ECRPORTNUMBER","INCORRECT VALUE"));
                                          FRcheckBox -> setChecked(FALSE);
                                          break;
                                    }
                              } 
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"ECRBAUDRATE",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              FRBaudRate -> setCurrentItem (GetItemNum(*Param));
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRBAUDRATE","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRBAUDRATE","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRBAUDRATE","INCORRECT VALUE"));
                              break;
                        }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"ECRTYPEDEVICE",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              int typedevice = 0;
                              if (!*Param)                              typedevice = 0;
                              else                              typedevice = 1;
                              FRTypeDevice -> setCurrentItem(typedevice);
                              FRTypeDeviceproperties();
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRTYPEDEVICE","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("CRTYPEDEVICE","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRTYPEDEVICE","INCORRECT VALUE"));
                              break;
                        }
                  }
				
				
				
	   
                  n = GetSchemeParam(SchemeNum,(char *)"ECRMODEL",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              //printf("*Param = %d\n",*Param);
                              for (unsigned int ii = 0; ii<FR_Protocol.size();ii++){
                                    int NumProtocol = FR_Protocol[ii].numProtocol;
                                    if ((*Param == NumProtocol) && (NumProtocol>0))
                                    {
                                           FRProtocol -> setCurrentItem(NumProtocol -1);
                                           //printf("NumProtocol = %d\n",NumProtocol);
                                    }
                              }
                              //FRProtocol -> setCurrentItem (GetModel(*Param));
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMODEL","OK"));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMODEL","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMODEL","INCORRECT VALUE"));
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"ECRUSEACCESSPASSWORD",Param, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRUSEACCESSPASSWORD","OK"));
                              if (*Param != 0)
                              {
                                    UseAccessPassword -> setChecked(true);
                                    AccessPasswordtextLabel->setEnabled(TRUE);
                                    AccessPassword->setEnabled(TRUE);
                                    n = GetStrSchemeParam(SchemeNum,(char *)"ECRACCESSPASSWORD",StrParam,fp);
                                    //printf("StrParam = %s\n",StrParam);
                                    //printf("n = %d\n",n);
                                    switch (n)
                                    {
                                           case 1: 
                                           {
                                                  AccessPassword -> setText(StrParam);
                                                  strcpy(StrParam,"");
                                                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRACCESSPASSWORD","OK"));
                                                  break;
                                           } 
                                           case 0:
                                           { 
                                                  ErrorNum = 1;
                                                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRACCESSPASSWORD","NOT FOUND"));
                                                  break;
                                           } 
                                           case -2:
                                           { 
                                                  ErrorNum = 1;
                                                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRACCESSPASSWORD","INCORRECT VALUE"));
                                                  break;
                                           } 
                                    }
                              }
                              else{
                                    UseAccessPassword -> setChecked(FALSE);
                                    AccessPasswordtextLabel->setEnabled(FALSE);
                                    AccessPassword->setEnabled(FALSE);
                              }
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRUSEACCESSPASSWORD","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRUSEACCESSPASSWORD","INCORRECT VALUE"));
                              break;
                        }
                  }
	   
                  n = GetSchemeParam(SchemeNum,(char *)"USEMONEYBOX",Param, fp);
                  switch (n)
                  {
                        case 1:  
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("USEMONEYBOX","OK"));
                              if (*Param == 0)    UseMoneyBox -> setChecked(FALSE);
                              else UseMoneyBox -> setChecked(TRUE);
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("USEMONEYBOX","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("USEMONEYBOX","INCORRECT VALUE"));
                              break;
                        } 
                  }
				
	 
				
                  n = GetSchemeParam(SchemeNum,(char *)"ECRMBOXCNL",Param, fp);
                  switch (n)
                  {
                        case 1:  
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXCNL","OK"));
                              FRMBoxCNL->setCurrentItem(*Param-1);
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXCNL","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXCNL","INCORRECT VALUE"));
                              break;
                        } 
                  }
				
				
                  n = GetSchemeParam(SchemeNum,(char *)"ECRMBOXI1",Param, fp);
                  switch (n)
                  {
                        case 1:  
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI1","OK"));
                              FRMBoxI1->setText(itoa(*Param));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI1","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI1","INCORRECT VALUE"));
                              break;
                        } 
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"ECRMBOXI2",Param, fp);
                  switch (n)
                  {
                        case 1:  
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI2","OK"));
                              FRMBoxI2->setText(itoa(*Param));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI2","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXI2","INCORRECT VALUE"));
                              break;
                        } 
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"ECRMBOXICNT",Param, fp);
                  switch (n)
                  {
                        case 1:  
                        {
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXICNT","OK"));
                              FRMBoxICnt->setText(itoa(*Param));
                              break;
                        }
                        case 0: 
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXICNT","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRMBOXICNT","INCORRECT VALUE"));
                              break;
                        } 
                  }
				
		       
                  n = GetStrSchemeParam(SchemeNum,(char *)"ECRPASSWORD",StrParam, fp);
                  switch (n)
                  {
                        case 1: 
                        {
                              AdminPassword -> setText(StrParam);
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPASSWORD","OK"));
                              break;
                        }
                        case 0:
                        {
                              ErrorNum = 1; 
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPASSWORD","NOT FOUND"));
                              break;
                        }
                        case -2:
                        {
                              ErrorNum = 1; 
                              dl->PrintString(DebugLevel,INFO,AlignParam("ECRPASSWORD","INCORRECT VALUE"));
                              break;
                        }
                  }  
                  break;
            }
            case  0: 
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRTYPE","NOT FOUND"));
                  break;
            }
            case -2: 
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("ECRTYPE","INCORRECT VALUE"));
                  break;
            }  
      } 
      //------------------MCR----------------------
      n = GetSchemeParam(SchemeNum,(char *)"MCRTYPE",Param, fp);
      int TypeMCR = *Param;
      switch(n)
      {
            case 1:
            {  
                  dl->PrintString(DebugLevel,INFO,AlignParam("MCRTYPE","OK"));
                  if (TypeMCR == 0)                  MCRcheckBox -> setChecked(FALSE);
                  else                  MCRcheckBox -> setChecked(TRUE);
                  n = GetSchemeParam(SchemeNum,(char *)"MCRLOGICALNUMBER",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRLOGICALNUMBER","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRLOGICALNUMBER","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRLOGICALNUMBER","INCORRECT VALUE"));
                                break;
                         }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"MCRPORTNUM",Param, fp); 
                  int PortNumMCR = *Param;
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                                if (PortNumMCR>0){
                                       QString SearchingPortNum = "";
                                       QString sss = "";
                                       sss.setNum(PortNumMCR);
                                       switch(TypeMCR){
                                              case 1:{SearchingPortNum = Com+sss;break;}
                                              case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                              case 3:{SearchingPortNum = UsbCom+sss;break;}
                                              case 4:{SearchingPortNum = Acm+sss;break;}	
                                       }
                                       int kk = GetNumSearchingTextInComboBox(SearchingPortNum,4);
                                       if (kk== -1){ 	
                                              ErrorNum = 1;
                                              dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in MCRPortNum\n",SearchingPortNum.ascii());		
                                              MCRcheckBox -> setChecked(FALSE);
                                              break;
                                       }
                                       MCRPortNum -> setCurrentItem(kk);
                                }
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUM","OK"));
                                break;
                         }
                         case 0:
                         {
                                //ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUM","NOT FOUND"));
                                //MCRcheckBox -> setChecked(FALSE);
                                n = GetSchemeParam(SchemeNum,(char *)"MCRPORTNUMBER",Param, fp); 
                                int PortNumMCR = *Param;
                                switch (n)
                                {	    	      
                                       case 1: 
                                       {
                                              if (PortNumMCR>0){
                                                     QString SearchingPortNum = "";
                                                     QString sss = "";
                                                     sss.setNum(PortNumMCR);
                                                     switch(TypeMCR){
                                                            case 1:{SearchingPortNum = Com+sss;break;}
                                                            case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                            case 3:{SearchingPortNum = UsbCom+sss;break;}
                                                            case 4:{SearchingPortNum = Acm+sss;break;}	
                                                     }
                                                     int kk = GetNumSearchingTextInComboBox(SearchingPortNum,4);
                                                     if (kk== -1){ 	
                                                            ErrorNum = 1;
                                                            dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in MCRPortNum\n",SearchingPortNum.ascii());		
                                                            MCRcheckBox -> setChecked(FALSE);
                                                            break;
                                                     }
                                                     MCRPortNum -> setCurrentItem(kk);
                                              }
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUMBER","OK"));
                                              break;
                                       }
                                       case 0:
                                       {
                                              ErrorNum = 1;
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUMBER","NOT FOUND"));
                                              MCRcheckBox -> setChecked(FALSE);
                                              break;
                                       }
                                       case -2:
                                       {
                                              ErrorNum = 1;
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUM","INCORRECT VALUE"));
                                              MCRcheckBox -> setChecked(FALSE);
                                              break;
                                       } 
                                }
                                break;
                         }
                         case -2:
                         {
					
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUM","INCORRECT VALUE"));
                                n = GetSchemeParam(SchemeNum,(char *)"MCRPORTNUMBER",Param, fp); 
                                int PortNumMCR = *Param;
                                switch (n)
                                {	    	      
                                       case 1: 
                                       {
                                              if (PortNumMCR>0){
                                                     QString SearchingPortNum = "";
                                                     QString sss = "";
                                                     sss.setNum(PortNumMCR);
                                                     switch(TypeMCR){
                                                            case 1:{SearchingPortNum = Com+sss;break;}
                                                            case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                            case 3:{SearchingPortNum = UsbCom+sss;break;}
                                                            case 4:{SearchingPortNum = Acm+sss;break;}	
                                                     }
                                                     int kk = GetNumSearchingTextInComboBox(SearchingPortNum,4);
                                                     if (kk== -1){ 	
                                                            ErrorNum = 1;
                                                            dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in MCRPortNum\n",SearchingPortNum.ascii());		
                                                            MCRcheckBox -> setChecked(FALSE);
                                                            break;
                                                     }
                                                     MCRPortNum -> setCurrentItem(kk);
                                              }
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUMBER","OK"));
                                              break;
                                       }
                                       case 0:
                                       {
                                              ErrorNum = 1;
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUMBER","NOT FOUND"));
                                              MCRcheckBox -> setChecked(FALSE);
                                              break;
                                       }
                                       case -2:
                                       {
                                              ErrorNum = 1;
                                              dl->PrintString(DebugLevel,INFO,AlignParam("MCRPORTNUM","INCORRECT VALUE"));
                                              MCRcheckBox -> setChecked(FALSE);
                                              break;
                                       } 
                                }
                                break;
                         } 
                  }
            
                  n = GetSchemeParam(SchemeNum,(char *)"MCRBAUDRATE",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                                MCRBaudRate -> setCurrentItem(GetItemNum(*Param));
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRBAUDRATE","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRBAUDRATE","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRBAUDRATE","INCORRECT VALUE"));
                                break;
                         }
                  }   
                  n = GetSchemeParam(SchemeNum,(char *)"MCRPREF1",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                                MCRPREF1 -> setText(QString::number (*Param,10));
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF1","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF1","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF1","INCORRECT VALUE"));
                                break;
                         }
                  }
           
                  n = GetSchemeParam(SchemeNum,(char *)"MCRPREF2",Param, fp);
                  switch (n) 
                  {
                         case 1: 
                         {
                                MCRPREF2 -> setText(QString::number (*Param,10));
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF2","OK"));
                                break;
                         }
                         case 0:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF2","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRPREF2","INCORRECT VALUE"));
                                break;
                         }
                  }	
            
                  n = GetSchemeParam(SchemeNum,(char *)"MCRSUF1",Param, fp);
                  switch (n)
                  { 
                         case 1: 
                         {
                                MCRSUF1 -> setText(QString::number (*Param,10));
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF1","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF1","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF1","INCORRECT VALUE"));
                                break;
                         }
                  }
			  
                  n = GetSchemeParam(SchemeNum,(char *)"MCRSUF2",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                                MCRSUF2 -> setText(QString::number (*Param,10));
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF2","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF2","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("MCRSUF2","INCORRECT VALUE"));
                                break;
                         }
                  }
                  break;
            }
            case 0:
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("MCRTYPE","NOT FOUND"));
                  break;
            }
            case -2:
              {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("MCRTYPE","INCORRECT VALUE"));
                  break;
            }
      } 	
	 
      //------------------PS----------------------
      n = GetSchemeParam(SchemeNum,(char *)"PSTYPE",Param, fp);
      int TypePS = *Param;
      switch(n)
      {
            case 1:
            {  
                  dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPE","OK"));
                  if (TypePS == 0){
                         PScheckBox -> setChecked(FALSE);
                         PS_on = false;
                  }
                  else{
                         PS_on = true;
                         PScheckBox -> setChecked(TRUE);
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSLOGICALNUMBER",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSLOGICALNUMBER","OK"));
                              break;
                         }
                         case 0: 
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSLOGICALNUMBER","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSLOGICALNUMBER","INCORRECT VALUE"));
                              break;
                         }
                  }
                  int TypeSystem = 0;
                  //вид платежной системы
                  n = GetSchemeParam(SchemeNum,(char *)"PSTYPESYSTEM",Param, fp);
                  switch(n)
                  {
                         case 1:
                         { 
                              TypeSystem = *Param;
                              PSType->setCurrentItem(TypeSystem);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESYSTEM","OK"));
                              break;
                         }
                         case 0:{
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESYSTEM","NOT FOUND"));
                              break;	
                         }
                         case -2:{
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESYSTEM","INCORRECT VALUE"));
                              break;					  
                         }
                  }
				
                  //схема платежной системы
                  n = GetSchemeParam(SchemeNum,(char *)"PSTYPESCHEME",Param, fp);
                  switch(n)
                  {
                         case 1:
                         { 
                              int TypeScheme = *Param;
                              PSTypeScheme->setCurrentItem(TypeScheme);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESCHEME","OK"));
                              break;
                         }
                         case 0:{
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESCHEME","NOT FOUND"));
                              break;	
                         }
                         case -2:{
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPESCHEME","INCORRECT VALUE"));
                              break;					  
                         }
                  }
	
								 
                  n = GetSchemeParam(SchemeNum,(char *)"PSTYPEEXCHANGE",Param, fp); 
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                              PSTypeExchange -> setCurrentItem(*Param);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPEEXCHANGE","OK"));
                              break;
                         }
                         case 0:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPEEXCHANGE","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPEEXCHANGE","INCORRECT VALUE"));
                              break;
                         } 
                  }
            
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSTERMINALID",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                              PSTerminalId-> setText(CP1251Codec->toUnicode(StrParam));
                              strcpy(StrParam,"");
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTERMINALID","OK"));
                              break;
                         }
                         case 0: 
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTERMINALID","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTERMINALID","INCORRECT VALUE"));
                              break;
                         }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSTIMEOUT1",Param, fp); 
                  QString temp = "";
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                              PSTimeout1-> setText(temp);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT1","OK"));
                              break;
                         }
                         case 0:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT1","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT1","INCORRECT VALUE"));
                              break;
                         } 
                  }
						
                  n = GetSchemeParam(SchemeNum,(char *)"PSTIMEOUT2",Param, fp); 
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                              PSTimeout2->setText(temp);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT2","OK"));
                              break;
                         }
                         case 0:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT2","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT2","INCORRECT VALUE"));
                              break;
                         } 
                  }		
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSTIMEOUT3",Param, fp); 
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                              PSTimeout3-> setText(temp);
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT3","OK"));
                              break;
                         }
                         case 0:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT3","NOT FOUND"));
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSTIMEOUT3","INCORRECT VALUE"));
                              break;
                         } 
                  }
				
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSNAMEDIR",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                              PSNameDir-> setText(CP1251Codec->toUnicode(StrParam));
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSNAMEDIR","OK"));
                              break;
                         }
                         case 0: 
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSNAMEDIR","NOT FOUND"));
                              if (TypeSystem == 2){//sber
                                     StrParam = "/usr/local/bin/SB/";
                                     PSNameDir-> setText(StrParam);    
                              }
                              break;
                         }
                         case -2:
                         {
                              ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSNAMEDIR","INCORRECT VALUE"));
                              break;
                         }
                  }
				
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSPORTNUM",Param, fp);
                  int PSPortNum = *Param;
                  switch (n)
                  {	    	      
                         case 1: 
                         {	
                              if (PSPortNum>0){  
                                    QString SearchingPortNum = "";
                                    QString sss = "";
                                    sss.setNum(PSPortNum);
                                    switch(TypePS){
                                          case 1:{SearchingPortNum = Com+sss;break;}
                                          case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                          case 3:{SearchingPortNum = UsbCom+sss; break;}
                                          case 4:{SearchingPortNum = Acm+sss; break;}
                                    }
                                    int kk = GetNumSearchingTextInComboBox(SearchingPortNum,6);
                                    if (kk == -1){
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in PSPortVerifone\n",SearchingPortNum.ascii());		
                                          PScheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    PSPortVerifone -> setCurrentItem(kk);
                              }
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUM","OK"));
                              break;
                         }
                         case 0:
                         {
                              //	  ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUM","NOT FOUND"));
                              //	  PScheckBox -> setChecked(FALSE);
                              n = GetSchemeParam(SchemeNum,(char *)"PSPORTNUMBERVER",Param, fp);
                              int PSPortNum = *Param;
                              switch (n)
                              {	    	      
                                    case 1: 
                                    {	
                                          if (PSPortNum>0){  
                                                QString SearchingPortNum = "";
                                                QString sss = "";
                                                sss.setNum(PSPortNum);
                                                switch(TypePS){
                                                      case 1:{SearchingPortNum = Com+sss;break;}
                                                      case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                      case 3:{SearchingPortNum = UsbCom+sss; break;}
                                                      case 4:{SearchingPortNum = Acm+sss; break;}
                                                }
                                                int kk = GetNumSearchingTextInComboBox(SearchingPortNum,6);
                                                if (kk == -1){
												ErrorNum = 1;
												dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in PSPortVerifone\n",SearchingPortNum.ascii());		
												PScheckBox -> setChecked(FALSE);
												break;
                                                }
                                                PSPortVerifone -> setCurrentItem(kk);
                                          }
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","OK"));
                                          break;
                                    }
                                    case 0:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","NOT FOUND"));	
                                          PScheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    case -2:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","INCORRECT VALUE"));
                                          PScheckBox -> setChecked(FALSE);
                                          break;
                                    } 
                              }
                              break;
                         }
                         case -2:
                         {
                              //ErrorNum = 1;
                              dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUM","INCORRECT VALUE"));
                              n = GetSchemeParam(SchemeNum,(char *)"PSPORTNUMBERVER",Param, fp);
                              int PSPortNum = *Param;
                              switch (n)
                              {	    	      
                                    case 1: 
                                    {	
                                          if (PSPortNum>0){  
                                                QString SearchingPortNum = "";
                                                QString sss = "";
                                                sss.setNum(PSPortNum);
                                                switch(TypePS){
                                                      case 1:{SearchingPortNum = Com+sss;break;}
                                                      case 2:{SearchingPortNum = DefCodec->toUnicode("клавиатурный");break;}
                                                      case 3:{SearchingPortNum = UsbCom+sss; break;}
                                                      case 4:{SearchingPortNum = Acm+sss; break;}
                                                }
                                                int kk = GetNumSearchingTextInComboBox(SearchingPortNum,6);
                                                if (kk == -1){						                                                                     ErrorNum = 1;						                                                                     dl->PrintString(DebugLevel,INFO,"PORT %s-------------not found in PSPortVerifone\n",SearchingPortNum.ascii());
                                                      PScheckBox -> setChecked(FALSE);
                                                      break;
                                                }
                                                PSPortVerifone -> setCurrentItem(kk);
                                          }
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","OK"));
                                          break;
                                    }
                                    case 0:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","NOT FOUND"));	
                                          PScheckBox -> setChecked(FALSE);
                                          break;
                                    }
                                    case -2:
                                    {
                                          ErrorNum = 1;
                                          dl->PrintString(DebugLevel,INFO,AlignParam("PSPORTNUMBERVER","INCORRECT VALUE"));
                                          PScheckBox -> setChecked(FALSE);
                                          break;
                                    } 
                              }
                              //PScheckBox -> setChecked(FALSE);
                              break;
                         } 
                  }
            
                  n = GetSchemeParam(SchemeNum,(char *)"PSBAUDRATEVER",Param, fp);
                  switch (n)
                  {
                         case 1: 
                         {
                                PSBaudRateVerifone -> setCurrentItem(GetItemNum(*Param));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBAUDRATEVER","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBAUDRATEVER","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBAUDRATEVER","INCORRECT VALUE"));
                                break;
                         }
                  } 
				
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSIPADDRESS",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSIPAddress-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESS","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESS","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESS","INCORRECT VALUE"));
                                break;
                         }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSIPPORT",Param, fp); 
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                                PSIPPort-> setText(temp);
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORT","OK"));
                                break;
                         }
                         case 0:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORT","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORT","INCORRECT VALUE"));
                                break;
                         } 
                  }
				
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSIPADDRESSVER",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSIPAddressVerifone-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESSVER","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESSVER","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPADDRESSVER","INCORRECT VALUE"));
                                break;
                         }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSIPPORTVER",Param, fp); 
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                                PSIPPortVerifone-> setText(temp);
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORTVER","OK"));
                                break;
                         }
                         case 0:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORTVER","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSIPPORTVER","INCORRECT VALUE"));
                                break;
                         } 
                  }
				
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSPRINTCANSELCHECK",Param, fp); 
                  temp.setNum(*Param);
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                                PSPrintCanselCheck-> setChecked(*Param);
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSPRINTCANSELCHECK","OK"));
                                break;
                         }
                         case 0:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSPRINTCANSELCHECK","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSPRINTCANSELCHECK","INCORRECT VALUE"));
                                break;
                         } 
                  }			
				
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSBANK",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSBank-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBANK","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBANK","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSBANK","INCORRECT VALUE"));
                                break;
                         }
                  }
				 
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSTRADENAME",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSTradeName-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTRADENAME","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTRADENAME","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTRADENAME","INCORRECT VALUE"));
                                break;
                         }
                  }
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSTOWNTRADE",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSTownTrade-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTOWNTRADE","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTOWNTRADE","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSTOWNTRADE","INCORRECT VALUE"));
                                break;
                         }
                  }
				
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSADDRESSTRADE",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSAddress-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSADDRESSTRADE","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSADDRESSTRADE","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSADDRESSTRADE","INCORRECT VALUE"));
                                break;
                         }
                  }
				
			
                  strcpy(StrParam,"");
                  n = GetStrSchemeParam(SchemeNum,(char *)"PSCARDMASK",StrParam, fp);
                  switch (n)
                  { 
                         case 1:
                         {
                                PSCardMask-> setText(CP1251Codec->toUnicode(StrParam));
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSCARDMASK","OK"));
                                break;
                         }
                         case 0: 
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSCARDMASK","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSCARDMASK","INCORRECT VALUE"));
                                break;
                         }
                  }
				
                  n = GetSchemeParam(SchemeNum,(char *)"PSENCRYPTIONANSWER",Param, fp); 
                  switch (n)
                  {	    	      
                         case 1: 
                         {
                                if (*Param == 0) Encryption0->setChecked(TRUE);//CP1251
                                else Encryption1->setChecked(TRUE);//DOS866
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSENCRYPTIONANSWER","OK"));
                                break;
                         }
                         case 0:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSENCRYPTIONANSWER","NOT FOUND"));
                                break;
                         }
                         case -2:
                         {
                                ErrorNum = 1;
                                dl->PrintString(DebugLevel,INFO,AlignParam("PSENCRYPTIONANSWER","INCORRECT VALUE"));
                                break;
                         } 
                  }			
                  break;
            }
            case 0:
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPE","NOT FOUND"));
                  break;
            }
            case -2:
            {
                  ErrorNum = 1;
                  dl->PrintString(DebugLevel,INFO,AlignParam("PSTYPE","INCORRECT VALUE"));
                  break;
            }
      } 
      //-----(+)-----Zuskin-----24-06-2011------
       //------External Discount System ------
      n = GetSchemeParam(SchemeNum,(char *)"EDSTYPE",Param, fp); 
      int TypeEDS = *Param;
      if (n==1)
      { 
            dl->PrintString(DebugLevel,INFO,AlignParam("EDSTYPE","OK"));
            if (TypeEDS == 0) EDScheckBox -> setChecked(FALSE);
            else EDScheckBox -> setChecked(TRUE);
            EDSproperties(EDScheckBox->isChecked());
      }
      n = GetStrSchemeParam(SchemeNum,(char *)"EDSIPADDR",StrParam, fp); 
      switch (n) {   
            case 1: {
		EDS_IpAddr-> setText(CP1251Codec->toUnicode(StrParam));
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSIPADDR","OK"));
		break;
            }
            case 0:  {
	                ErrorNum = 1;
		EDS_IpAddr-> setText("127.0.0.1");	
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSIPADDR","NOT FOUND"));
		break;
            }
            case -2:  {
		ErrorNum = 1;
		EDS_IpAddr-> setText("127.0.0.1");
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSIPADDR","INCORRECT VALUE"));
		break;
            }
      }
      n = GetSchemeParam(SchemeNum,(char *)"EDSPORT",Param, fp); 
      switch (n) {
            case 1: {
		EDS_IpPort->setValue(*Param);
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSPORT","OK"));
		break;
            }
            case 0:  {
	                ErrorNum = 1;
		EDS_IpPort->setValue(10065);
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSPORT","NOT FOUND"));
		break;
            }
            case -2:  {
		ErrorNum = 1;
		EDS_IpPort->setValue(10065);
		dl->PrintString(DebugLevel,INFO,AlignParam("EDSPORT","INCORRECT VALUE"));
		break;
            }
      }
	 
      // потом надо будет добавить чтение параметров EDSLOGICALNUMBER, EDSIPADDR, EDSPORT
      //---------------------------------------------------
      free(Param);
      free(StrParam);
	 
      CloseFile(fp);
	 
	 
      if (BCScheckBox->isChecked())
		  BCSPortNumChanged();
      if (MCRcheckBox->isChecked())
		  MCRPortNumChanged();
      if (FRcheckBox->isChecked())
		  FRModelChanged();
	
      PSModelChanged(); 	
      PSTypeChanged();
      if (FRMBoxCNL->currentItem() == 1)
            FRMBoxCNLChanged();
      FlagInLoading = false;
}



//======================================================//
//возвращает номер строки в комбо-боксах BaudRate по скорости
int Configurations::GetItemNum( int Rate )
{
//  printf("int Configurations::GetItemNum( int Rate )\n");  
  switch(Rate)
    {
      case 4: return 0;
      case 5: return 1;
      case 7: return 2;
      case 9: return 3;
      case 12: return 4;
      case 14: return 5;
      case 18: return 6;	  
    
    } 
  return 1;  
}



//======================================================//
//возвращает номер строки в комбо-боксах Модели по модели
int Configurations::GetModel( int Model )
{
	 switch(Model)
	 {
	 case 1: return 0;
	 case 2: return 1;
	 case 3: return 2;
	 case 4: return 3;
	 case 5: return 4;
	 }    
	 return 1;	  
}


//======================================================//
//возвращает скорость по номеру строки в комбобоксе BaudRate
int Configurations::GetBaudRate( int ItemNum )
{
  switch(ItemNum)
    {
      case 0: return 4;
      case 1: return 5;
      case 2: return 7;
      case 3: return 9;
      case 4: return 12;
      case 5: return 14;
      case 6: return 18;	
	 default:{return 5;}  
    }   
}

//======================================================//
int Configurations::bool_to_int( bool Var )
{
 if (Var == FALSE)
   return 0;
 else
   return 1;
}

//======================================================//
//сохранить текущую схему
int Configurations::Save()
{
   dl->PrintString(DebugLevel,INFO,"int Configurations::Save()\n");
   QString n_doc = CurrentDocNum->text();
   if (!n_doc.isEmpty()){
       int n_d = n_doc.toInt();
       hw->SetParam("CHECKNUMBER", 0, double(n_d));
   }
   QString er_str;
   QString er_id;
   FILE * fp;
   NewButton->setEnabled(TRUE);
   SchemeName->setEnabled(TRUE);
   if (SchemeName->count() == 0)
   {
       DefaultScheme->setChecked(true);
       if (SchemeName->currentText().isEmpty())
       {  
           QString s = "Scheme ";
           int col_schem = SchemeName->count()+1;
           s += s.setNum(col_schem);
           SchemeName->setCurrentText(CP1251Codec->toUnicode(s));
/*char* s = (char*) calloc (15,sizeof(char));
strcpy(s,"Scheme ");
*(s+7) = SchemeName->count()+1+48;
 SchemeName -> setCurrentText(CP1251Codec->toUnicode(s));
free(s);   */
       }
   }
   int n = 0;
   n = CheckPortNum();
   if (n == 1)
   {
       dl->PrintString(DebugLevel,INFO,"CheckPortNum - OK!\n"); 
       n = 0;
       n = CheckSufPrefScanner(); 
       if (n >= 0)
       {
           dl->PrintString(DebugLevel,INFO,"CheckScannerType - OK!\n");
           n = 0;
           n = CheckSufPrefReader();
           if (n >= 0)
           {
               dl->PrintString(DebugLevel,INFO,"CheckMCRType - OK!\n");
               n = 0;
               n = CheckSchemeName(CP1251Codec->fromUnicode(SchemeName->currentText()),1);
               if (n == 0)
               {
                   dl->PrintString(DebugLevel,INFO,"CheckSchemeName - OK!\n");
                   n = 0;
                   FILE* tmp; 
                   int SchemeNum = 0;
                   fp = fopen(way,"r");
                   if ( fp == NULL)
                   {
                       dl->PrintString(DebugLevel,INFO,"Can't open file'./ hw.conf'\n");
                       return -3;
                   }
                   if ((access(TEMP_WAY,0))==-1)
                   {
                       dl->PrintString(DebugLevel,INFO,"file doesn't exist  %s\n",TEMP_WAY);
                       tmp = fopen(TEMP_WAY,"a");
                       if (tmp == NULL)
                       {
                           dl->PrintString(DebugLevel,INFO,"Can't open file TEMP_WAY\n");	
                           return -3; 
                       }
                       fclose(tmp);							  
                   }
                   if ((access(TEMP_WAY,4))==-1)
                   {
                       dl->PrintString(DebugLevel,INFO,"file couldn't be read  %s\n",TEMP_WAY);
                       return -4;
                   }
                   tmp = fopen(TEMP_WAY,"w");
                   if (tmp == NULL)
                   {
                       dl->PrintString(DebugLevel,INFO,"Can't work with file'./hw.temp'\n");
                       return -4;
                   }

                   char * str = (char*) calloc (551,sizeof(char));
                   dl->PrintString(DebugLevel,INFO,"SchemeName->currentItem() = %d\n",SchemeName->currentItem());	    
                   while (!feof(fp))
                   {
                       fgets(str,550,fp);
                       if (SchemeNum < SchemeName->currentItem()+1)//все схемы до текущей в файле
                       { 
                           if (strstr(str,"[SCHEME]") != NULL)
                               SchemeNum++;
                           if (SchemeNum < SchemeName->currentItem()+1)
                               fprintf(tmp,str);
                       }		
                       while (SchemeNum == SchemeName->currentItem()+1)  //текущая схема
                       {
                           if  (fgets(str,550,fp) == NULL)//имею ввиду конец файла
                           {
                               n = 0;
                               break;
                           }
                           if (strstr(str,"[SCHEME]") != NULL)
                           {
                               SchemeNum++;
                           }
                           if (SchemeNum != SchemeName->currentItem()+1) 
                               break;  
                       }  
                       if (SchemeNum > SchemeName->currentItem()+1)  //все схемы после текущей пока пропускаем
                       { 
                           n = 1;
                           break;
                       } 
                       strcpy(str,"");
                   }
                   if (n != 0)//сохраняемая схема не последняя в списке схем
                   {
                       WriteConf(tmp);
                       fprintf(tmp,str);
                       while (!feof(fp))
                       {
                           fgets(str,550,fp);
                           fwrite(str, sizeof(char),strlen(str),tmp);
                           strcpy(str,"");
                       }
                   }
                   else 
                   {
                       WriteConf(tmp); 
                   }
                   fseek(tmp,0,SEEK_END);
                   SetDefaultScheme(tmp);
                   fclose(fp);
                   fclose(tmp);  
                   fp = fopen(way,"w");
                   if (fp == NULL)
                   {
                       dl->PrintString(DebugLevel,ERROR,"Can't open file './ hw.conf'\n");	
                       free(str);
                       return -3;
                   }
                   tmp = fopen(TEMP_WAY,"r");
                   if (tmp== NULL)
                   {
                       dl->PrintString(DebugLevel,ERROR,"Can't work with file './hw.temp'\n");
                       free(str);
                       return -4;
                   }
                   while (!feof(tmp))
                   {
                       fgets(str,550,tmp);
                       fwrite(str, sizeof(char),strlen(str),fp);
                       strcpy(str,"");
                   }
                   free(str);
                   fclose (fp);
                   fclose (tmp);
                   if (SchemeName->currentText().isEmpty())
                   { 
		/*char* s = (char*) calloc (15,sizeof(char));
		strcpy(s,"Scheme");
		*(s+7) = SchemeName->count()+1+48;
		 SchemeName->insertItem(CP1251Codec->toUnicode(s),SchemeName -> count());  
		 free(s);*/
		 QString s = "Scheme ";
                                 int col_schem = SchemeName->count()+1;
                                 s += s.setNum(col_schem);
                                 SchemeName->insertItem(CP1251Codec->toUnicode(s),SchemeName -> count());
                   }
                   else
                   {
                       QString ssss = SchemeName->currentText();
                       SchemeName -> insertItem(ssss,SchemeName->currentItem());
                   }
                    if (SchemeName->count() > 1) 
                       SchemeName -> removeItem(SchemeName -> currentItem()+1);
                   if (show_message){
                       DeviceConfMessageText(14,er_str, er_id);
                       InterfaceMessageWin(er_str, er_id);
                   }
                   DeleteEmptyString();
                   DeleteButton->setEnabled(TRUE); 	
                   
                   SaveFlag = true;
                   changeflag = false;
                   return 1;
               }
               else 
               {
                   QString er_str;
                   QString er_id;
                   switch (n)
                   {
                       case -1: {  
                           if (show_message){
                               DeviceConfErrorText(1, er_str, er_id);
                               InterfaceErrorWin(er_str, er_id);
                           }
                           return n;
                       }
                       case -2: { 
                           if (show_message){
                               DeviceConfMessageText(1, er_str, er_id);
                               InterfaceMessageWin(er_str, er_id);
                           }
                           return n; 
                       }
                       case -3: { 
                           if (show_message){
                               DeviceConfErrorText(2, er_str, er_id);
                               InterfaceErrorWin(er_str, er_id);
                           }
                           return n; 
                       }
                       case -4: { 
                           if (show_message){
                               DeviceConfErrorText(3, er_str, er_id);
                               InterfaceErrorWin(er_str, er_id);
                           }
                           return n; 
                       }
                   }
               }
           }
       }
       return n;  
   }
   else
   {
       //Схема не может быть сохранена, так как два вида оборудования настроены на один COM-порт
       if (show_message){
           DeviceConfErrorText(26, er_str, er_id);
           InterfaceErrorWin(er_str, er_id);
       }
   }
   return n; 
}




//======================================================//
//проверка что два или более вида оборудования не настроены на один порт вида com, usb<->com, acm
int Configurations::CheckPortNum()
{
	 dl->PrintString(DebugLevel,INFO,"int Configurations::CheckPortNum()\n");
	 unsigned int PortNum = 0; 
//-----(*)-----Zuskin-----30/08/2011-----
	// char * com = (char *) calloc(20,sizeof(char));  //Com-ports
	// char * comusb = (char *) calloc(20,sizeof(char));
	// char * acm = (char *) calloc(20,sizeof(char));
 	 char * com = (char *) calloc(100,sizeof(char));  //Com-ports
	 char * comusb = (char *) calloc(100,sizeof(char));
	 char * acm = (char *) calloc(100,sizeof(char));
//-------------------------------------------------	 
	 int TypePort = 0;

	 if (BCScheckBox -> isChecked())
	 {
		  TypePort = CheckScannerType();	
		  dl->PrintString(DebugLevel,INFO,"TypePort bcs = %d\n",TypePort );
		  QString s =  DefCodec->fromUnicode(BCSPortNum->currentText());
		  switch(TypePort){
		  case 1:{//com
					 int k = s.find(Com);
					 if (k!=-1){
						  PortNum = (s.right(s.length()-Com.length())).toInt();
						  dl->PrintString(DebugLevel,INFO,"bcs PortNum com = %d\n",PortNum);	
//-----(+)-----Zuskin-----30/08/2011-----						  
						  if (PortNum > 99) { 
						      free (com);
						      free (comusb); 
						      free (acm);
						      return -1; }
//-------------------------------------------------						  
						  if (*(com+PortNum) == 0)
								*(com+PortNum) = 48;
						  else{
								if (PortNum > 0) {
									 free (com);
									 free (comusb); 
									 free (acm);
									 return -2;
								}	
						  } 
					 }
					 break;}
		  case 3:{//ttyusb
					 int k = s.find(UsbCom);
					 if (k!=-1){
						  PortNum = (s.right(s.length()-UsbCom.length())).toInt();
						  dl->PrintString(DebugLevel,INFO,"bcs PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
						  if (PortNum > 99) { 
						      free (com);
						      free (comusb); 
						      free (acm);
						      return -1; }
//-------------------------------------------------	  
						  if (*(comusb+PortNum) == 0)
								*(comusb+PortNum) = 48;
						  else{
								if (PortNum > 0) {
									 free (com);
									 free (comusb); 
									 free (acm);
									 return -2;
								}	
						  }
					 }
					 break;}
		  case 4:{//acm
					 int k = s.find(Acm);
					 if (k!=-1){
						  PortNum = (s.right(s.length()-Acm.length())).toInt();
						  dl->PrintString(DebugLevel,INFO,"bcs PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
						  if (PortNum > 99) { 
						      free (com);
						      free (comusb); 
						      free (acm);
						      return -1; }
//-------------------------------------------------					  						  
						  if (*(acm+PortNum) == 0)
								*(acm+PortNum) = 48;
						  else{
								if (PortNum > 0) {
									 free (com);
									 free (comusb); 
									 free (acm);
									 return -2;
								}	
						  }
					 }
					 break;}
		  default:{break;}
	}
    }  
	 
    if (EWcheckBox -> isChecked())
    {
	QString s =  DefCodec->fromUnicode(EWPortNum->currentText());
	TypePort = CheckEWType();
	dl->PrintString(DebugLevel,INFO,"TypePort ew = %d\n",TypePort);
	switch(TypePort){
	case 1:{//com
		int k = s.find(Com);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Com.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ew PortNum com = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------				  		    
		    if (*(com+PortNum) == 0)
			*(com+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    } 
		}
		break;}
	case 3:{//ttyusb
		int k = s.find(UsbCom);
		if (k!=-1){
		    PortNum = (s.right(s.length()-UsbCom.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ew PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------
		    if (*(comusb+PortNum) == 0)
			*(comusb+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	case 4:{//acm
		int k = s.find(Acm);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Acm.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ew PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		 
		    if (*(acm+PortNum) == 0)
			*(acm+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	default:{break;}
	}
    }

    if (CDSPcheckBox -> isChecked())
    {
	QString s =  DefCodec->fromUnicode(CDSPPortNum->currentText());
	TypePort = CheckDisplayType();
	dl->PrintString(DebugLevel,INFO,"TypePort dsp = %d\n",TypePort);
	switch(TypePort){
	case 1:{//com
		int k = s.find(Com);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Com.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"dsp PortNum com = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(com+PortNum) == 0)
			*(com+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    } 
		}
		break;}
	case 3:{//ttyusb
		int k = s.find(UsbCom);
		if (k!=-1){
		    PortNum = (s.right(s.length()-UsbCom.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"dsp PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		   
		    if (*(comusb+PortNum) == 0)
			*(comusb+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	case 4:{//acm
		int k = s.find(Acm);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Acm.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"dsp PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		   
		    if (*(acm+PortNum) == 0)
			*(acm+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	default:{break;}
	}
    }
	 
    if (FRcheckBox -> isChecked())
    {
	QString s =  DefCodec->fromUnicode(FRPortNum->currentText());
	TypePort = CheckFRType();
	dl->PrintString(DebugLevel,INFO,"TypePort fr = %d\n",TypePort);
	switch(TypePort){
	case 1:{//com
		int k = s.find(Com);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Com.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"fr PortNum com = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		  
		    if (*(com+PortNum) == 0)
			*(com+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    } 
		}
		break;}
	case 3:{//ttyusb
		int k = s.find(UsbCom);
		if (k!=-1){
		    PortNum = (s.right(s.length()-UsbCom.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"fr PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(comusb+PortNum) == 0)
			*(comusb+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	case 4:{//acm
		int k = s.find(Acm);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Acm.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"fr PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(acm+PortNum) == 0)
			*(acm+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	default:{break;}
	}
    }

    if (MCRcheckBox -> isChecked())
    {
	QString s =  DefCodec->fromUnicode(MCRPortNum->currentText());
	TypePort = CheckMCRType();
	dl->PrintString(DebugLevel,INFO,"TypePort mcr = %d\n",TypePort);
	switch(TypePort){
	case 1:{//com
		int k = s.find(Com);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Com.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"mcr PortNum com = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(com+PortNum) == 0)
			*(com+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    } 
		}
		break;}
	case 3:{//ttyusb
		int k = s.find(UsbCom);
		if (k!=-1){
		    PortNum = (s.right(s.length()-UsbCom.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"mcr PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(comusb+PortNum) == 0)
			*(comusb+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	case 4:{//acm
		int k = s.find(Acm);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Acm.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"mcr PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(acm+PortNum) == 0)
			*(acm+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	default:{break;}
	}
    }  
    if (PScheckBox-> isChecked())
    {
	QString s =  DefCodec->fromUnicode(PSPortVerifone->currentText());
	TypePort = CheckPSType();
	dl->PrintString(DebugLevel,INFO,"TypePort ps = %d\n",TypePort);
	switch(TypePort){
	case 1:{//com
		int k = s.find(Com);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Com.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ps PortNum com = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(com+PortNum) == 0)
			*(com+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    } 
		}
		break;}
	case 3:{//ttyusb
		int k = s.find(UsbCom);
		if (k!=-1){
		    PortNum = (s.right(s.length()-UsbCom.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ps PortNum comusb = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(comusb+PortNum) == 0)
			*(comusb+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	case 4:{//acm
		int k = s.find(Acm);
		if (k!=-1){
		    PortNum = (s.right(s.length()-Acm.length())).toInt();
		    dl->PrintString(DebugLevel,INFO,"ps PortNum acm = %d\n",PortNum);
//-----(+)-----Zuskin-----30/08/2011-----						  
		    if (PortNum > 99) { 
		      free (com);
		      free (comusb); 
		      free (acm);
		    return -1; }
//-------------------------------------------------		    
		    if (*(acm+PortNum) == 0)
			*(acm+PortNum) = 48;
		    else{
			if (PortNum > 0) {
			    free (com);
			    free (comusb); 
			    free (acm);
			    return -2;
			}	
		    }
		}
		break;}
	default:{break;}
	} 
    }  
    dl->PrintString(DebugLevel,INFO,"CheckPort return all ok!!");
    free (com);
    free (comusb); 
    free (acm);
    return 1;
}




//======================================================//
//проверка что не существует такой же по названию схемы в файле конфигурации
//CurrentName - Имя схемы
//fl : 1 - такая уже должна существовать в файле
//     2 - с таким названием схемы в файле быть не должно
//Возвращает: 0 - не найдено
//                        -1 - найдена (если fl == 1 и схема не текущая) или (fl == 2)
int Configurations::CheckSchemeName(QString CurrentName, int fl)
{
	 dl->PrintString(DebugLevel,INFO,"int Configurations::CheckSchemeName(%s,%d)\n" ,CurrentName.ascii(),fl);
	 FILE *fp;
	 int res = 0;
	 if ((access(CONF_WAY,0))==-1)
	 {
		  dl->PrintString(DebugLevel,ERROR,"file doesn't exist CONF_WAY\n");
		  fp = fopen(CONF_WAY,"a");
		  if (fp == NULL) 
			return -3;
		  fclose(fp);
	 }
	 if ((access(CONF_WAY,4))==-1)
	 {
		  dl->PrintString(DebugLevel,ERROR,"ошибка доступа к файлу './ hw.conf'\n");
		  return -3;
	 }

	 fp = fopen(CONF_WAY,"r");
	 if (fp == NULL)
	 {
		  dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла './ hw.conf'\n");
		  return -3;
	 }
	 
	 char* current_name = (char*) calloc (CurrentName.length() + 1,sizeof(char)); 
	 strcpy(current_name,CurrentName);
	
	// printf("current_name = |%s|\n",current_name);
	//  strcpy(current_name,CP1251->fromUnicode(CurrentName));
	// printf("current_name = |%s|\n",current_name);
	 if (strlen(current_name) == 0)
	 {
		  free(current_name);
		  fclose(fp);
		  return -2;     
	 }
	 char** Scheme = 0;
	 int num = 0; 
	 int max_size = 0;
	 while (!feof(fp))
	 {
		  char * str = (char *)calloc(550,sizeof(char));
		  fgets(str,550,fp);
		  if (strstr(str,"[SCHEME]") != NULL)
		  { 
				num++;
				if (num > max_size)
				{
					 max_size = max_size + 1;
					 if (!Scheme)
						  Scheme = (char**)calloc(max_size,sizeof(char*)); 
					 else
						  Scheme = (char**)realloc(Scheme,max_size*sizeof(char*));
				}
				Scheme[num-1] = (char*) calloc(strlen(str) + 1,sizeof(char));
				char * tmp_str = (char *)calloc(strlen(str) + 1,sizeof(char));
			     getStrParam(str,tmp_str);
				strcpy(Scheme[num-1],tmp_str);
				free(tmp_str);
		  }
		  free(str);
	 }
	 if ( fl == 1)  // save   
	 { 
		  while (--num >=0)
		  {     
				if ((strcmp(Scheme[num],current_name) == 0) && (num != SchemeName->currentItem())) 
				{
					 free(Scheme[num]);
					 free(Scheme);
					 free(current_name);
					 fclose(fp);
					 return -1;
				} 
				free(Scheme[num]);   
		  }
	 }
	 if (fl == 2)
	 { 
		  while (--num >=0)
		  {
				if (strstr(Scheme[num],current_name) != NULL) 
				{
					 free(Scheme[num]);
					 free(Scheme);
					 free(current_name);
					 fclose(fp);
					 return -1;
				} 
				free(Scheme[num]);   
		  }
	 }      
	 free(Scheme);
	 free(current_name);
	 res = fclose(fp);
	 return 0;
}



  
//======================================================//
//устанавливает имя схемы по умолчанию
void Configurations::SetDefaultScheme(FILE* file)
{
    dl->PrintString(DebugLevel,INFO,"void Configurations::SetDefaultScheme(FILE* file)\n");  
    if (DefaultScheme -> isChecked())
    {
	fseek(file,0,0); 
	fprintf(file,"[DEFAULTSCHEME] %d\n",(SchemeName -> currentItem())+1);
	fprintf(file,"[DEBUGLEVEL] %d\n", DebugLevelComboBox -> currentItem());
	if ((access(CONF_WAY,0))==-1)
	{
	    file = fopen(CONF_WAY,"a");
	    if (file == NULL) return;
	    fclose(file);
	}
	file= fopen(CONF_WAY,"rw");
	if (file == NULL)
	{
	   dl->PrintString(DebugLevel,ERROR,"Can't open file'./ hw.conf'\n");
	}
	DSN = (SchemeName -> currentItem())+1;
	DefaultScheme -> setEnabled(false);
    }  
}

//======================================================//
//удаляет схему из файла с настройками
int Configurations::Delete()
{
  dl->PrintString(DebugLevel,INFO,"int Configurations::Delete()");
  QString er_str;
  QString er_id;  
  if (DefaultScheme->isChecked())
  {
		DeviceConfMessageText(2,er_str, er_id);
		InterfaceMessageWin(er_str, er_id);
  }
  else
  {
      if (SchemeName->isEnabled()==0)
      {
         NewButton->setEnabled(TRUE);
         FILE *fp;
         FILE* tmp; 
         int SchemeNum = 0;
         char* str; 
         str =  (char*)calloc(550,sizeof(char));
         if ((access(way,0))==-1)
         {
            dl->PrintString(DebugLevel,ERROR,"file doesn't exist way\n");
            fp = fopen(way,"a");
		  if (fp == NULL){
		    free(str);
		    return -1;
		  }
            fclose(fp); 
         }
         if ((access(way,4))==-1)
        {
            dl->PrintString(DebugLevel,ERROR,"file couldn't be read  way\n");
            free(str);
            return -1;
         }
	    fp = fopen(way,"r");
         if (fp == NULL)
         {
            dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла './ hw.conf'\n");
            free(str);
            return -1;
         }
         if ((access(TEMP_WAY,0))==-1)
         {
            dl->PrintString(DebugLevel,ERROR,"file doesn't exist TEMP_WAY\n");
            free(str);
            return -1;
         }
         if ((access(TEMP_WAY,4))==-1)
         {
            dl->PrintString(DebugLevel,ERROR,"file couldn't be read TEMP_WAY\n");
            free(str);
            return -1;
         }			
	    tmp = fopen(TEMP_WAY,"w");
         if (tmp== NULL)
         {
            dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла './hw.temp'\n");
            free(str);
            return -1;
         }
         if (DSN > SchemeName -> currentItem()+1)
            DSN--;
         fprintf (tmp,"[DEFAULTSCHEME] %d\n",int(DSN));
         fprintf (tmp,"[DEBUGLEVEL] %d\n",DebugLevelComboBox->currentItem());
         int fl = 0;
         while (!feof(fp))
         {
            fgets(str,550,fp);
            if (strstr(str,"[SCHEME]") != NULL)
            { 
               fl = 1; SchemeNum ++; 
            } 
            if ((SchemeNum != SchemeName -> currentItem()+1) && (fl == 1)) 
               fprintf(tmp,str);   
            strcpy(str,"");
         } 
          fclose (fp);
          fclose (tmp);
         if ((access(way,0))==-1)
        {
           dl->PrintString(DebugLevel,ERROR,"file doesn't exist way\n");
            return -1;
         }
         if ((access(way,4))==-1)
         {
               dl->PrintString(DebugLevel,ERROR,"file couldn't be read  way\n");
               return -1;
         }
	    fp = fopen(way,"w");
         if (fp == NULL)
         {
            dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла '/etc/hwsrv/hw.conf'\n");
            return -1;
         }
         if ((access(TEMP_WAY,0))==-1)
         {
            dl->PrintString(DebugLevel,ERROR,"file doesn't exist TEMP_WAY\n");
            return -1;
         }
          if ((access(TEMP_WAY,4))==-1)
         {
            dl->PrintString(DebugLevel,ERROR,"file couldn't be read  TEMP_WAY\n");
            return -1;
         }
	   tmp = fopen(TEMP_WAY,"r");
         if (tmp == NULL)
         {
            dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла '/etc/hwsrv/hw.temp'\n");
            return -1;
         }
         while (!feof(tmp))
         {
            fgets(str,550,tmp);
            fwrite(str, sizeof(char),strlen(str),fp);
            strcpy(str,"");
         } 
          fclose (fp);
         fclose (tmp);
         free(str);
         int j = SchemeName->currentItem();
         SchemeName -> removeItem(j); 
         if (j+1 < SchemeName->count())
         { 
            LoadScheme(j+1);
         }
         else
         {
            LoadScheme(j); 
            SchemeName->setCurrentText(SchemeName->text(j-1)); 
         }  
      }
      else
     {
         switch(QMessageBox::information( this, DefCodec->toUnicode("Удаление схемы"), DefCodec->toUnicode("Вы точно желаете удалить схему?"), DefCodec->toUnicode("Да"), DefCodec->toUnicode("Нет"),  0, 1))
         {
            case 0:
            {
                NewButton->setEnabled(TRUE);
                FILE *fp;
                FILE* tmp; 
                int SchemeNum = 0;
                char* str; 
                str =  (char*)calloc(550,sizeof(char));
                if ((access(way,0))==-1)
               {
                   dl->PrintString(DebugLevel,ERROR,"file doesn't exist way\n");
                   fp = fopen(way,"a");
			   if (fp == NULL){
			     free(str);	
				return -1;
			    }
                   fclose(fp);
               }
               if ((access(way,4))==-1)
               {
                   dl->PrintString(DebugLevel,ERROR,"file couldn't be read  way\n");
                   return -1;
                }
			fp = fopen(way,"r");
                if (fp == NULL)
                {
                    dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла './ hw.conf'\n");
                    return -1;
                }
                if ((access(TEMP_WAY,0))==-1)
                {
                   dl->PrintString(DebugLevel,ERROR,"file doesn't exist TEMP_WAY\n");
                   fp = fopen(TEMP_WAY,"a");
			   if (fp == NULL){
			     free(str);
				return -1;
			   }
                   fclose(fp);
                }
                if ((access(TEMP_WAY,4))==-1)
                {
                   dl->PrintString(DebugLevel,ERROR,"file couldn't be read  TEMP_WAY\n");
                   return -1;
                }
			tmp = fopen(TEMP_WAY,"w");
                if (tmp == NULL)
                {
                   dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла './hw.temp'\n");
                   return -1;
                 }
                if (DSN > SchemeName -> currentItem()+1)
                   DSN--;
                fprintf (tmp,"[DEFAULTSCHEME] %d\n\n",int(DSN));
                fprintf (tmp,"[DEBUGLEVEL] %d\n\n",DebugLevelComboBox->currentItem());
                int fl = 0;
                while (!feof(fp))
                {
                   fgets(str,550,fp);
                   if (strstr(str,"[SCHEME]") != NULL)
                   {
                      fl = 1; SchemeNum ++; 
                   } 
                   if ((SchemeNum != SchemeName -> currentItem()+1) && (fl == 1)) 
                     fprintf(tmp,str);   
                   strcpy(str,"");
                   }
                    fclose (fp);
                    fclose (tmp);
                   if ((access(way,0))==-1)
                   {
                     dl->PrintString(DebugLevel,ERROR,"file doesn't exist way\n");
                     fp = fopen(CONF_WAY,"a");
				if (fp == NULL){
					 free(str);
					 return -1;
				}
                     fclose(fp);
                   }
                   if ((access(way,4))==-1)
                   {
                      dl->PrintString(DebugLevel,ERROR,"file couldn't be read  way\n");
                      return -1;
                   }
			   fp = fopen(way,"w");
			   if (fp == NULL){
			      free(str);
				 return -1;
			   }
                   if (fp == NULL)
                   {
                      dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла '/etc/hwsrv/hw.conf'\n");
                      return -1;
                   }
                   if ((access(TEMP_WAY,0))==-1)
                   {
                      dl->PrintString(DebugLevel,ERROR,"file doesn't exist TEMP_WAY\n");
                      fp = fopen(TEMP_WAY,"a");
			      if (fp == NULL){
			        free(str);
				   return -1;
			      }
                      fclose(fp);
                   }
                   if ((access(TEMP_WAY,4))==-1)
                   {
                        dl->PrintString(DebugLevel,ERROR,"file couldn't be read TEMP_WAY\n");free(str);
                        return -1;
                   }
			   tmp = fopen(TEMP_WAY,"r");
                   if (tmp == NULL)
                  {
                      dl->PrintString(DebugLevel,ERROR,"ошибка при открытии файла '/etc/hwsrv/hw.temp'\n");                      free(str);
				 return -1;
                   }
                   while (!feof(tmp))
                   {
                      fgets(str,550,tmp);
                      fwrite(str, sizeof(char),strlen(str),fp);
                      strcpy(str,"");
                   }
	               free(str);	  
                    fclose (fp);
                    fclose (tmp);
			    DeviceConfMessageText(3,er_str, er_id);
	              InterfaceMessageWin(er_str, er_id);
                   int j = SchemeName->currentItem();
                   SchemeName -> removeItem(j); 
                   if (j+1 < SchemeName->count())
                   { 
                      LoadScheme(j+1);
                   }
                   else
                   {
                      LoadScheme(j); 
                      SchemeName->setCurrentText(SchemeName->text(j-1)); 
                   }
                }
            }
         }
     }
  return 1;
}


//======================================================//
//записать текущую конфигурацию в файл с настройками
int Configurations::WriteConf(FILE* file)
{   
	 dl->PrintString(DebugLevel,INFO,"int Configurations::WriteConf(FILE* file)\n");  
	 fseek(file,0 ,SEEK_END);
	 unsigned int i,n,m = 0;  
	 char* NS; int k;
	 NS = (char*) calloc(50,sizeof(char));
	 strcpy(NS,"[SCHEME] ");
	 *(NS+9) = 94;  
	 if (SchemeName->currentText().isEmpty())
	 { 
		  strcat(NS,"Scheme");
		  *(NS+17) = SchemeName->count()+48; 
	 }
	 else
		  strcat(NS, CP1251Codec->fromUnicode(SchemeName -> currentText()));
	 //printf("NAME SAVING SCHEME = %s\n",NS);
	 *(NS+(strlen(NS))) = 94;
	 m = strlen(NS);
	 if (m < 32)
		  for (i=0;i<(32-m);i++)
				*(NS+m+i) = ' ';
	 fprintf(file,"\n\n");
	 fprintf(file,NS);
	 free(NS);
	 fprintf(file,"\n");
	 fprintf(file,"#BarCodeScanner section\n");
   
	 i = CheckScannerType();
	 dl->PrintString(DebugLevel,INFO,"CheckScannerType() = %d\n",i);
	 fprintf(file,"[BCSTYPE] %d\n",i);
	 fprintf(file,"[BCSLOGICALNUMBER] %d\n",1);
    
	 QString PortNumber = CP1251Codec->fromUnicode(BCSPortNum->currentText());
	 dl->PrintString(DebugLevel,INFO,"bcs PortNumber = %s\n",PortNumber.ascii());
	 switch(i){
	 case 1:{//com-портовый
				k = PortNumber.find(Com);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Com.length());
					 fprintf(file,"[BCSPORTNUM] %d\n", sss.toInt()); 
				}	
				break;	
		  }
	 case 3:{//usb<->com-портовый
				k = PortNumber.find(UsbCom);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
					 fprintf(file,"[BCSPORTNUM] %d\n",sss.toInt()); 
				}
				break;	
		  }
	  case 4:{//acm-портовый
				k = PortNumber.find(Acm);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Acm.length());
					 fprintf(file,"[BCSPORTNUM] %d\n",sss.toInt()); 
				}
				break;	
		  }
	 default:{
				fprintf(file,"[BCSPORTNUM] 0\n"); 	
				break;
		  }
	 }
	 //-----------Mirabell 16/12/11
	 if (!BCSInterSymbolTimeout->isEnabled())
	 {
	     fprintf(file,"[BCSINTERSYMBOLTIMEOUT] 200\n");
	 }
	 else{
	     QString tmp_str = BCSInterSymbolTimeout->text();
	     fprintf(file,"[BCSINTERSYMBOLTIMEOUT] %s\n",tmp_str.ascii());
	 }
	 //-----------------------------------------------------------------------

	 n = GetBaudRate(BCSBaudRate -> currentItem());
	 if (n > 9)
		  fprintf(file,"[BCSBAUDRATE] %d\n",n);
	 else
	 {
		  NS = (char*) calloc(3,sizeof(char));
		  *NS = 48;
		  *(NS+1) = n+48;
		  fprintf(file,"[BCSBAUDRATE] %s\n",NS);   
		  free (NS);
	 }           
	 
	 if (!BCSPREF1->isEnabled())
	 {
		  fprintf(file,"[BCSPREF1] 000\n");
	 }
	 else
	 {
		  NS = (char*)calloc(4,sizeof(char)); 
		  n = strlen(BCSPREF1 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i) = 48;
		  strcat(NS, BCSPREF1 -> text());
		  fprintf(file,"[BCSPREF1] %s\n",NS);
		  free(NS);
	 }

	 NS = (char*)calloc(4,sizeof(char));
	 n = strlen(BCSPREF2 -> text());
	 if (n < 3)
		  for (i=0;i<(3-n);i++)
				*(NS+i) = 48;
	 strcat(NS, BCSPREF2 -> text());
	 fprintf(file,"[BCSPREF2] %s\n",NS);
	 free(NS);

	 NS = (char*)calloc(4,sizeof(char));
	 n = strlen(BCSSUF1 -> text());
	 if (n < 3)
		  for (i=0;i<(3-n);i++)
				*(NS+i) = 48;
	 strcat(NS, BCSSUF1 -> text());
	 fprintf(file,"[BCSSUF1] %s\n",NS); 
	 free(NS);
 
	 if (!BCSSUF2->isEnabled())
		  fprintf(file,"[BCSSUF2] 000\n");
	 {
		  NS = (char*)calloc(4,sizeof(char));
		  n = strlen(BCSSUF2 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i) = 48;
		  strcat(NS, BCSSUF2 -> text());
		  fprintf(file,"[BCSSUF2] %s\n",NS);
		  free(NS); 
	 }
	 fprintf(file,"#Elecrtonic Weighter section\n");  
	 if (EWcheckBox -> isChecked())
		  i = CheckEWType();
	 else
		  i = 0;
	 fprintf(file,"[EWTYPE] %d\n",i);
	 fprintf(file,"[EWLOGICALNUMBER] %d\n",1);
	 
	 PortNumber = CP1251Codec->fromUnicode(EWPortNum->currentText());
	 dl->PrintString(DebugLevel,INFO,"ew PortNumber = %s\n",PortNumber.ascii());
	 switch(i){
	 case 1:{//com-портовый
				k = PortNumber.find(Com);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Com.length());
					 fprintf(file,"[EWPORTNUM] %d\n", sss.toInt()); 
				}	
				break;	
		  }
	 case 3:{//usb<->com-портовый
				k = PortNumber.find(UsbCom);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
					 fprintf(file,"[EWPORTNUM] %d\n",sss.toInt()); 
				}
				break;	
		  }	
	 case 4:{//acm-портовый
				k = PortNumber.find(Acm);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Acm.length());
					 fprintf(file,"[EWPORTNUM] %d\n",sss.toInt()); 
				}
				break;	
		  }
	 default:{
					 fprintf(file,"[EWPORTNUM] 0\n"); 	
					 break;
				}
	 }
	 n = GetBaudRate(EWBaudRate -> currentItem());
	 if (n > 9)
		  fprintf(file,"[EWBAUDRATE] %d\n",n);
	 else
	 {
		  NS = (char*) calloc(3,sizeof(char));
		  *NS = 48;
		  *(NS +1) = n+48;
		  fprintf(file,"[EWBAUDRATE] %s\n",NS);  
		  free(NS); 
	 }
	 fprintf(file,"[EWMODEL] %d\n",(EWModel -> currentItem())+1);	
	 fprintf(file,"#Customer Display section\n");  
	  i = CheckDisplayType();
	 fprintf(file,"[CDSPTYPE] %d\n",i);
	  fprintf(file,"[CDSPLOGICALNUMBER] %d\n",1);
	 if (i == 5){
		  string cur_str =  CDSPPortNum->text(CDSPPortNum->currentItem());
		  int usb_num =  cur_str.find(Usb.ascii());
		  if (usb_num!=-1){
				int pos = Usb.length();
				//printf("pos = %d\n",pos);
				string n = "";
				while (cur_str.at(pos)!=32){
					n += cur_str.at(pos);
					pos++;
				}
				usb_num = atoi(n.c_str());
				int id_pr = 0;
				int id_vend = 0;
				for (unsigned int i = 0; i<usb_struct.size();i++){
					 if (usb_struct[i].usbNum == usb_num){
						  id_pr =   usb_struct[i].idProd;
						  id_vend = usb_struct[i].idVend;
					 }
				}
				/*printf("*****************usb_num = %d\n",usb_num);
				printf("*****************idProd = %d\n",id_pr);
				printf("*****************idVend = %d\n",id_vend);*/
				fprintf(file,"[CDSPIDPRODUCT] %d\n",id_pr);
				fprintf(file,"[CDSPIDVENDOR] %d\n",id_vend);	
		  
		  }
		  else{
				fprintf(file,"[CDSPIDPRODUCT] %d\n",0);
				fprintf(file,"[CDSPIDVENDOR] %d\n",0);	
		  }
		  fprintf(file,"[CDSPPORTNUM] %d\n",0);
	 }
	 else{
	 
		  fprintf(file,"[CDSPIDPRODUCT] %d\n",0);
		  fprintf(file,"[CDSPIDVENDOR] %d\n",0);
	 
		  
		  
		  QString PortNumber = CP1251Codec->fromUnicode(CDSPPortNum->currentText());
		  dl->PrintString(DebugLevel,INFO,"cdsp PortNumber = %s\n",PortNumber.ascii());
		  switch(i){
		  case 1:{//com-портовый
					 k = PortNumber.find(Com);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-Com.length());
						  fprintf(file,"[CDSPPORTNUM] %d\n", sss.toInt()); 
					 }	
					 break;	
				}
				
		  case 3:{//usb<->com-портовый
					 k = PortNumber.find(UsbCom);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
						  fprintf(file,"[CDSPPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}	
		  case 4:{//acm-портовый
					 k = PortNumber.find(Acm);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-Acm.length());
						  fprintf(file,"[CDSPPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}
		  default:{
					 fprintf(file,"[CDSPPORTNUM] 0\n"); 	
					 break;
				}
		  }
	 }

	 n = GetBaudRate(CDSPBaudRate -> currentItem());
	 if (n > 9)
		  fprintf(file,"[CDSPBAUDRATE] %d\n",n);
	 else
	 {
		  NS = (char*) calloc(3,sizeof(char));
		  *NS = 48;
		  *(NS +1) = n+48;
		  fprintf(file,"[CDSPBAUDRATE] %s\n",NS);   
		  free(NS);
	 }
	 fprintf(file,"[CDSPPERECODIROVKA] %d\n",(CDSPPerec -> currentItem())+1);
	 fprintf(file,"[CDSPNUMBERPROT] %d\n",(CDSPNumberProt -> currentItem())+1);
	 fprintf(file,"[CDSPBRIGHTNESS] %d\n",(CDSPBrightness -> currentItem())+1);
	 fprintf(file,"[CDSPSCROLLSPEED] %d\n",(CDSPScrollSpeed -> currentItem())+1);
	 fprintf(file,"[CDSPLENGTHSTR] %d\n",(CDSPLengthStr -> currentItem())+1);

	 QString upstr = CP1251Codec->fromUnicode(CDSP_UP->text());
	 char * upvalue = (char * )calloc(501,sizeof(char));
	 strcpy(upvalue,upstr.ascii());
	 QString downstr = CP1251Codec->fromUnicode(CDSP_DOWN->text());
	 char * downvalue = (char * )calloc(501,sizeof(char));
	 strcpy(downvalue,downstr.ascii()); 
	 NS = (char*)calloc(520,sizeof(char));

	 strcpy(NS,"[DEMONUPSTRING] "); 
	 *(NS+16) = 94;
	 strcat(NS, upvalue);
	 *(NS+(strlen(NS))) = 94;
	 strcat(NS,"\n");
	 fprintf(file, "%s", NS );
	 free(NS);

	 NS = (char*)calloc(525,sizeof(char));
	 strcpy(NS,"[DEMONDOWNSTRING] "); 
	 *(NS+18) = 94;  
	 strcat(NS, downvalue);
	 *(NS+(strlen(NS))) = 94;
	 strcat(NS,"\n");
	 fprintf(file,"%s",NS);
	 free(NS);

	 free(upvalue);
	 free(downvalue);   

	 fprintf(file,"[DEMONUPALIGN] %d\n",(CDSP_UP_Align_comboBox -> currentItem())+1);
	 fprintf(file,"[DEMONDOWNALIGN] %d\n",(CDSP_DOWN_Align_comboBox -> currentItem())+1);
	 fprintf(file,"[DEMONUPSCROLLMODE] %d\n",(CDSP_UP_ScrollMode_checkBox->isChecked()));
	 fprintf(file,"[DEMONDOWNSCROLLMODE] %d\n",(CDSP_DOWN_ScrollMode_checkBox->isChecked()));
	 fprintf(file,"#FR section\n"); 
	 if (FRcheckBox -> isChecked())
		  i = CheckFRType();
	 else
		  i = 0;
	 fprintf(file,"[ECRTYPE] %d\n",i);
	 
	 fprintf(file,"[ECRLOGICALNUMBER] %d\n",1);
	 
	 PortNumber = DefCodec->fromUnicode(FRPortNum->currentText());
	 switch(i){
	 case 1:{
				QString sss = PortNumber.right(PortNumber.length()-Com.length());
				fprintf(file,"[ECRPORTNUM] %d\n",sss.toInt()); 
				break;
		  }
	 case 3:{
				QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
				fprintf(file,"[ECRPORTNUM] %d\n",sss.toInt()); 
				break;
		  }
	 case 4:{
				QString sss = PortNumber.right(PortNumber.length()-Acm.length());
				fprintf(file,"[ECRPORTNUM] %d\n",sss.toInt()); 
				break;
		  }
	 default:{
					 fprintf(file,"[ECRPORTNUM] 0\n"); 	break;
				}
	 }

	 n = GetBaudRate(FRBaudRate -> currentItem());
	 if (n > 9)
		  fprintf(file,"[ECRBAUDRATE] %d\n",n);
	 else
	 {
		  NS = (char*) calloc(3,sizeof(char));
		  *NS = 48;
		  *(NS +1) = n+48;
		  fprintf(file,"[ECRBAUDRATE] %s\n",NS);  
		  free(NS); 
	 }
	 fprintf(file,"[ECRTYPEDEVICE] %d\n",(FRTypeDevice -> currentItem()));
	 int ECRMod = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()), 1);
	 fprintf(file,"[ECRMODEL] %d\n",ECRMod);
	 
	// fprintf(file,"[ECRMODEL] %d\n",(FRProtocol -> currentItem())+1);
	 fprintf(file,"[USEMONEYBOX] %d\n",bool_to_int(UseMoneyBox -> isChecked()));
	 fprintf(file,"[ECRUSEACCESSPASSWORD] %d\n",bool_to_int(UseAccessPassword ->isChecked()));
	 NS = (char*)calloc(35+AccessPassword -> text().length(),sizeof(char));
	 strcpy(NS,"[ECRACCESSPASSWORD] "); //  maxlen == 4
	 *(NS+20) = 94;
	 n = strlen(AccessPassword -> text());
	 while((4-n)>0){
		  char ch[1];
		  ch[0] = 48;
		  strcat(NS,ch);
		  n++;	  
	 }
	 strcat(NS, AccessPassword -> text());
	 *(NS+(strlen(NS))) = 94;
	 fprintf(file,NS);
	 fprintf(file,"\n");
	 free(NS); 
	 NS = (char*)calloc(27,sizeof(char)); 
	 strcpy(NS,"[ECRPASSWORD] "); // maxlen == 8
	 *(NS+14) =94;
	 n = strlen(AdminPassword ->text());
	 if (n < 8)
	 {
		  char* S2 = (char*) calloc(9,sizeof(char));
		  for (i=0;i<(8-n);i++)
				*(S2+i) = 48;
		  strcat(NS,S2);
		  free (S2);
	 }
	 strcat(NS, AdminPassword -> text());
	 *(NS+ strlen(NS)) = 94;
	 fprintf(file,NS);
	 fprintf(file,"\n");
	 free(NS);
                 fprintf(file,"[ECRMBOXCNL] %d\n", FRMBoxCNL->currentItem()+1);
	 fprintf(file,"[ECRMBOXI1] %d\n", atoi(FRMBoxI1->text()));
	 fprintf(file,"[ECRMBOXI2] %d\n", atoi(FRMBoxI2->text()));
	 fprintf(file,"[ECRMBOXICNT] %d\n",atoi(FRMBoxICnt->text()));
	 fprintf(file,"#MagneticCardReader section\n");  
	 i = CheckMCRType();
	 PortNumber = DefCodec->fromUnicode(MCRPortNum->currentText());
	 fprintf(file,"[MCRTYPE] %d\n",i);  
                 fprintf(file,"[MCRLOGICALNUMBER] %d\n",1);
	 switch(i){
		  case 1:{//com-портовый
				k = PortNumber.find(Com);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Com.length());
					 fprintf(file,"[MCRPORTNUM] %d\n", sss.toInt()); 
				}	
				break;	
		  }
		  case 3:{//usb<->com-портовый
					 k = PortNumber.find(UsbCom);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
						  fprintf(file,"[MCRPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}	
		  case 4:{//acm-портовый
					 k = PortNumber.find(Acm);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-Acm.length());
						  fprintf(file,"[MCRPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}
		  default:{
					 fprintf(file,"[MCRPORTNUM] 0\n"); 	break;
				}
    }
	 if (i != 2)
	 {
		  n = GetBaudRate(MCRBaudRate -> currentItem());
		  if (n > 9)
				fprintf(file,"[MCRBAUDRATE] %d\n",n);
		  else
		  {
				NS = (char*) calloc(3,sizeof(char));
				*NS = 48;
				*(NS +1) = n+48;
				fprintf(file,"[MCRBAUDRATE] %s\n",NS);  
				free(NS); 
		  }  
	 }
	 else
		  fprintf(file,"[MCRBAUDRATE] 00\n"); 
    
	 if (MCRPortNum -> currentItem() == 0)
	 {
		  NS = (char*)calloc(4,sizeof(char)) ;
		  n = strlen(MCRPREF1 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i) = 48;
		  strcat(NS, MCRPREF1 -> text());
		  fprintf(file,"[MCRPREF1] %s\n",NS);
		  free(NS);
      
		  NS = (char*)calloc(4,sizeof(char)) ;
		  n = strlen(MCRPREF2 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i) = 48;
		  strcat(NS,MCRPREF2 -> text());
		  fprintf(file,"[MCRPREF2] %s\n",NS);
		  free(NS);
      
		  NS = (char*)calloc(4,sizeof(char)) ;
		  n = strlen(MCRSUF1 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i)= 48;
		  strcat(NS,MCRSUF1 -> text());
		  fprintf(file,"[MCRSUF1] %s\n",NS);
		  free(NS);
      
		  NS = (char*)calloc(4,sizeof(char)) ;   
		  n = strlen(MCRSUF2 -> text());
		  if (n < 3)
				for (i=0;i<(3-n);i++)
					 *(NS+i) = 48;
		  strcat(NS,MCRSUF2 -> text());
		  fprintf(file,"[MCRSUF2] %s\n",NS); 
		  free(NS);
	 }
	 else
	 {
		  fprintf(file,"[MCRPREF1] 0\n");
		  fprintf(file,"[MCRPREF2] 0\n");
		  fprintf(file,"[MCRSUF1] 0\n");
		  fprintf(file,"[MCRSUF2] 0\n"); 
	 }
	 fprintf(file,"#Payment System  section\n"); 
	 i = CheckPSType();
	 fprintf(file,"[PSTYPE] %d\n",i);  
	 fprintf(file,"[PSLOGICALNUMBER] %d\n",1);
	 
	 int typeSystem = PSType->currentItem();
	 fprintf(file,"[PSTYPESYSTEM] %d\n",typeSystem); 
	 
	 int typeScheme = PSTypeScheme->currentItem();
	 fprintf(file,"[PSTYPESCHEME] %d\n",typeScheme); 
	 
	 
	 int ii = PSTypeExchange->currentItem();
	 fprintf(file,"[PSTYPEEXCHANGE] %d\n",ii); 
	 
	 
	 char * terid = (char *)calloc(PSTerminalId->text().length(),sizeof(char));
	 strcpy(terid,CP1251Codec->fromUnicode(PSTerminalId->text()));
	 fprintf(file,"[PSTERMINALID] ^%s^\n",terid);
	 free(terid);
	 
	 char * timeout = (char *)calloc(PSTimeout1->text().length(),sizeof(char)); 
	 strcpy(timeout,PSTimeout1->text()); 
	 fprintf(file,"[PSTIMEOUT1] %d\n",atoi(timeout)); 
	 
	 strcpy(timeout,PSTimeout2->text()); 
	 fprintf(file,"[PSTIMEOUT2] %d\n",atoi(timeout)); 
	 
	 strcpy(timeout,PSTimeout3->text()); 
	 fprintf(file,"[PSTIMEOUT3] %d\n",atoi(timeout)); 
	 free(timeout);
	 char * d = (char *)calloc(CP1251Codec->fromUnicode(PSNameDir->text()).length()+2,sizeof(char));
	 strcpy(d,CP1251Codec->fromUnicode(PSNameDir->text()));
	 fprintf(file,"[PSNAMEDIR] ^%s^\n",d); 
	 free(d);
	 	 
	 PortNumber = DefCodec->fromUnicode(PSPortVerifone->currentText());
	 switch(i){
		  case 1:{//com-портовый
				k = PortNumber.find(Com);
				if(k!=-1){
					 QString sss = PortNumber.right(PortNumber.length()-Com.length());
					 fprintf(file,"[PSPORTNUM] %d\n", sss.toInt()); 
				}	
				break;	
		  }
		  case 3:{//usb<->com-портовый
					 k = PortNumber.find(UsbCom);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-UsbCom.length());
						  fprintf(file,"[PSPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}	
		  case 4:{//acm-портовый
					 k = PortNumber.find(Acm);
					 if(k!=-1){
						  QString sss = PortNumber.right(PortNumber.length()-Acm.length());
						  fprintf(file,"[PSPORTNUM] %d\n",sss.toInt()); 
					 }
					 break;	
				}
		  default:{
					 fprintf(file,"[PSPORTNUM] 0\n"); 	break;
				}
		  }

	 n = GetBaudRate(PSBaudRateVerifone -> currentItem());
	 if (n > 9)
		  fprintf(file,"[PSBAUDRATEVER] %d\n",n);
	 else
	 {
		  NS = (char*) calloc(3,sizeof(char));
		  *NS = 48;
		  *(NS +1) = n+48;
		  fprintf(file,"[PSBAUDRATEVER] %s\n",NS);  
		  free(NS); 
	 }
	 char * ipaddr = (char *)calloc(CP1251Codec->fromUnicode(PSIPAddress->text()).length(),sizeof(char));
	 strcpy(ipaddr,CP1251Codec->fromUnicode(PSIPAddress->text()));
	// printf("ipaddr = |%s|\n",ipaddr);
	 fprintf(file,"[PSIPADDRESS] ^%s^\n",ipaddr); 
	 free(ipaddr);
	 
	 int port = (PSIPPort->text()).toInt();
	 fprintf(file,"[PSIPPORT] ^%d^\n",port); 

	 char * ipaddrver = (char *)calloc(CP1251Codec->fromUnicode(PSIPAddressVerifone->text()).length(),sizeof(char));
	 strcpy(ipaddrver,CP1251Codec->fromUnicode(PSIPAddressVerifone->text()));
	// printf("ipaddrver = |%s|\n",ipaddrver);
	 fprintf(file,"[PSIPADDRESSVER] ^%s^\n",ipaddrver); 
	 free(ipaddrver);
	 
	 int portver = (PSIPPortVerifone->text()).toInt();
	 fprintf(file,"[PSIPPORTVER] ^%d^\n",portver); 
	// printf("portver = |%d|\n",portver);
	 fprintf(file,"[PSPRINTCANSELCHECK] %d\n",bool_to_int(PSPrintCanselCheck->isChecked()));
				
	 char * bank = (char *)calloc(CP1251Codec->fromUnicode(PSBank->text()).length()+1,sizeof(char));
	 strcpy(bank,CP1251Codec->fromUnicode(PSBank->text()));
	// printf("bank = |%s|\n",bank);
	 fprintf(file,"[PSBANK] ^%s^\n",bank); 
	 free(bank);
	 
	 char * nametrade = (char *)calloc(CP1251Codec->fromUnicode(PSTradeName->text()).length()+1,sizeof(char));
	 strcpy(nametrade,CP1251Codec->fromUnicode(PSTradeName->text()));
	// printf("nametrade = |%s|\n",nametrade);
	 fprintf(file,"[PSTRADENAME] ^%s^\n",nametrade); 
	 free(nametrade);
	 
	 
	 char * town = (char *)calloc(CP1251Codec->fromUnicode(PSTownTrade->text()).length()+1,sizeof(char));
	 strcpy(town,CP1251Codec->fromUnicode(PSTownTrade->text()));
	 //printf("town = |%s|\n",town);
	 fprintf(file,"[PSTOWNTRADE] ^%s^\n",town); 
	 free(town);
	 
	 char * address = (char *)calloc(CP1251Codec->fromUnicode(PSAddress->text()).length()+1,sizeof(char));
	 strcpy(address,CP1251Codec->fromUnicode(PSAddress->text()));
	 //printf("address = |%s|\n",address);
	 fprintf(file,"[PSADDRESSTRADE] ^%s^\n",address); 
	 free(address);
	 
	 
	 char * mask = (char *)calloc(17,sizeof(char));
	 strcpy(mask,PSCardMask->text());
	 while (strlen(mask)<16){
		  strcat(mask," ");
	 }
	 fprintf(file,"[PSCARDMASK] ^%s^\n",mask);
	 free(mask);
	 
	 if (Encryption0->isChecked()){
		  fprintf(file,"[PSENCRYPTIONANSWER] 0\n");
	 }
	 else
		  fprintf(file,"[PSENCRYPTIONANSWER] 1\n");
	 fprintf(file,"\n");
	 
	 //-----(+)-----Zuskin-----24-06-2011-----
	 fprintf(file,"#External Discount System  section\n"); 
	 i = CheckEDSType();
	 fprintf(file,"[EDSTYPE] %d\n",i);  
	 fprintf(file,"[EDSLOGICALNUMBER] %d\n",1);
	 QString ipstr = CP1251Codec->fromUnicode(EDS_IpAddr->text());
	 if (ipstr.length() < 7) {
	     ipstr = QString("127.0.0.1");
	 }
	 fprintf(file,"[EDSIPADDR] ^%s^\n",ipstr.ascii());
	 ipstr = QString::number(EDS_IpPort->value(), 10);
	 fprintf(file,"[EDSPORT] ^%s^\n",ipstr.ascii());
	 //-------------------------------------------------
	 
	 return 1;	
}



//======================================================//
//создать список схем
int Configurations::MakeSchemeList()
{ 
    dl->PrintString(DebugLevel,INFO,"void Configurations::MakeSchemeList()\n");   
    QString er_str;
    QString er_id;
    FILE *fp;
    int c,bi=0;
    int MaxLen = 6;
    int MaxResLen = 40;
    bool Ind1 = false, Ind2 = false, Ind3 = false;
    int *DL = (int *) calloc (1,sizeof(int));
    int *DefaultSchemeNum = (int *) calloc (1,sizeof(int));   
    char * Buf = (char *) calloc (MaxLen+1, sizeof(char));  
    char * Val = (char *) calloc(MaxResLen+1, sizeof(char));
    char * PName = (char *) calloc (MaxResLen+1, sizeof(char));
    strcpy(PName, "SCHEME");
    *(Buf+MaxLen) = 0;
    *(Val+MaxResLen) = 0;
    way = (char *) calloc (30, sizeof(char));
    strcpy(way,CONF_WAY);
    fp = fopen(way,"r");
    if (!fp)
    {     
	conf_file_exist = false;
	fp = fopen(way,"w");
	if (fp == NULL)
	{
	    DeviceConfMessageText(4,er_str,er_id);
	    InterfaceMessageWin(er_str, er_id);
	    return -1; 
	}
	DeviceConfMessageText(5,er_str,er_id);
	InterfaceMessageWin(er_str, er_id);
	fprintf(fp,"[DEFAULTSCHEME] 1\n");
	fprintf(fp,"[DEBUGLEVEL] 1\n");
	fprintf(fp,"\n\n");
	fseek(fp, 0L, SEEK_SET);//установить курсор в конец файла!!!
	fclose(fp);
	New();
	DeleteButton->setEnabled(FALSE);
	SaveButton->setEnabled(TRUE);
	NewButton->setEnabled(TRUE);
	SchemeName->setEnabled(TRUE);	
	return 0;
    }
    else
    {
	int v=fgetc(fp);
	if(v==-1) 
	{
	    DeviceConfMessageText(6,er_str,er_id);
	    InterfaceMessageWin(er_str, er_id);
	    DeviceConfMessageText(7,er_str,er_id);
	    InterfaceMessageWin(er_str, er_id);
	    fp = fopen(CONF_WAY,"w");
	    if (fp == NULL) return -1;
	    fprintf(fp,"[DEFAULTSCHEME] 1\n");
	    fprintf(fp,"[DEBUGLEVEL] 1\n");
	    fprintf(fp,"\n\n");
	    fseek(fp, 0L, SEEK_SET);
	    fclose(fp);
	    New();
	    DeleteButton->setEnabled(FALSE);
	    SaveButton->setEnabled(TRUE);
	    NewButton->setEnabled(TRUE);
	    DeviceConfMessageText(8,er_str,er_id);
	    InterfaceMessageWin(er_str, er_id);
	    return 0;
	}

	while ((c = fgetc(fp)) != EOF)
	{
	    if (Ind1 == false)
	    {
		if (c==0x5B) // c == '['
		    Ind1 = true;
	    }
	    else
		if (Ind2 == false)
		{
		if (c == 0x5D) // c == ']'
		{
		    if (bi==0)
		    {
			Ind1=false;
		    }
		    else
		    {
			*(Buf+bi)=0;
			if (!strcmp(PName, Buf))
			{
			    bi=0;
			    Ind2=true;
			}
			else
			{
			    Ind1=false;
			    bi=0;
			}     
		    }
		}   
		else
		{
		    if  ( (c > 0x40) && (c< 0x5B) )
		    {
			if (bi>MaxLen-1)
			{
			    bi=0;
			    Ind1 = false;
			}
			else
			{      
			    *(Buf+bi) = c;
			    bi++;
			}
		    }
		}   
	    }
	    else //Ind2 == true !!!	  
	    {
		if (c==0x5B)
		{
		    if (bi==0)
		    {
			break;
		    }
		    else
		    {
			if (Ind3==true) 
			{
			    *(Val+bi) = 0;	 
			    SchemeName -> insertItem(CP1251Codec->toUnicode(Val),-1);                      
			    Ind1 = false;
			    Ind2 = false;
			    Ind3 = false;
			    bi = 0;
			}
			else
			    break;
		    }	 
		}
		else // we've got a symbol, that is not '['
		{
		    if (Ind3 == true)
		    {
			if (c != 0x5E)
			{
			    if (bi>MaxResLen-1)
				break;
			    *(Val+bi) = c;
			    bi++;
			}
			if ( c == 0x5E ) // if we've got '^' (wich means the end of the value)
			{
			    if (bi == 0)
				break;
			    else
			    {
				if (bi<MaxResLen)
				    *(Val+bi) = 0;
				SchemeName -> insertItem(CP1251Codec->toUnicode(Val),-1); 
				Ind1 = false;
				Ind2 = false;
				Ind3 = false;
				bi = 0;
			    }
			}
		    } 
		    else //Ind3 == false
		    {
			if (c == 0x5E) // if we've got '^' (wich means the beginning of the value)
			    Ind3 = true;
		    }
		}
	    }
	}
	if ((Ind1 == true) && (Ind2 == true) && (Ind3 == true) && (bi > 0))
	{
	    *(Val+bi) = 0;
	    SchemeName -> insertItem(CP1251Codec->toUnicode(Val),-1); 
	}
	free(Buf);
	free(Val);
	free(PName);
	fseek(fp,0,0); 
	int j = GetIntParam((char *)"DEFAULTSCHEME", DefaultSchemeNum);
	DSN = 1;
	if (j == 1) 
	{ 
	    DSN = *DefaultSchemeNum;    
	    SchemeName -> setCurrentItem(*DefaultSchemeNum-1) ;
	    LoadScheme(*DefaultSchemeNum);
	    GetIntParam((char *)"DEBUGLEVEL", DL);
	    int res = *DL;
	    free(DL); 
	    switch(res){
		 case 0:{
					DebugLevel = NODEBUG;break;}
		 case 1:{
					DebugLevel = LOGFILE;break;}
		 case 2:{
					DebugLevel = STDOUT;break;}
		 default:{
					DebugLevel = NODEBUG;break;}			
	    } 
	    DebugLevelComboBox->setCurrentItem(res);
	} 
	else
	    LoadScheme(1);
    }
    free(DefaultSchemeNum);
    fclose(fp);
    return 0;
}



//======================================================//
//заполнить список схем с перекодировками
void Configurations::Codes()
{
	 //puts("void Configurations::Codes()");
	 WarningTextLabel->setText("");
	 if (access(CODES_WAY,F_OK) == -1){
		  WarningTextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>Файл с перекодировками не найден!!!</b></u></font>"));
		  change = true;  
	 }
	 else
	 {
		 if ((access(CODES_WAY,4))==-1)
		  {
				dl->PrintString(DebugLevel,ERROR,"file couldn't be read  CODES_WAY\n");
				return;
		  }
		  /*int i = FileAcces(CODES_WAY);
		  printf("i = %d\n",i);
		  if (i!=0){
				QString er_str,er_id;
				DeviceConfErrorText(33, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				return;
		  }*/
		  char* PName = (char *) calloc (14, sizeof(char));
		  strcpy(PName, "PERECODIROVKA");
		  int MaxLen1 = 13;
		  int MaxResLen1 = 518;
		  char * Buf1 = (char *) calloc (MaxLen1+1, sizeof(char));  
		  char * Val1 = (char *) calloc(MaxResLen1+1, sizeof(char));
		  int i = 1;
		  while (GetCodesParam(PName, Val1, 518,i)==1)
		  {
				CDSPPerec -> insertItem(CP1251Codec->toUnicode(Val1),-1); 
				i++;
		  }
		  CDSPPerec->currentItem();
		  free(Buf1);
		  free(Val1);
		  free(PName);
	 } 
}


//======================================================//
//проверить настройки сканера с ручной настройкой
void Configurations::EditScannerTest()
{ 
	 dl->PrintString(DebugLevel,INFO,"void Configurations::EditScannerTest()\n"); 
	 QString er_str;
	 QString er_id;
	 if ( CheckPortNum() == -2)
	 {
		  DeviceConfErrorText(4,er_str,er_id);
		  InterfaceErrorWin(er_str, er_id);
		  return;
	 }
     
	 if (BCSPREF1->text() == NULL)
		  BCSPREF1->setText("0");
	 if (BCSPREF2->text()==NULL)
		  BCSPREF2->setText("0");   
	 if (BCSSUF1->text()==NULL)
		  BCSSUF1->setText("0");
	 if (BCSSUF2->text()==NULL)
		  BCSSUF2->setText("0");
	 st_p = new ScannerTest;
	 bool ok;
	 int Pref1 = BCSPREF1->text().toInt(&ok,10);
	 int Pref2 = BCSPREF2->text().toInt(&ok,10); 
	 int Suf1 = BCSSUF1->text().toInt(&ok,10);
	 int Suf2 = BCSSUF2->text().toInt(&ok,10);  
	 BaudRate = GetBaudRate(BCSBaudRate->currentItem());
	 int TypeBCS = CheckScannerType();
	 QString sss = DefCodec->fromUnicode(BCSPortNum->currentText());
	 int PortNum = 0;
	 switch(TypeBCS){
	 case 1:{
				PortNum = (sss.right(sss.length()-Com.length())).toInt();
				break;
		  }
	 case 3:{
				PortNum = (sss.right(sss.length()-UsbCom.length())).toInt();
				break;
		  }
	 case 4:{
				PortNum = (sss.right(sss.length()-Acm.length())).toInt();
				break;
		  }
	 default:{
				PortNum = 0;
				break;
		  }
	 }

	 st_p->InsertParam(TypeBCS, PortNum, BaudRate, Pref1, Pref2, Suf1, Suf2);
	 if (st_p->ErrNum!=0){
		  delete (st_p);	
		  return;
	 }
	 //puts("----exec----");
	 st_p -> exec();  
	 //если все преф и суф == 0
	 if (st_p->BarCodetextLabel->text()!="")
	 {
		  if (((BCSPREF1->text()=="0") || (BCSPREF1->text()=="")) &&  ((BCSPREF2->text()=="0") || (BCSPREF2->text()=="")) && ((BCSSUF1->text() == "0") || (BCSSUF1->text() == "")) && ((BCSSUF2->text()=="0") || (BCSSUF2->text()==""))) 
		  {
				BCSPREF1->setText(st_p->GetPar(1));
				BCSPREF2->setText(st_p->GetPar(2));  
				BCSSUF1->setText(st_p->GetPar(3));  
				BCSSUF2->setText(st_p->GetPar(4)); 
		  }
	 } 
	 delete (st_p);
	 releaseKeyboard();
}


//======================================================//
//проверить настройки ридера
void Configurations::EditMCRTest()
{
	 dl->PrintString(DebugLevel,INFO,"void Configurations::EditMCRTest()\n");      
	 QString er_str;
	 QString er_id;
	 if ( CheckPortNum() == -2)
	 {
		  DeviceConfErrorText(5,er_str,er_id);
		  InterfaceErrorWin(er_str, er_id);
		  return;
	 }
	 int TypeMCR = CheckMCRType();  
	 int PortNum = 0;
	 QString port = DefCodec->fromUnicode(MCRPortNum->currentText());
	 switch(TypeMCR){
	 case 1:{
				PortNum = (port.right(port.length()-Com.length())).toInt();
				break;
		  }
	 case 3:{
				PortNum = (port.right(port.length()-UsbCom.length())).toInt();
				break;
		  }
	 case 4:{
				PortNum = (port.right(port.length()-Acm.length())).toInt();
				break;
		  }
	 default:{
				PortNum = 0;
				break;
		  }
	 }
	 
	 mt_p = new MCRTest;
	 mt_p->Type = TypeMCR;
	 mt_p->PortNumber = PortNum;
	 printf("TypeMCR = %d \n", TypeMCR);
	 if (TypeMCR == 2)
	 { 
		  /*if (MCRPREF1->text() == NULL)
				MCRPREF1->setText("0");
		  if (MCRPREF2->text()==NULL)
				MCRPREF2->setText("0");   
		  if (MCRSUF1->text()==NULL)
				MCRSUF1->setText("0");
		  if (MCRSUF2->text()==NULL)
				MCRSUF2->setText("0");*/
		  //bool ok;
		  mt_p->Pref1 = MCRPREF1->text().toInt();
		  mt_p->Pref2 = MCRPREF2->text().toInt(); 
		  mt_p->Suf1 =  MCRSUF1->text().toInt();
		  mt_p->Suf2 =  MCRSUF2->text().toInt(); 
		  printf("  -- pref suf setted --  ");
		  mt_p->MCRPref1->setReadOnly(TRUE);
		  mt_p->MCRPref2->setReadOnly(TRUE);
		  mt_p->MCRSuf1->setReadOnly(TRUE);
		  mt_p->MCRSuf2->setReadOnly(TRUE);
		  mt_p->MyInitKlavaPort();
	 }
	 else
	 { 
		  if (PortNum == 0){
				delete(mt_p);
				return;
		  }
		  mt_p->BaudRate = GetBaudRate(MCRBaudRate->currentItem());
		  mt_p->MyInitRS232();
		  mt_p->groupBox18->hide();
		  mt_p->Save->hide();
		  if (mt_p->ErrNum != 0) 
		  {
				delete(mt_p);
				return;	
		  }
	 }
    mt_p->exec();
    if (mt_p->save_flag == true)
    {
         MCRPREF1->setText(mt_p->MCRPref1->text());
         MCRPREF2->setText(mt_p->MCRPref2->text());
         MCRSUF1->setText(mt_p->MCRSuf1->text());
         MCRSUF2->setText(mt_p->MCRSuf2->text());
    }
    releaseKeyboard();
    delete(mt_p);
}

//======================================================//
//мерцание строки сообщения о проверке настройки фискального регистратора
void Configurations::blink() 
{
    if (FRTesttextLabel->text().length()==0) 
    { 
	FRTesttextLabel->setText(CP1251Codec->toUnicode(errmes)); 
    }
    else 
    { 
	FRTesttextLabel->clear(); 
    }
    num_blink++;
    if (num_blink >= 7) 
	qtimer->stop();
}

//======================================================//
//таймер мерцания строки
void Configurations::Delay() 
{
    
    if (delay_flag == true)
    {
	qtimer->stop();
	delay_flag = false;
	Delay();
    }	
    else
    {
	if (num_blink != 0) 
	{
	    delete qtimer;
	}
	num_blink = 0;
	qtimer = new QTimer(this);
	connect(qtimer, SIGNAL(timeout()), SLOT(blink()));
	qtimer->start(500);
	blink();
    }
}

//======================================================//
//проверить настройки фискального регистратора
void Configurations::EditFRTest()
{
    dl->PrintString(DebugLevel,INFO,"void Configurations::EditFRTest()\n"); 
    FRTestButton->setEnabled(FALSE);
    QString er_str;
    QString er_id;
    if (CheckPortNum() == -2)
    {
	DeviceConfErrorText(6,er_str,er_id);
	InterfaceErrorWin(er_str, er_id);
	FRTestButton->setEnabled(TRUE);
	return;
    }
    QApplication::setOverrideCursor(waitCursor);
    int k = SetParamECR();
    if (k!=0)
    {
	hw->ECR_ReadError(k);
	errmes = QString(CP1251Codec->toUnicode(hw->ECR_GetErrNum()))+QString(CP1251Codec->toUnicode(hw->ECR_GetErrDescription()));
	FRTesttextLabel->setText(errmes);
    }
    else
    {
	qApp-> processEvents();
	FRTesttextLabel->clear();
	qApp-> processEvents();
	int res = hw->ECR_ReadStatus();
	int Protocol = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()), 1);
	if (Protocol == 5) 
	    res = hw->ECR_ReadStatus(); 
	if (res!=0)
	{
	    hw->ECR_ReadError(res);
	    errmes =QString( hw->ECR_GetErrNum())+QString(hw->ECR_GetErrDescription());
	    FRTesttextLabel->setText(errmes);
	    Delay();
	}
	else
	{
	    QString stringfortest = "SerialNumber = ";
	    stringfortest += hw->ECR_GetSerialNumber();
	    stringfortest += "  Description = ";
	    stringfortest += CP1251Codec->toUnicode(hw->ECR_GetUDescription());
	    errmes = stringfortest;
	    FRTesttextLabel->setText(errmes);
	}
    }
    dl->PrintString(DebugLevel,INFO,"void Configurations::EditFRTest() before hw->ECR_CloseConnection();\n"); 
    hw->ECR_CloseConnection();
    qApp-> processEvents();
    QApplication::restoreOverrideCursor();
    qApp-> processEvents(); 
    FRTestButton->setEnabled(TRUE); 
}
 

//======================================================//
//возвращает тип фискального регистратора
int Configurations::CheckFRType(void){
   int i = 0,k = 0;
   if (FRcheckBox -> isChecked()){
       QString PortNum = DefCodec->fromUnicode(FRPortNum->currentText());
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {
           i = 3;	
           return i;
       }    
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;	
           return i;
       }
       k = PortNum.find("ACM");
       if(k!=-1)
       {
           i = 4;	
           return i;
       }	
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }		
   }
   return i;
}  


//======================================================//
//возвращает тип платежной системы
int Configurations::CheckPSType(void){
   int i = 0,k = 0;
   if (PScheckBox -> isChecked()){
       QString PortNum = DefCodec->fromUnicode(PSPortVerifone->currentText());
       k = PortNum.find("USB<->COM");
        if(k!=-1)
       {
           i = 3;	
           return i;
       } 
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;	
           return i;
       }
       k = PortNum.find("ACM");
       if(k!=-1)
       {
           i = 4;	
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }		  
   }
   return i;
}  
    
//======================================================//
//возвращает тип дисплея
int Configurations::CheckDisplayType(){
   int i = 0,k = 0;
   if (CDSPcheckBox -> isChecked()){
       QString PortNum = DefCodec->fromUnicode(CDSPPortNum->currentText()); 
       k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           i = 2;
           return i;
       }
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {
           i = 3;	
           return i;
       }
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;	
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }
       k = PortNum.find("ACM");
       if (k!=-1){
           i = 4;
           return i;
       }
   }
   return i;
}

//======================================================//
//возвращает тип весов
int Configurations::CheckEWType(){
   int i = 0,k = 0;
   if (EWcheckBox -> isChecked()){
       QString PortNum = DefCodec->fromUnicode(EWPortNum->currentText());
       k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           i = 2;
           return i;
       }
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {
           i = 3;	
           return i;
       }
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;	
           return i;
       }
       k = PortNum.find("ACM");
       if (k!=-1){
           i = 4;
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }
   }
    return i;
}

//======================================================//

//проверяет правильную настройку суффиксов и префиксов сканера
int Configurations::CheckSufPrefScanner(){
   if (BCScheckBox -> isChecked()){
       QString er_str;
       QString er_id;
       bool ok;
       if (( BCSSUF1->text().isEmpty() && BCSSUF2->text().isEmpty()) || ((BCSSUF1->text().toInt(&ok,10) == 0) && (BCSSUF2->text().toInt(&ok,10) == 0)))    
       {
           if (show_message){
               DeviceConfErrorText(7,er_str,er_id);
               InterfaceErrorWin(er_str, er_id);
           }
           return -3;
      }
       QString PortNum = DefCodec->fromUnicode(BCSPortNum->currentText());
       int k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           if (( BCSPREF1->text().isEmpty() && BCSPREF2->text().isEmpty()) || ((BCSPREF1->text().toInt(&ok,10) == 0) && (BCSPREF2->text().toInt(&ok,10) == 0)))
           {
               if (show_message){
                   DeviceConfErrorText(8,er_str,er_id);
                   InterfaceErrorWin(er_str, er_id);
               }
               return -3;
           }
       }
    }
    return 0;
}


//======================================================//    
//проверяет тип сканера
int Configurations::CheckScannerType()
{  
    // puts("int Configurations::CheckScannerType()");
    QString er_str;
    QString er_id;
    int i = 0,k = 0;    
    if (BCScheckBox -> isChecked())
   {	
       QString PortNum = DefCodec->fromUnicode(BCSPortNum->currentText());
       k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           i = 2;
           return i;
       }
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {    
           i = 3;
           return i;
       }	
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;
           return i;
       }
       k = PortNum.find("ACM");
       if(k!=-1)
       {
           i = 4;
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }
   }
   return i;  
}



//======================================================//

//проверяет правильную настройку суффиксов и префиксов сканера
int Configurations::CheckSufPrefReader(){
    if (MCRcheckBox -> isChecked()){
       QString er_str;
       QString er_id;
       QString PortNum = DefCodec->fromUnicode(MCRPortNum->currentText()); 
       int k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           bool ok;
           if ((MCRSUF1->text().toInt(&ok,10) != 0) && (MCRSUF2->text().toInt(&ok,10) != 0) && (MCRPREF2->text().toInt(&ok,10) != 0) && (MCRPREF1->text().toInt(&ok,10) != 0))//все суффиксы и префиксы не являются 0
           { 
               if (( MCRSUF1->text().isEmpty() && MCRSUF2->text().isEmpty()) || ((MCRSUF1->text().toInt(&ok,10) == 0) && (MCRSUF2->text().toInt(&ok,10) == 0)))    
               {
                   if (show_message){
                       DeviceConfErrorText(7,er_str,er_id);
                       InterfaceErrorWin(er_str, er_id);
                   }
                   return -3;
               }
               if (( MCRPREF1->text().isEmpty() && MCRPREF2->text().isEmpty()) || ((MCRPREF1->text().toInt(&ok,10) == 0) && (MCRPREF2->text().toInt(&ok,10) == 0)))    
               {
                   if(show_message){
                      DeviceConfErrorText(8,er_str,er_id);
                      InterfaceErrorWin(er_str, er_id);
                   }
                    return -3;
               }
           }
       }
   } 
   return 0;
}



//======================================================//    
//проверяет тип ридера
int Configurations::CheckMCRType()
{
   int i = 0,k = 0;
   QString er_str;
   QString er_id;
   if (MCRcheckBox -> isChecked())
   {
       QString PortNum = DefCodec->fromUnicode(MCRPortNum->currentText()); 
       k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           i = 2;
           return i;
       } 
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {    
           i = 3;
           return i;
       }	
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;
           return i;
       }
       k = PortNum.find("ACM");
       if(k!=-1)
       {
           i = 4;
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }
   }
   return i;
}
//-----------Mirabell 16/12/11
//======================================================//    
//проверяет тип сканера
int Configurations::CheckBCSType()
{
   int i = 0,k = 0;
   QString er_str;
   QString er_id;
   if (BCScheckBox -> isChecked())
   {
       QString PortNum = DefCodec->fromUnicode(BCSPortNum->currentText()); 
       k = PortNum.find("клавиатурный");
       if (k!=-1)
       {
           i = 2;
           return i;
       } 
       k = PortNum.find("USB<->COM");
       if(k!=-1)
       {    
           i = 3;
           return i;
       }	
       k = PortNum.find("COM");
       if(k!=-1)
       {
           i = 1;
           return i;
       }
       k = PortNum.find("ACM");
       if(k!=-1)
       {
           i = 4;
           return i;
       }
       k = PortNum.find("USB");
       if (k!=-1){
           i = 5;
           return i;
       }
   }
   return i;
}
//----------------------------------

//======================================================//    
//при изменении порта сканера
void Configurations::BCSPortNumChanged()
{
    //-----------Mirabell 16/12/11
    int type = CheckBCSType();
    if (( type == 2) || (type == 3)){
	BCSInterSymbolTimeouttextLabel->setEnabled(TRUE);
	BCSInterSymbolTimeout->setEnabled(TRUE);
    }
    else{
	BCSInterSymbolTimeouttextLabel->setEnabled(FALSE);
	BCSInterSymbolTimeout->setEnabled(FALSE);
    }
    //----------------------------------
    if (type == 2)
    {
	BCSBaudRatetextLabel -> setEnabled(false);
	BCSBaudRate -> setEnabled(false);
    }
    else
    {
	BCSBaudRatetextLabel -> setEnabled(true);
	BCSBaudRate -> setEnabled(true);
    }  
}


//======================================================//    
//при изменении порта ридера
void Configurations::MCRPortNumChanged()
{
    if (MCRPortNum -> currentText() == DefCodec->toUnicode("клавиатурный"))
    {
	MCRBaudRatetextLabel -> setEnabled(false);
	MCRBaudRate -> setEnabled(false);
	MCRPREF1 -> setEnabled(true);  
	MCRPREF2 -> setEnabled(true);  
	MCRSUF1 -> setEnabled(true);  
	MCRSUF2 -> setEnabled(true);  
	MCRPref1textLabel -> setEnabled(true);  
	MCRPref2textLabel -> setEnabled(true);  
	MCRSuf1textLabel -> setEnabled(true);  
	MCRSuf2textLabel -> setEnabled(true);

	MCRPREF1 -> show();
	MCRPREF2 -> show();
	MCRSUF1 -> show();  
	MCRSUF2 -> show(); 
	MCRPref1textLabel -> show();
	MCRPref2textLabel -> show(); 
	MCRSuf1textLabel -> show(); 
	MCRSuf2textLabel -> show();
    }
    else
    {
	MCRBaudRatetextLabel -> setEnabled(true);
	MCRBaudRate -> setEnabled(true);
	MCRPREF1 -> hide();
	MCRPREF2 -> hide();
	MCRSUF1 -> hide();  
	MCRSUF2 -> hide(); 
	MCRPref1textLabel -> hide();
	MCRPref2textLabel -> hide(); 
	MCRSuf1textLabel -> hide(); 
	MCRSuf2textLabel -> hide();
    }  	
}


//======================================================//    
//проверить настройки дисплея покупателя
void Configurations::EditDisplayTest()
{  

	 QString er_str;
	 QString er_id;
	 //проверка занят ли текущий порт
	 if ( CheckPortNum() == -2)
	 {
		  DeviceConfErrorText(36,er_str,er_id); 
		  InterfaceErrorWin(er_str, er_id);
		  return;
	 }
	 //проверка найден ли файл с перекодировками
	 // printf("|%s|\n",WarningTextLabel->text().ascii());
	 QString s = WarningTextLabel->text();
	 if (!s.isEmpty())
	 {
		  DeviceConfErrorText(10,er_str,er_id);
		  InterfaceErrorWin(er_str, er_id);
		  return;	
	 }
	 //проверка не пустые ли строки пользователь пытается отправить на дисплей
	 if ((CDSP_DOWN->text().isEmpty()) && (CDSP_UP->text().isEmpty()))
	 {
		  DeviceConfMessageText(9,er_str,er_id);
		  InterfaceMessageWin(er_str, er_id);
		  return;	
	 }
	 hw->DSP_Stop();
	 //puts("+++++++++++++++++++++hw->DSP_Stop();");
	 int NumberProt = 0;
	 QString  ForProtokol = CDSPNumberProt->currentText();
	 if (ForProtokol == "Aedex")
	 {
		  NumberProt  = 1; 
	 }
	 if (ForProtokol == "CD5220")
	 {
		  NumberProt  = 2; 
	 }
	 if (ForProtokol == "DSP800")
	 {
		  NumberProt  = 4; 
	 }
	 if (ForProtokol == "Epson")
	 {
		  NumberProt  = 3; 
	 }
	 if (ForProtokol == "Promag Protocol")
	 {
		  NumberProt = 5;
	 }
    
	 int LogicalNumber = 1;
	 int Type = CheckDisplayType(); //получить тип дисплея 0 - выкл, 1 - com, 3-com-usb,5 - usb
	 int IdProd = 0;
	 int IdVend = 0;
	 
	 int PortNum = 0;
	 s = DefCodec->fromUnicode(CDSPPortNum->currentText());
	 switch(Type){
	 case 1:{
				PortNum = (s.right(s.length()-Com.length())).toInt();
				break;
		  }
	 case 3:{
				PortNum = (s.right(s.length()-UsbCom.length())).toInt();
				break;
		  }
	 case 4:{
				PortNum = (s.right(s.length()-Acm.length())).toInt();
				break;
		  }
	 case 5:{
				//заполнить IdProd и IdVend
				string cur_str =  CDSPPortNum->text(CDSPPortNum->currentItem());
				int usb_num =  cur_str.find(Usb.ascii());
				if (usb_num!=-1){
					 int pos = Usb.length();
					 string n = "";
					 while (cur_str.at(pos)!=32){
						  n += cur_str.at(pos);
						  pos++;
					 }
					 usb_num = atoi(n.c_str());
					 for (unsigned int i = 0; i<usb_struct.size();i++){
						  if (usb_struct[i].usbNum == usb_num){
								IdProd =   usb_struct[i].idProd;
								IdVend = usb_struct[i].idVend;
						  }
					 }
				}
				break;
		  }
	 }
	 
	 int NumberPerecod = CDSPPerec->currentItem()+1;
	 int CodeTable = 1;
	 QString upstr;
	 QString downstr;
	 char * upvalue;
	 char * downvalue;
    
	 if (CDSPPerec->currentText()=="Datecs DPD-201")
	 {
		  upstr = CP1251Codec->fromUnicode(CDSP_UP->text().lower());
		  upvalue = (char * )calloc((upstr.length()+1),sizeof(char));
		  strcpy(upvalue,upstr);
		  downstr = CP1251Codec->fromUnicode(CDSP_DOWN->text().lower());
		  downvalue = (char * )calloc((downstr.length()+1),sizeof(char));
		  strcpy(downvalue,downstr); 
	 }
	 else   
	 {
		  upstr = CP1251Codec->fromUnicode(CDSP_UP->text());
		  upvalue = (char * )calloc((upstr.length()+1),sizeof(char));
		  strcpy(upvalue,upstr);
		  downstr = CP1251Codec->fromUnicode(CDSP_DOWN->text());
		  downvalue = (char * )calloc((downstr.length()+1),sizeof(char));
		  strcpy(downvalue,downstr); 
	 }
	 
	 if (CDSP_DOWN->text()==NULL)
		  strcpy(downvalue,"");
	 if (CDSP_UP->text()==NULL)
		  strcpy(upvalue,""); 
	 int k = hw->DSP_SetConnectOptions(Type, LogicalNumber, IdProd, IdVend, PortNum,GetBaudRate(CDSPBaudRate->currentItem()),NumberPerecod,NumberProt, CodeTable, CDSPScrollSpeed->currentItem()+1, CDSPLengthStr->currentItem()+1, CDSPBrightness->currentItem()+1, CDSP_UP_Align_comboBox->currentItem()+1, CDSP_UP_ScrollMode_checkBox->isChecked(), CDSP_DOWN_Align_comboBox->currentItem()+1,CDSP_DOWN_ScrollMode_checkBox->isChecked(),(const char *) upvalue,(const char *) downvalue);
	 switch(k){
	 case -1:{
				DeviceConfErrorText(14,er_str,er_id);
				InterfaceErrorWin(er_str, er_id);
				break;
		  }
	 case -2:{
				DeviceConfErrorText(33,er_str,er_id);
		      	InterfaceErrorWin(er_str, er_id);
				RemoveStopScrollFlag();
				break;
		  }
	 case -3:{
				DeviceConfErrorText(34,er_str,er_id);
				InterfaceErrorWin(er_str, er_id);
				RemoveStopScrollFlag();
				break;
		  }
	 case -4:{
				DeviceConfErrorText(35,er_str,er_id);
				InterfaceErrorWin(er_str, er_id);
				RemoveStopScrollFlag();
				break;
		  }
	 case 0:{
				hw->DSP_AdjustBrightness(CDSPBrightness->currentItem()+1);
				hw->DSP_Up((const char *)upvalue);
				hw->DSP_Down((const char *)downvalue);
				break;
		  } 
	 }
	 free(upvalue);
	 free(downvalue);
}

//дает пользователю возможность программного удаления файла остановки бегущих строк дисплея
//=======================================================//
void Configurations::RemoveStopScrollFlag(){
	 int t = QMessageBox::information( this, DefCodec->toUnicode("Cообщение"), DefCodec->toUnicode("Для исправления данной ошибки можно удалить файл остановки бегущих строк дисплея.\nУдалить?"),DefCodec->toUnicode("Да"),DefCodec->toUnicode("Нет"),0,0);
	 if (t==0)
	 {
		  QFile file(SCROLL_WAY);
		  file.remove();
	 }      
}


//======================================================//
// проверяет имя схема на правильность, для задания имени по умолчанию
// newname - проверяемое имя
// num - номер проверяемого имени, при переименовании схем (для нового имени == -1) 
bool Configurations::validSchemeName(QString newname, int num)
{
    if ( newname.isNull()  || newname.isEmpty() ) return false;
    if ( SchemeName->count() == 0) return true;
    
    int i = 0;
    if ( num == 0 )  i = 1;
    while (   i < SchemeName->count() ) {
	if (newname == SchemeName->text(i) ) break;
	i++; if (i==num) i++; 
    }
    return  ( i == SchemeName->count() );
}

//======================================================//
//возвращает имя схемы по умолчанию
QString Configurations::getDefaultSchemeName()
{
    QString defaultname;
    int i = 0;
    while (  ! validSchemeName (defaultname, -1 ) ) {
	i++;
	defaultname = QString("Scheme");
	defaultname.append(QString::number(i));  
    }
    return defaultname;
}

//======================================================//
//создать новую схему
int Configurations::New()
{
	 dl->PrintString(DebugLevel,INFO,"int Configurations::New()\n");
	 QString er_str;
	 QString er_id;
	 char *NS;
	 FILE *fp;
	 unsigned int i;
	 bool flag = true;
	 QString ss;
	 char * sn;
	 unsigned int k=0;
	 int n = 1; 
	 
	 while (1) 
	 { 
		  sn = (char*) calloc (100,sizeof(char));
		  strcpy(sn,"Scheme ");
		  if ((SchemeName -> count()+1+k) < 10){
				*(sn+(strlen(sn)+1)) = SchemeName->count()+1+k+48;
		  }
		  else
		  {
				QString s;
				char * s1 = (char*) calloc (4,sizeof(char));
				s.setNum(SchemeName->count()+1+k,10);
				strcpy(s1,s);
				for (unsigned int i = 0; i<strlen(s1);i++) 
					 *(sn+7+i) = *(s1+i);
				free(s1); 
		  }
		  ss = QString(sn); 
		  n = CheckSchemeName(ss,2); 	   
		  if ( n == 0)
		  {
				break;  	   
		  }
		  if ( n == -3)
		  {	
				DeviceConfErrorText(2, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				return -1;
		  }
		  free(sn);
		  k++;
	 }
	 while (1==1)
	 {
	
		  DialogNewScheme * nscheme = new DialogNewScheme();
		  QString s = getDefaultSchemeName();
		  char * sname = (char *)calloc(100, sizeof(char));
		  strcpy(sname, s.ascii());
		  nscheme->setNewSchemeName(sname);
		  if (nscheme->exec() != QDialog::Accepted)
		  { 
				//Пользователь нажал отмену!!!
				if (conf_file_exist == true)
				{
					 free(sname);
					 return -2; 
				}
				else
				{
					 DeviceConfMessageText(17, er_str, er_id);
					 InterfaceMessageWin(er_str, er_id);
					 conf_file_exist = true;
				}
		  }
		  else
		  {
				QString scheme_name = CP1251Codec->fromUnicode(DefCodec->toUnicode(nscheme->getSchemeName()));
				strcpy(sname, scheme_name.ascii());
				nscheme->close();
				delete nscheme;	
		  }
		  //проверка существует ли схема с таким именем
		  if (SchemeName->count()==0)
		  {
				flag = true;
		  }
		  //printf("ВВЕДЕНО ИМЯ СХЕМЫ : %s\n", sname.ascii());
		  flag = true;
		  for (int h = 0;h<(SchemeName->count());h++)
		  {
				QString str = CP1251Codec->fromUnicode(SchemeName->text(h));
				if  (strcmp(sname,str) == 0)		   
				{
					 DeviceConfErrorText(13, er_str, er_id);
					 InterfaceErrorWin(er_str, er_id);
					 flag = false;
					 break;
				}     
		  }   
		  if (flag == true)
		  {
				//printf("sname = <%s>\n",sname);
				SchemeName->setCurrentText(sname);
				strcpy(sn,sname);
				fp = fopen(way,"r+");
				if (fp == NULL)
				{
					 return -2;
				} 
				else
				{
					 if ((access(way,4))==-1) 
					 {return -1;}
				}
				if (SchemeName->count() <= 1)
				{ 
					 DefaultScheme->setChecked(true); 
					 SetDefaultScheme(fp);  
				}
				fseek(fp,0,2);  
				NS = (char*) calloc(99,sizeof(char));   
				strcpy(NS,"[SCHEME] ");
				*(NS+strlen(NS)) = 94; 
				strcat(NS,sn);  
				*(NS+(strlen(NS))) = 94;
				fprintf(fp,"\n");
				fprintf(fp,NS);
				free(NS);
				fprintf(fp,"\n");
				fprintf(fp,"#BarCodeScanner section\n");
				fprintf(fp,"[BCSTYPE] %d\n",0);
				fprintf(fp,"[BCSLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[BCSPORTNUM] %s\n","00"); 
				fprintf(fp,"[BCSBAUDRATE] %s\n","00");
				fprintf(fp,"[BCSSufPref] %d\n",0);
				fprintf(fp,"[BCSPREF1] %s\n","000");
				fprintf(fp,"[BCSPREF2] %s\n"," 000");
				fprintf(fp,"[BCSSUF1] %s\n","000"); 
				fprintf(fp,"[BCSSUF2] %s\n","000");
				 //-----------Mirabell 16/12/11
				fprintf(fp,"[BCSINTERSYMBOLTIMEOUT] %d\n",200);
				//---------------------------------
				fprintf(fp,"#Elecrtonic Weighter section\n");  
				fprintf(fp,"[EWTYPE] %d\n",0);
				fprintf(fp,"[EWLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[EWPORTNUM] %s\n","00"); 
				fprintf(fp,"[EWBAUDRATE] %s\n","00");  
				fprintf(fp,"[EWMODEL] %d\n",1);	
				fprintf(fp,"#Customer Display section\n");  
				fprintf(fp,"[CDSPTYPE] %d\n",0);
				fprintf(fp,"[CDSPLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[CDSPIDVENDOR] %d\n",0);
				fprintf(fp,"[CDSPIDPRODUCT] %d\n",0);
				fprintf(fp,"[CDSPPORTNUM] %s\n","00");  
				fprintf(fp,"[CDSPBAUDRATE] %s\n","00"); 
				fprintf(fp,"[CDSPPERECODIROVKA] %d\n",1);   
				fprintf(fp,"[CDSPNUMBERPROT] %d\n",1);    
				fprintf(fp,"[CDSPBRIGHTNESS] %d\n",1);
				fprintf(fp,"[CDSPSCROLLSPEED] %d\n",1);
				fprintf(fp,"[CDSPLENGTHSTR] %d\n", 20);
				fprintf(fp,"[DEMONUPSTRING] ^%s^\n"," ");
				fprintf(fp,"[DEMONDOWNSTRING] ^%s^\n"," ");
				fprintf(fp,"[DEMONUPALIGN] %d\n",0);
				fprintf(fp,"[DEMONDOWNALIGN] %d\n",0);
				fprintf(fp,"[DEMONUPSCROLLMODE] %d\n",0);
				fprintf(fp,"[DEMONDOWNSCROLLMODE] %d\n",0);
				fprintf(fp,"#FR section\n");
				fprintf(fp,"[ECRTYPE] %d\n",0); 
				fprintf(fp,"[ECRLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[ECRPORTNUM] %s\n","00");  
				fprintf(fp,"[ECRBAUDRATE] %s\n","00");  
				fprintf(fp,"[ECRTYPEDEVICE] %d\n",0); 
				fprintf(fp,"[ECRMODEL] %d\n",1);
				fprintf(fp,"[USEMONEYBOX] %d\n",0);
				fprintf(fp,"[ECRUSEACCESSPASSWORD] %d\n",0);   
				NS = (char*)calloc(27,sizeof(char)); 
				strcpy(NS,"[ECRACCESSPASSWORD] "); //  maxlen == 4
				*(NS+20) = 94;
				for (i=0;i<4;i++)
					 *(NS+21+i) = 48;
				*(NS+ strlen(NS)) = 94;
				fprintf(fp,NS);
				fprintf(fp,"\n");
				free(NS);
				NS = (char*)calloc(27,sizeof(char)); 
				strcpy(NS,"[ECRPASSWORD] "); // maxlen == 8
				*(NS+14) = 94;
				for (i=0;i<8;i++)
					 *(NS+15+i) = 48;
				*(NS+ strlen(NS)) = 94;
				fprintf(fp,NS);
				fprintf(fp,"\n");
				free(NS);     	 
				fprintf(fp,"[ECRMBOXCNL] %d\n",2);
				fprintf(fp,"[ECRMBOXI1] %d\n",1);
				fprintf(fp,"[ECRMBOXI2] %d\n",1);
				fprintf(fp,"[ECRMBOXICNT] %d\n",1);
				fprintf(fp,"#MagneticCardReader section\n");   
				fprintf(fp,"[MCRTYPE] %d\n",0);   
				fprintf(fp,"[MCRLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[MCRPORTNUM] %s\n","00"); 
				fprintf(fp,"[MCRBAUDRATE] %s\n","00");
				fprintf(fp,"[MCRPREF1] %s\n","000");
				fprintf(fp,"[MCRPREF2] %s\n","000");
				fprintf(fp,"[MCRSUF1] %s\n","000");
				fprintf(fp,"[MCRSUF2] %s\n","000"); 
				fprintf(fp,"#Payment System  section\n");
				fprintf(fp,"[PSTYPE] %d\n",0);
				fprintf(fp,"[PSLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[PSTYPESYSTEM] %d\n",0);
				fprintf(fp,"[PSTYPESCHEME] %d\n",0);
				fprintf(fp,"[PSTYPEEXCHANGE] 0\n");
				fprintf(fp,"[PSTERMINALID] ^000000^\n");
				fprintf(fp,"[PSTIMEOUT1] 45\n");
				fprintf(fp,"[PSTIMEOUT2] 45\n");
				fprintf(fp,"[PSTIMEOUT3] 3\n");
				fprintf(fp,"[PSNAMEDIR] ^ ^\n");
				fprintf(fp,"[PSPORTNUM] 0\n");	
				fprintf(fp,"[PSBAUDRATEVER] 00\n");	
				fprintf(fp,"[PSIPADDRESS] ^000.000.000.000^\n");
				fprintf(fp,"[PSIPPORT] ^00000^\n");
				fprintf(fp,"[PSIPADDRESSVER] ^000.000.000.000^\n");
				fprintf(fp,"[PSIPPORTVER] ^00000^\n");
				fprintf(fp,"[PSPRINTCANSELCHECK] %d\n",0);
				fprintf(fp,"[PSBANK] ^ ^\n");
				fprintf(fp,"[PSTRADENAME] ^ ^\n");
				fprintf(fp,"[PSTOWNTRADE] ^ ^\n");
				fprintf(fp,"[PSADDRESSTRADE] ^ ^\n");
				fprintf(fp,"[PSCARDMASK] ^************^\n");
				fprintf(fp,"[PSENCRYPTIONANSWER] 0\n");
				//-----(+)-----Zuskin-----24-06-2011-----
				fprintf(fp,"#External Discount System  section\n");
				fprintf(fp,"[EDSTYPE] %d\n",0);
				fprintf(fp,"[EDSLOGICALNUMBER] %d\n",1);
				fprintf(fp,"[EDSIPADDR] ^%s^\n","127.0.0.1");
				fprintf(fp,"[EDSPORT] ^%s^\n","10065");				
				fprintf(fp,"\n"); 
				fclose(fp);
				SchemeName->insertItem(CP1251Codec->toUnicode(sname),-1);  
				SchemeName->setCurrentItem(SchemeName->count()-1); 
				LoadScheme(SchemeName->count()); 
				DeleteButton->setEnabled(TRUE); 
				SaveButton->setEnabled(TRUE); 
				NewButton->setEnabled(FALSE); 
				FRTesttextLabel->setEnabled(FALSE); 
				FRTesttextLabel->setText(""); 
				SchemeName->setEnabled(FALSE); 
				free(sn); 
				return 1; 
		  }
	 }	
}


//======================================================//
//создать новую схему перекодировки
void Configurations::NewPerecCreate_clicked()
{
    p = CDSPPerec->currentItem();
    this->hide();
    n_p = new NewPerecodirovka;
    n_p -> exec();
    delete n_p;
    CDSPPerec->clear();
    Codes();
   CDSPPerec->setCurrentItem(p1);
}


//======================================================//
//создать список com-портов
int Configurations::InsertComPort()
{
   BCSPortNum->insertItem(DefCodec->toUnicode("клавиатурный"),-1);
   MCRPortNum->insertItem(DefCodec->toUnicode("клавиатурный"),-1);
   int h = CreateListComPort();
   if ( h== 1)
   {
      FILE * fc;
      if ((access("/tmp/ports.list",0))==-1)
      {
           return -1;
       }
       if ((access("/tmp/ports.list",4))==-1)
       {
           return -1;
       }
       fc = fopen("/tmp/ports.list","r");
       if (fc == NULL)
           return -1;
       char * line = (char *)calloc(100, sizeof(char));
       while (((line = fgets(line,100,fc))!=NULL) && strcmp(line,"\n")!=0) 
       {
           QString COM = "";
           QString TTYS = "ttyS";
           QString TTYUSB = "ttyUSB";
           QString TTYACM = "ttyACM";
           QString l = line;
           l.remove(l.right(1));
           QString m = "";
           int i = l.find(TTYS);
           if (i!=-1){
               m =l.right(i+l.length()-TTYS.length());
               COM = Com;
               i = m.toInt()+1;
               COM +=m.setNum(i);
           }
           i = l.find(TTYUSB);
           if (i!=-1){
               m =l.right(i+l.length()-TTYUSB.length());
               COM = UsbCom;
               i = m.toInt()+1;
               COM +=m.setNum(i);
           }
           i = l.find(TTYACM);
           if (i!=-1){
               m =l.right(i+l.length()-TTYUSB.length());
               COM = Acm;
               i = m.toInt()+1;
               COM +=m.setNum(i);
           }
           BCSPortNum->insertItem(COM,-1);
           EWPortNum->insertItem(COM,-1);
           FRPortNum->insertItem(COM,-1);
           CDSPPortNum->insertItem(COM,-1);
           MCRPortNum->insertItem(COM,-1);
           PSPortVerifone->insertItem(COM,-1);
        }
         fclose(fc);
         free(line);
   }
   hw->InsertUsbStruct();
   InsertUsbDevices();	
   return 1;
}


//=====================================================//
//добавить список поддерживаемых usb-устройств

void Configurations::InsertUsbDevices()
{
	 int usb_num = 1;
	 for(unsigned int i  = 0; i<hw->usbDeviceSupport.size();i++){
	 
		  int idProdSupp = hw->usbDeviceSupport[i].idProduct;
		  //printf("idProdSupp = %d\n",idProdSupp);
		  int idVendSupp = hw->usbDeviceSupport[i].idVendor;
		 //printf("idVendSupp = %d\n",idVendSupp);
		  
		  for (unsigned int j = 0; j<hw->usbDevice.size();j++){
				int idProd = hw->usbDevice[j].idProduct;
				int idVend = hw->usbDevice[j].idVendor;
				if ((idProd == idProdSupp) && (idVend == idVendSupp)){
					// puts("найдено устройство, ктр мы поддерживаем!!!");
					 USBStruct usb_str = {usb_num,idProd,idVend};
					 usb_struct.push_back(usb_str);
					 string Product = hw->GetProductUsbDevice(idProd,idVend).substr(0,20);
					 int pos_last_probel = 0;
					 pos_last_probel = Product.find_last_of(" ");
					 if (pos_last_probel!=0){
						  Product = Product.substr(0,pos_last_probel);
					 }
					// printf("Product = |%s|\n",Product.c_str());
					 QString UsbMod = Usb;
					 UsbMod += itoa(usb_num);
					 UsbMod +=" ";
					 UsbMod +=Product.c_str();
					 CDSPPortNum->insertItem(UsbMod,-1);
					 usb_num++;
				}
		  }
	 }
}


//======================================================//
//при изменении параметров на форме изменяет флаг изменений
void Configurations::ParamChanged()
{
      changeflag = true;
	 Manage();
	 if (!FlagInLoading)
		  CheckRights(true);
	 
}


//======================================================//
//удаляет пустые строки из файла с настройками оборудования
void Configurations::DeleteEmptyString()
{
	 FILE * fk;
	 FILE * ft;
	 fk = fopen(CONF_WAY,"r");
	 if (fk == NULL) return;
	 ft = fopen(TMP_WAY,"w");
	 if (ft == NULL) return;
	 char * str =(char *) calloc(100,sizeof(char));
	 char * strbeforestr =(char *) calloc(100,sizeof(char));
	 strcpy(strbeforestr,"");
	 int probel = 0;
	 while (fgets(str,100,fk)!=NULL)
	 {
		  if ((strcmp(str,"\n")==0) && (strcmp(strbeforestr,"\n")==0))
		  {
				probel++;   
		  }
		  if ((strcmp(str,"\n") !=0) && (strcmp(strbeforestr,"\n")==0))
		  {
				probel=0;   
		  }
		  if (probel<3)
		  {
				fprintf(ft,str);
		  }
		  strcpy(strbeforestr,str);
	 }
	 fclose(ft);
	 fclose(fk);
	 strcpy(str,"");
	 fk = fopen(CONF_WAY,"w");
	 if (fk == NULL){ 
		  free(str);
		  free(strbeforestr);
		  return;
	 }
	 ft = fopen(TMP_WAY,"r");
	 if (ft == NULL){ 
		  free(str);
		  free(strbeforestr);
		  return;
	 }
	 while (fgets(str,100,ft)!=NULL)
	 {
		  fprintf(fk,str);
	 }
	 fclose(ft);
	 fclose(fk);
	 free(str);
	 free(strbeforestr);
}


//======================================================//
//при изменении протокола фискального регистратора
void Configurations::FRModelChanged()
{
    dl->PrintString(DebugLevel,INFO,"FRModelChanged()\n");
    if (FRcheckBox->isChecked()==TRUE)
   {	
       //-----(+)-----Zuskin-----21/10/2011-----
       FRBaudRate->setEnabled(TRUE);
       //-------------------------------------------------	
       // puts("FRcheckBox->isChecked()==TRUE");
       int Protocol = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()), 1);
       int DeviceType = FRTypeDevice->currentItem();
       if (DeviceType == 0){//FR
           switch(Protocol){
	case 6: { // Касби-02К
	}
	case 7: { // Касби-04К
	}
	case 8: {  // ОРИОН-ФР01К
	    //-----(+)-----Zuskin-----18/10/2011-----
	    UseAccessPassword->setEnabled(FALSE);
	    AccessPassword->setEnabled(FALSE);
	    AccessPasswordtextLabel->setEnabled(FALSE);
	    UseAccessPassword->setChecked(FALSE);
	    AccessPassword->setText("");  
	    //-------------------------------------------------
	    FRBaudRate->setCurrentItem(3);
	    FRBaudRate->setEnabled(FALSE);
	    break;
	}
	case 1:{
	    // dl->PrintString(DebugLevel,INFO,"--------ШТРИХ-------\n");
	    UseAccessPassword->setEnabled(FALSE);
	    AccessPassword->setEnabled(FALSE);
	   AccessPasswordtextLabel->setEnabled(FALSE);
	   UseAccessPassword->setChecked(FALSE);
	   AccessPassword->setText("");  
	   break; 
	}
	case 2:{}
	case 3:{}
	case 4:{
	    // dl->PrintString(DebugLevel,INFO,"--------АТОЛ-----CASBY----МЕРКУРИЙ-----\n");
	    UseAccessPassword->setEnabled(TRUE);
	    if (UseAccessPassword->isChecked()){
	        AccessPassword->setEnabled(TRUE);
	        if (AccessPassword->text().isEmpty())
	             AccessPassword->setText("0000");
	        AccessPasswordtextLabel->setEnabled(TRUE);
	    }
	    else{
	        AccessPassword->setEnabled(FALSE);
	        if (AccessPassword->text().isEmpty())
                            AccessPassword->setText("0000");
	        AccessPasswordtextLabel->setEnabled(FALSE);
	    }
	    break;
	}
	case 5:{
	    // dl->PrintString(DebugLevel,INFO,"--------ПРИМ------\n");
	    UseAccessPassword->setEnabled(TRUE);
	     if (UseAccessPassword->isChecked()){
	        AccessPassword->setEnabled(TRUE);
	        AccessPasswordtextLabel->setEnabled(TRUE);
	      }
	      else{
	        AccessPassword->setEnabled(FALSE);
	        AccessPasswordtextLabel->setEnabled(FALSE);
	     }
	      break;	  
	 }
	default:{ 
	FRProtocol->setCurrentItem(0);
	UseAccessPassword->setEnabled(FALSE);
	AccessPassword->setEnabled(FALSE);
	AccessPasswordtextLabel->setEnabled(FALSE);
	UseAccessPassword->setChecked(FALSE);
	AccessPassword->setText("");   
	break;
	}
             }
           }
           else{//PrinterCheck	
        }  
    }
}

//======================================================//
//при изменении типа обмена платежной системы
void Configurations::PSModelChanged()
{
	//puts("PSModelChanged");
	 if (PSTypeExchange->currentItem() == 0){
		  //файловый
		  InpasWidgetStack->raiseWidget(0);
		  PSTimeout1_textLabel->setText(DefCodec->toUnicode("Таймаут удаления файла запроса к серверу:"));
		  PSTimeout2_textLabel->setText(DefCodec->toUnicode("Таймаут появления файла ответа от сервера:"));
		  PSTimeout3_textLabel->setText(DefCodec->toUnicode("Таймаут между получением байтов:"));
	 }
	 else{
		  //TCP/IP
		  InpasWidgetStack->raiseWidget(1);
		  PSTimeout1_textLabel->setText(DefCodec->toUnicode("Минимальный таймаут ожидания ответа на запрос кассира:"));
		  PSTimeout2_textLabel->setText(DefCodec->toUnicode("Таймаут получения ответа от пин-пада на запрос данных:"));
		  PSTimeout3_textLabel->setText(DefCodec->toUnicode("Таймаут между получением байтов:"));
	 }
}	


//======================================================//
//очищает префиксы и суффиксы сканера
void Configurations::ClearPrefSuf_pushButton_clicked()
{
    BCSPREF1->setText("");
    BCSPREF2->setText("");
    BCSSUF1->setText("");
    BCSSUF2->setText("");
    ScannerTestButton->setFocus();
}

//======================================================//
//при получении сигнала от клавиатурного устройства
void Configurations::keyPressEvent(QKeyEvent * k)
{	
	 //dl->PrintString(DebugLevel,INFO,"k->text() = %c\n",k->key());
	 QFont f1;
	 QFont f2;
	 f1.setBold(true);
	 f2.setBold(false);
	 switch(k->key())   
	 {
	 case Qt::Key_F2:{
		  Save(); break;
	 }
	 case Qt::Key_F7:{
		  New(); break;
	 }
	 case Qt::Key_F8:{
		  Delete(); break;
	 }
	 case Qt::Key_Enter :{
		if ((BCScheckBox->hasFocus()) && (BCScheckBox->isChecked()))
		{
			 BCScheckBox->setChecked(FALSE);break;
		}
		if ((BCScheckBox->hasFocus()) && (!BCScheckBox->isChecked()))
		{  
			 BCScheckBox->setChecked(TRUE);break;
		}
		if ((CDSPcheckBox->hasFocus()) && (CDSPcheckBox->isChecked()))
		{  
			 CDSPcheckBox->setChecked(FALSE);break;
		}
		if ((CDSPcheckBox->hasFocus()) && (!CDSPcheckBox->isChecked()))
		{
			 CDSPcheckBox->setChecked(TRUE);break;
		}
		if ((FRcheckBox->hasFocus()) && (FRcheckBox->isChecked()))
		{  
			 FRcheckBox->setChecked(FALSE);break;
		}
		if ((FRcheckBox->hasFocus()) && (!FRcheckBox->isChecked()))
		{ 
			 FRcheckBox->setChecked(TRUE);break;
		} 
		if ((MCRcheckBox->hasFocus()) && (MCRcheckBox->isChecked()))
		{  
			 MCRcheckBox->setChecked(FALSE);break;
		}
		if ((MCRcheckBox->hasFocus()) && (!MCRcheckBox->isChecked()))
		{  
			 MCRcheckBox->setChecked(TRUE); break;
		}
		if ((EWcheckBox->hasFocus()) && (EWcheckBox->isChecked()))
		{  
			 EWcheckBox->setChecked(FALSE);break;
		}
		if ((EWcheckBox->hasFocus()) && (!EWcheckBox->isChecked()))
		{  
			 EWcheckBox->setChecked(TRUE);break; 
		}
		if ((DefaultScheme->hasFocus()) && (DefaultScheme->isChecked()))
		{  
			 DefaultScheme->setChecked(FALSE);break;
		}
		if ((DefaultScheme->hasFocus()) && (!DefaultScheme->isChecked()))
		{  
			 DefaultScheme->setChecked(TRUE); break;
		}
		if ((UseMoneyBox->hasFocus()) && (UseMoneyBox->isChecked()))
		{  
			 UseMoneyBox->setChecked(FALSE);break;
		}
		if ((UseMoneyBox->hasFocus()) && (!UseMoneyBox->isChecked()))
		{   
			 UseMoneyBox->setChecked(TRUE);break;
		}
		if ((UseAccessPassword->hasFocus()) && (UseAccessPassword->isChecked()))
		{   
			 UseAccessPassword->setChecked(FALSE);
			 break;
		}
		if ((UseAccessPassword->hasFocus()) && (!UseAccessPassword->isChecked()))
		{   
			 UseAccessPassword->setChecked(TRUE);
			 break;
		}
		break;
	}
	 case Qt::Key_Return :{
		if ((BCScheckBox->hasFocus()) && (BCScheckBox->isChecked()))
		{
			 BCScheckBox->setChecked(FALSE);break;
		}
		if ((BCScheckBox->hasFocus()) && (!BCScheckBox->isChecked()))
		{
			 BCScheckBox->setChecked(TRUE);break;
		}
		if ((CDSPcheckBox->hasFocus()) && (CDSPcheckBox->isChecked()))
		{ 
			 CDSPcheckBox->setChecked(FALSE);break;
		}
		if ((CDSPcheckBox->hasFocus()) && (!CDSPcheckBox->isChecked()))
		{
			 CDSPcheckBox->setChecked(TRUE);break;
		}
		if ((FRcheckBox->hasFocus()) && (FRcheckBox->isChecked()))
		{  
			 FRcheckBox->setChecked(FALSE);break;
		}
		if ((FRcheckBox->hasFocus()) && (!FRcheckBox->isChecked()))
		{   
			 FRcheckBox->setChecked(TRUE); break;  
		}
		if ((MCRcheckBox->hasFocus()) && (MCRcheckBox->isChecked()))
		{ 
			 MCRcheckBox->setChecked(FALSE);break;
		}
		if ((MCRcheckBox->hasFocus()) && (!MCRcheckBox->isChecked()))
		{
			 MCRcheckBox->setChecked(TRUE); break;
		}
		if ((EWcheckBox->hasFocus()) && (EWcheckBox->isChecked()))
		{  
			 EWcheckBox->setChecked(FALSE);break;
		}
		if ((EWcheckBox->hasFocus()) && (!EWcheckBox->isChecked()))
		{ 
			 EWcheckBox->setChecked(TRUE); break;
		}
		if ((DefaultScheme->hasFocus()) && (DefaultScheme->isChecked()))
		{
			 DefaultScheme->setChecked(FALSE);break;
		}
		if ((DefaultScheme->hasFocus()) && (!DefaultScheme->isChecked()))
		{ 
			 DefaultScheme->setChecked(TRUE); break;
		}
		if ((UseMoneyBox->hasFocus()) && (UseMoneyBox->isChecked()))
		{  
			 UseMoneyBox->setChecked(FALSE);break;
		}
		if ((UseMoneyBox->hasFocus()) && (!UseMoneyBox->isChecked()))
		{
			 UseMoneyBox->setChecked(TRUE);break;
		}
		if ((UseAccessPassword->hasFocus()) && (UseAccessPassword->isChecked()))
		{
			 UseAccessPassword->setChecked(FALSE);break;
		}
		if ((UseAccessPassword->hasFocus()) && (!UseAccessPassword->isChecked()))
		{
			 UseAccessPassword->setChecked(TRUE); break;
		}
		break;
	}
	case Qt::Key_Left :
		break;
	 case Qt::Key_Right :
		break;	
	case Qt::Key_Tab :
		break;
	case Qt::Key_Up :{
		if (toolBox1->hasFocus())
		{
			 toolBox1->setCurrentIndex(toolBox1->currentIndex()-1);
		}
		if (toolBox2->hasFocus())
		{
			 toolBox2->setCurrentIndex(toolBox2->currentIndex()-1);
		}
		if (toolBox3->hasFocus())
		{
			 toolBox3->setCurrentIndex(toolBox3->currentIndex()-1);
		}
		break;
	}
	 case Qt::Key_Home:{
		 if (OpenMoneyBoxButton->hasFocus())
			  OpenMoneyBoxButton_clicked();
		break;
	}
	 case Qt::Key_Down :{
		if (toolBox1->hasFocus())
		{
			 toolBox1->setCurrentIndex(toolBox1->currentIndex()+1);
		}
		if (toolBox2->hasFocus())
		{
			 toolBox2->setCurrentIndex(toolBox2->currentIndex()+1);
		}
		if (toolBox3->hasFocus())
		{
			 toolBox3->setCurrentIndex(toolBox3->currentIndex()+1);
		}
		break;     
	 }
	case Qt::Key_F12 :{
		reject();	 
		close();
		break;
	}
	case Qt::Key_Escape :{
		close();
		reject();
		break;
	}
	case Qt::Key_Backspace :{
		if(BCSPREF1->hasFocus())
			 BCSPREF1->backspace();
		if(BCSPREF2->hasFocus())
			 BCSPREF2->backspace();
		if(BCSSUF1->hasFocus())
			 BCSSUF1->backspace();
		if(BCSSUF2->hasFocus())
			 BCSSUF2->backspace();
		if(CDSP_UP->hasFocus())
			 CDSP_UP->backspace();
		if(CDSP_DOWN->hasFocus())
			 CDSP_DOWN->backspace();
		if(AccessPassword->hasFocus())
			 AccessPassword->backspace();
		if(AdminPassword->hasFocus())
			 AdminPassword->backspace();
		if(MCRPREF1->hasFocus())
			 MCRPREF1->backspace();
		if(MCRPREF2->hasFocus())
			 MCRPREF2->backspace();
		if(MCRSUF1->hasFocus())
			 MCRSUF1->backspace();
		if(MCRSUF2->hasFocus())
			 MCRSUF2->backspace();
		break;
	 }
	default :{
		if(BCSPREF1->hasFocus())
			 BCSPREF1->insert(k->text());
		if(BCSPREF2->hasFocus())
			 BCSPREF2->insert(k->text());
		if(BCSSUF1->hasFocus())
			 BCSSUF1->insert(k->text());
		if(BCSSUF2->hasFocus())
			 BCSSUF2->insert(k->text());
		if(CDSP_UP->hasFocus())
			 CDSP_UP->insert(k->text());
		if(CDSP_DOWN->hasFocus())
			 CDSP_DOWN->insert(k->text());
		if(AccessPassword->hasFocus())
			 AccessPassword->insert(k->text());
		if(AdminPassword->hasFocus())
			 AdminPassword->insert(k->text());
		if(MCRPREF1->hasFocus())
			 MCRPREF1->insert(k->text());
		if(MCRPREF2->hasFocus())
			 MCRPREF2->insert(k->text());
		if(MCRSUF1->hasFocus())
			 MCRSUF1->insert(k->text());
		if(MCRSUF2->hasFocus())
			 MCRSUF2->insert(k->text());
		if (SchemeName->hasFocus())
		{
			 SchemeName->lineEdit()->insert(k->text());
		}
	}
        }
}

//======================================================//
//проверяет режимы автоматической прокрутки строк дисплея в завистимости от их длины
void Configurations:: CheckLengthStr(void)
{
	 QString er_str;
	 QString er_id;
	 int LengthStr = CDSPLengthStr->currentItem()+1;
	 if (CDSP_UP_ScrollMode_checkBox->isChecked())
	 {
		  if ((CDSP_UP->text().length())>LengthStr)
		  {
				DeviceConfErrorText(27, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);  
				//Верхняя строка не может принудительно прокручиваться!
				CDSP_UP_ScrollMode_checkBox->setChecked(FALSE);
		  }
	 }
	 if (CDSP_DOWN_ScrollMode_checkBox->isChecked())
	 {
		  if ((CDSP_DOWN->text().length())>LengthStr)
		  {
				DeviceConfErrorText(28, er_str, er_id);
				InterfaceErrorWin(er_str, er_id); 
				//Нижняя строка не может принудительно прокручиваться!
				CDSP_DOWN_ScrollMode_checkBox->setChecked(FALSE);
		  }
	 }
}

//======================================================//
//заполняет серийный номер продукта
int Configurations::GetSerialNumProd(void)
{
	// puts("int Configurations::GetSerialNumProd(void)");
	 int res = hw->FileAccess(SERIAL_WAY);
	 if (res == 0)
	 {
		  FILE * fp = fopen(SERIAL_WAY,"r");
		  if (fp == NULL){
				return -1;
		  }
		  char * str = (char *) calloc(100,sizeof(char));
		  if (fgets(str,100,fp) == NULL){
				free(str);
				return -4;
		  }
		  if (strcmp(str,"") == 0)
		  {
				//Считанное значение пустое
				free(str);
				return -4;
		  }
		  if (strlen(str)!=12)
		  {
				//длина считанного кода не равна 12
				free(str);
				return -5;
		  }
		  else
		  {
				for (unsigned int i = 0; i<strlen(str);i++)
				{
					 if ((*(str+i)>57)&&(*(str+i)<48))
					 {
						  //Не все символы являются цифрами!
						  free(str);
						  return -6;
					 }
				}
		  }
		  QString s = DefCodec->toUnicode("Серийный номер: ");
		  QString ss = str;
		  s +=ss.left(4);
		  s +="-";
		  s +=ss.mid(4,4);
		  s +="-";
		  s +=ss.right(4);
		  SerialNumProd_textLabel->setText(s);
		  free(str);
		  fclose(fp);
		  return 0;
	 }
	 else
		  return res;
}

//======================================================//
//проверяет ввод серийного номера
int Configurations::ManageSerial(void)
{
	//puts("int Configurations::ManageSerial(void)");
    int i = GetSerialNumProd();
    if (i!=0)
    {
	SerialNumProd_textLabel->setText(DefCodec->toUnicode("Серийный номер:  НЕ ВВЕДЕН"));
	InsertSerialNumProd->setEnabled(TRUE);
	KodActivization_textLabel->setEnabled(TRUE);
	KodActivization_lineEdit->setEnabled(TRUE);	    
	KeyActivization_textLabel->setEnabled(TRUE);
	InsertActivKod->setEnabled(FALSE);
	Message_TextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>ПРОДУКТ НЕ АКТИВИРОВАН</b></u></font>"));
    }
    return i;
}


//======================================================//
//заполняет ключ активации продукта
int Configurations::GetActivKeyProd(void)
{
	 int res = hw->FileAccess(ACTKEY_WAY);
	 if (res == 0)
	 {
		  FILE * fp = fopen(ACTKEY_WAY,"r+");
		  if (fp == NULL) return -1;
		  char * str = (char *) calloc(100,sizeof(char)); 
		  fgets(str,100,fp);
		  if (strcmp(str,"") == 0)
		  {
				//Считанное значение пустое
				free(str);
				return -4;
		  }	
		  if (strlen(str)!=16)
		  {
				//длина считанного кода не равна 16
				free(str);
				return -5;
		  }
		  else
		  {
				for (unsigned int i = 0; i<strlen(str);i++)
				{
					 if ((*(str+i)>57)&&(*(str+i)<48))
					 {
						  //Не все символы являются цифрами!
						  free(str);
						  return -6;
					 }
				}
		  }
		  free(str);
		  fclose(fp);
		  return 0;
	 }
	 else
		  return res;
}


//======================================================//
//проверяет  ввод ключа активации продукта
int Configurations::ManageActkey(void)
{
    int i = GetActivKeyProd();
    return i;
}


//======================================================//
//ввод пользователем серийного номера продукта
void Configurations::InsertSerialNumProd_clicked(void)
{
    dl->PrintString(DebugLevel,INFO,"void Configurations::InsertSerialNumProd_clicked(void)\n"); 
    InsertKod * i_k = new InsertKod;
    i_k->setCaption(DefCodec->toUnicode("Ввод серийного номера"));
    i_k->MyInit(1);
    i_k->exec();
    delete(i_k);
    Manage();
}

//======================================================//
//получить код активации продукта
QString Configurations::GetActivKod(void)
{
    char * str = (char *)calloc(17,sizeof(char));
    strcpy(str,hw->Get_ActiveKod(str));
    QString s = str; 
    free(str);
    return s;
}

//======================================================//
//ввод пользователем ключа активации продукта
void Configurations::InsertActivKod_clicked()
{
    hw->GenerateKod();
     InsertKod * i_k = new InsertKod;
    i_k->setCaption(DefCodec->toUnicode("Ввод ключа активации"));
    i_k->MyInit(2);
    i_k->exec();
    delete(i_k);
    Manage();
}

//======================================================//
//активировать продукт
void Configurations::Manage(void)
{
	 //puts("void Configurations::Manage(void)");
	 int m = hw->GenerateKod();
	 //printf("m = %d\n",m);
	 int i = ManageSerial();
	 if ( i== 0) 
	 {
		  QString l = GetActivKod();
		  if ( l !=NULL) 	
		  {
				//printf("GetActivKod return = %s\n",l.ascii());
				KodActivization_lineEdit->setText(l.left(4)+"-"+l.mid(4,4)+"-"+l.mid(8,4)+"-"+l.right(4));
				int j = ManageActkey();
				//printf("ManageActkey return = %d\n",j);
				if ( j ==0) 
				{
					 if (m == 1)
					 {
						  Message_TextLabel->setText(DefCodec->toUnicode("<font color=green><u><b>ПРОДУКТ АКТИВИРОВАН!</b></u></font>"));
						  Advert_textEdit->setText(DefCodec->toUnicode("<b>Замечания и предложения по улучшению продукта направляйте на адрес support@dnc-soft.ru</b>"));
						  KodActivization_textLabel->hide();
						  KodActivization_lineEdit->hide();
						  KeyActivization_textLabel->hide();
						  InsertActivKod->hide();
						  InsertSerialNumProd->hide();
						  UpdateKodActivation->show();
					 }
					 else
					 {
						  Message_TextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>ПРОДУКТ НЕ АКТИВИРОВАН!</b></u></font>"));
						  KodActivization_textLabel->show();
						  KodActivization_lineEdit->show();
						  KeyActivization_textLabel->show();
						  InsertActivKod->show();
						  InsertSerialNumProd->show();
						  UpdateKodActivation->hide();
						  KodActivization_lineEdit->setEnabled(TRUE);  
						  KodActivization_textLabel->setEnabled(TRUE);  
						  KeyActivization_textLabel->setEnabled(TRUE);  
						  InsertActivKod->setEnabled(TRUE);  
						  Advert_textEdit->setText(DefCodec->toUnicode("<b>Для получения серийного номера и ключа активации продукта направьте запрос на sales@dnc-soft.ru или позвоните по телефону (495)781-54-45</b>"));
						 if (PScheckBox->isChecked() && (!FlagInLoading)){
								PScheckBox->setChecked(false);
								show_message = false;
								Save();
								show_message = true;
						  }
					 }
				}
				else
				{
					 Message_TextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>ПРОДУКТ НЕ АКТИВИРОВАН!</b></u></font>"));
					 KodActivization_textLabel->show();
					 KodActivization_lineEdit->show();
					 KeyActivization_textLabel->show();
					 InsertActivKod->show();
					 InsertSerialNumProd->show();
					 UpdateKodActivation->hide(); 
					 KodActivization_lineEdit->setEnabled(TRUE);  
					 KodActivization_textLabel->setEnabled(TRUE);  
					 KeyActivization_textLabel->setEnabled(TRUE);  
					 InsertActivKod->setEnabled(TRUE);  
					 Advert_textEdit->setText(DefCodec->toUnicode("<b>Для получения серийного номера и ключа активации продукта направьте запрос на sales@dnc-soft.ru или позвоните по телефону (495)781-54-45</b>"));
					 if (PScheckBox->isChecked() && (!FlagInLoading)){
						  PScheckBox->setChecked(false);
						  show_message = false;
						  Save();
						  show_message = true;
					 }
				}	      
		  }
		  else
		  {
				Message_TextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>ПРОДУКТ НЕ АКТИВИРОВАН!</b></u></font>"));
				KodActivization_textLabel->show();
				KodActivization_lineEdit->show();
				KeyActivization_textLabel->show();
				InsertActivKod->show();
				InsertSerialNumProd->show();
				UpdateKodActivation->hide();
				KodActivization_lineEdit->setEnabled(TRUE);  
				KodActivization_textLabel->setEnabled(TRUE);  
				KeyActivization_textLabel->setEnabled(TRUE);  
				InsertActivKod->setEnabled(TRUE);  
				Advert_textEdit->setText(DefCodec->toUnicode("<b>Для получения серийного номера и ключа активации продукта направьте запрос на sales@dnc-soft.ru или позвоните по телефону (495)781-54-45</b>"));
				if (PScheckBox->isChecked() && (!FlagInLoading)){
					 PScheckBox->setChecked(false);
					 show_message = false;
					 Save();
					 show_message = true;
				}
		  }
	 }
	 else
	 {
		  Message_TextLabel->setText(DefCodec->toUnicode("<font color=red><u><b>ПРОДУКТ НЕ АКТИВИРОВАН!</b></u></font>"));
		  KodActivization_textLabel->show();
		  KodActivization_lineEdit->show();
		  KeyActivization_textLabel->show();
		  InsertActivKod->show();
		  InsertSerialNumProd->show();
		  UpdateKodActivation->hide();
		  KodActivization_lineEdit->setEnabled(TRUE);  
		  KodActivization_textLabel->setEnabled(TRUE);  
		  KeyActivization_textLabel->setEnabled(TRUE);  
		  InsertActivKod->setEnabled(FALSE);
		  Advert_textEdit->setText(DefCodec->toUnicode("<b>Для получения серийного номера и ключа активации продукта направьте запрос на sales@dnc-soft.ru или позвоните по телефону (495)781-54-45</b>"));
		  if (PScheckBox->isChecked() && (!FlagInLoading)){
				PScheckBox->setChecked(false);
				show_message = false;
				Save();
				show_message = true;
		  }
	 }
}

//======================================================//
//сохранить номер текущего документа для фискального регистратора в обучающем режиме
void Configurations::SaveCurrentDoc()
{
   QString n_doc = CurrentDocNum->text();
   if (!n_doc.isEmpty()){
       int n_d = n_doc.toInt();
       int er = hw->SetParam("CHECKNUMBER", 0, double(n_d));
       if(er == -1)
	   QMessageBox::information(this, DefCodec->toUnicode("Сообщение"), DefCodec->toUnicode("Не удалось сохранить номер документа!"));
       else{
	   QMessageBox::information(this, DefCodec->toUnicode("Сообщение"), DefCodec->toUnicode("Номер следующего документа успешно сохранен!"));
       }
   }
}

//======================================================//
//получить номер строки в комбобоксе по названию(для позиционирования на нужной строке при загрузке параметров из файла с настройками)
int Configurations::GetNumSearchingTextInComboBox(QString  str, int type)
{
	 int currentItemNum = 0;
	 QString s = "";
	 QString ss = str;
	 switch(type){
	 case 1:{
				//DSP
				for (currentItemNum = 0; currentItemNum<CDSPPortNum->count();currentItemNum++){
					 s = DefCodec->fromUnicode(CDSPPortNum->text(currentItemNum));
					 int i = s.compare(ss);
					 if ( i == 0){ return   currentItemNum;}
				}
				break;
		  }	  
	 case 2:{
				//BCS
				for (currentItemNum = 0; currentItemNum<BCSPortNum->count();currentItemNum++){
					 s = DefCodec->fromUnicode(BCSPortNum->text(currentItemNum));
					 int i = s.compare(ss);
					 if ( i == 0){ return   currentItemNum;}
				}
				break;
		  }
	 case 3:{
				//FR
				for (currentItemNum = 0; currentItemNum<FRPortNum->count();currentItemNum++){
					 s = FRPortNum->text(currentItemNum);
					 int i = s.compare(ss);
					 if ( i == 0){return   currentItemNum;}
				}
				break;
		  }						
	 case 4:{
				//MCR
				for (currentItemNum = 0; currentItemNum<MCRPortNum->count();currentItemNum++){
					 s = MCRPortNum->text(currentItemNum);
					 int i = s.compare(ss);
					 if ( i == 0){return   currentItemNum;}
				}
				break;
		  }
	 case 5:{
				//EW
				for (currentItemNum = 0; currentItemNum<EWPortNum->count();currentItemNum++){
					 s = EWPortNum->text(currentItemNum);
					 int i = s.compare(ss);
					 if ( i == 0){return   currentItemNum;}
				}
				break;
		  }
	 case 6:{
				//PAYMENT SYSTEM
				for (currentItemNum = 0; currentItemNum<PSPortVerifone->count();currentItemNum++){
					 s = PSPortVerifone->text(currentItemNum);
					 int i = s.compare(ss);
					 if ( i == 0){return   currentItemNum;}
				}
				break;
		  }
	 }
	 return -1;
}

//======================================================//
//при изменении протокола фискального регистратора при использовании денежного ящика
void Configurations::UseMoneyBoxMercurii(void)
{
	// puts("UseMoneyBoxMercurii");
	 bool Var = false;
	 if (UseMoneyBox->isChecked() && FRcheckBox->isChecked()){
		  Var = true;
	 } 
	 int ModelFR = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()),1);
	// int ModelFR = FRProtocol -> currentItem()+1;
	 switch(ModelFR){
	 case 3:{
				if (FRMBoxI1->text().toInt()>127) 
					 FRMBoxI1->setText("127");
				FRMBoxI1->setValidator(FRMBoxImp);
				FRMBoxCNL_textLabel->setEnabled(Var);
				FRMBoxCNL->setEnabled(Var);
				FRMBoxICnt_textLabel->setEnabled(Var);
				FRMBoxICnt->setEnabled(Var);	  
				FRMBoxI1_textLabel->setEnabled(Var);
				FRMBoxI1->setEnabled(Var);
				FRMBoxI2_textLabel->setEnabled(Var);
				FRMBoxI2->setEnabled(Var);
				OpenMoneyBoxButton->setEnabled(Var);
				break;
		  }
	 case 4:{
				FRMBoxI1->setValidator(FRMBoxIcnt);
				FRMBoxI1_textLabel->setEnabled(Var);
				FRMBoxI1->setEnabled(Var);
				FRMBoxCNL_textLabel->setEnabled(FALSE);
				FRMBoxCNL->setEnabled(FALSE);
				FRMBoxICnt_textLabel->setEnabled(FALSE);
				FRMBoxICnt->setEnabled(FALSE);
				FRMBoxI2_textLabel->setEnabled(FALSE);
				FRMBoxI2->setEnabled(FALSE);
				OpenMoneyBoxButton->setEnabled(Var);
				break;
		  }
	 default:{
				FRMBoxCNL_textLabel->setEnabled(FALSE);
				FRMBoxCNL->setEnabled(FALSE);
				FRMBoxICnt_textLabel->setEnabled(FALSE);
				FRMBoxICnt->setEnabled(FALSE);
				FRMBoxI1_textLabel->setEnabled(FALSE);
				FRMBoxI1->setEnabled(FALSE);
				FRMBoxI2_textLabel->setEnabled(FALSE);
				FRMBoxI2->setEnabled(FALSE);
				OpenMoneyBoxButton->setEnabled(Var);
		  }
	 }
}

//======================================================//
//при изменении канала в протоколе меркурий при использовании денежного ящика
void Configurations::FRMBoxCNLChanged(void){
	 //puts("FRMBoxCNLChanged");
	 QString name = DefCodec->fromUnicode(FRProtocol->currentText());
	// printf("name = %s\n",name.ascii());
	// int frprot = FRProtocol->currentItem(); 
	 int frprot = GetNumByNameAndType(name,1);
	 if (frprot == 2){
		  int Var = FRMBoxCNL->currentItem();
		  if ((Var == 0)&&(UseMoneyBox->isChecked())){
				FRMBoxI1_textLabel->setEnabled(TRUE);
				FRMBoxI1->setEnabled(TRUE);
				FRMBoxI2_textLabel->setEnabled(TRUE);
				FRMBoxI2->setEnabled(TRUE);
		  }
		  else{
				FRMBoxI1_textLabel->setEnabled(FALSE);
				FRMBoxI1->setEnabled(FALSE);
				FRMBoxI2_textLabel->setEnabled(FALSE);
				FRMBoxI2->setEnabled(FALSE);
		  }
	 }
	 FRTesttextLabel->clear();
}

//-----------Mirabell 19/12/11
//======================================================//
//проверка работы платежной системы(проводка транзакции с  суммой в 10 рублей)
void Configurations::TestPS(void)
{
   dl->PrintString(DebugLevel,INFO,"void Configurations::TestPS(void)\n");
   qApp->processEvents();
   PSMessage->setText(" ");
   qApp-> processEvents(); 		
   QString er_str;
   QString er_id;
   int TypeSystem = PSType->currentItem();
   hw->PS_SetTypeSystem(TypeSystem);
   if (TypeSystem == 2){//SBERBANK
      QString name =  PSNameDir->text();
      if (name.isEmpty()){
         PSMessage->setPaletteForegroundColor(QColor(255,0,0));
         PSMessage->setText(DefCodec->toUnicode("Не заполнена директория обмена!"));
         return;
      }
      else{
         if((access(name.ascii(),0))==-1){
            PSMessage->setPaletteForegroundColor(QColor(255,0,0));
            PSMessage->setText(DefCodec->toUnicode("Директория не существует!"));	
            return;
         }	          
         hw->PS_SetNameDirExchange(DefCodec->fromUnicode(name));
         hw->PS_GenerateFileNames(); 
      }
      Delay();
      PSMessage->clear();
      hw->SetEducationMode(1);//режим обучения
      hw->PS_SetCodeOper(1);//код операции - оплата
      hw->PS_SetSumm(1000);//установка суммы - в копейках
      int res = hw->PS_SendRequest();
      printf("res SendRequest SBERBANK = %d\n",res);
      if ( res == 0){
         PSMessage->setPaletteForegroundColor(QColor(0,0,0));
         PSMessage->setText(DefCodec->toUnicode("Транзакция выполнена"));
         //проверить, что без ошибок прошла транзакция и провести операцию аварийная отмена.
         int status = hw->PS_GetTransStatus();
         if (status == 1){
            qApp->processEvents();
            QApplication::restoreOverrideCursor();
            qApp-> processEvents(); 
				 
             int rres  = QMessageBox::information( this, DefCodec->toUnicode("Настройка оборудования"), DefCodec->toUnicode("Транзакция прошла успешно!\n Уберите карту из пин-пада\n Отменить проведенную транзакцию?"), DefCodec->toUnicode("&Да"), DefCodec->toUnicode("&Нет"), 0, 0 );
            QApplication::setOverrideCursor(waitCursor);
            switch(rres){
               case 0:{
                  hw->PS_SetCodeOper(7);
                  res = hw->PS_SendRequest();
                  if (res == 0){
                     PSMessage->setPaletteForegroundColor(QColor(0,0,0));
                     PSMessage->setText(DefCodec->toUnicode("Аварийная отмена успешно проведена! "));		
                  }
                  else{
                     PSMessage->setPaletteForegroundColor(QColor(255,0,0));
                     PSMessage->setText(DefCodec->toUnicode(hw->PS_GetErrorDescr(res)));
                  }
                  break;
               }
               default:{}
            }
         }
      }
      else{
         PSMessage->setPaletteForegroundColor(QColor(255,0,0));
         PSMessage->setText(DefCodec->toUnicode(hw->PS_GetErrorDescr(res)));  
      }
      qApp->processEvents();
      QApplication::restoreOverrideCursor();
      qApp-> processEvents();
      return;
   }
   else{//INPAS
      //проверка занят ли текущий порт
      if ( CheckPortNum() == -2)
      {
         DeviceConfErrorText(37,er_str,er_id); 
         InterfaceErrorWin(er_str, er_id);
         return;
      }
      int TypeScheme =  PSTypeScheme->currentItem();
      hw->PS_SetTypeScheme(TypeScheme);
      hw->PS_SetTerminalId((char *)PSTerminalId->text().ascii());
      QString s = DefCodec->fromUnicode(PSPortVerifone->currentText());
      int PortNum = 0;
      int PSType = CheckPSType();
      hw->PS_SetType(PSType);
      switch(PSType){
         case 1:{
            PortNum = (s.right(s.length()-Com.length())).toInt();	
            break;}
         case 3:{
            PortNum = (s.right(s.length()-UsbCom.length())).toInt();	
            break;}
         case 4:{
            PortNum = (s.right(s.length()-Acm.length())).toInt();	
            break;}
      }
      if (PortNum<=0) return;
      hw->PS_SetPortNumVerifone(PortNum);
	 
      int BaudRate = GetBaudRate(PSBaudRateVerifone -> currentItem());
      hw->PS_SetBaudRateVerifone(BaudRate);
      hw->PS_SetTimeout1(PSTimeout1->text().toInt());
      hw->PS_SetTimeout2(PSTimeout2->text().toInt());
      hw->PS_SetTimeout3(PSTimeout3->text().toInt());
      if (Encryption0->isChecked())
         hw->PS_SetEncryptionType(0);
      else
         hw->PS_SetEncryptionType(1);
	 
      if ( TypeSystem == 0){ //PULSAR
         int type_exch = PSTypeExchange->currentItem();
         hw->PS_SetTypeExchange(type_exch);
		  
         if (type_exch == 0){//Файловый обмен
            QString name =  PSNameDir->text();
            if (name.isEmpty()){
               PSMessage->setPaletteForegroundColor(QColor(255,0,0));
               PSMessage->setText(DefCodec->toUnicode("Не заполнена директория обмена!"));
               return;
            }
            else{
               if((access(name.ascii(),0))==-1){
                  PSMessage->setPaletteForegroundColor(QColor(255,0,0));
                  PSMessage->setText(DefCodec->toUnicode("Директория не существует!"));	
                  return;
               }
		          
               hw->PS_SetNameDirExchange(DefCodec->fromUnicode(name)); 
               hw->PS_GenerateFileNames();
            }
         }
         else{
            //TCP/IP
            hw->PS_SetIpAddress(PSIPAddress->text());
            hw->PS_SetIpPort(PSIPPort->text().toInt());
            hw->PS_SetIpAddressVer(PSIPAddressVerifone->text()); 
            hw->PS_SetIpPortVer(PSIPPortVerifone->text().toInt());	
         }
      } 
      hw->PS_SetCodCurrency(643);//по умолчанию код валюты - рубли
      hw->PS_SetModeEnterCard(3);//способ ввода карты
      //установка настроек для печати на чеке
      hw->PS_SetBank(CP1251Codec->fromUnicode(PSBank->text()));// наименование банка 
      hw->PS_SetTradeName(CP1251Codec->fromUnicode(PSTradeName->text()));//торговая точка
      hw->PS_SetTownTrade(CP1251Codec->fromUnicode(PSTownTrade->text()));//город 
      hw->PS_SetAddressTrade(CP1251Codec->fromUnicode(PSAddress->text()));//адрес
      QApplication::setOverrideCursor(waitCursor);
      hw->SetEducationMode(1);//режим обучения
      hw->PS_SetCodeOper(1);//код операции - оплата
      hw->PS_SetSumm(3000);//установка суммы - в копейках
      hw->PS_SetUniqNumTrans(1);//установка номера транзакции
      Delay();
      PSMessage->clear();
      int res = hw->PS_Start();//запуск пин-пада или POS-терминала
      if (res!=0)
      {
         PSMessage->setText(DefCodec->toUnicode(hw->PS_GetErrorDescr(res))); 
      }
      else
      {
         res = hw->PS_SendRequest();
         if ( res == 0){
            PSMessage->setPaletteForegroundColor(QColor(0,0,0));
            PSMessage->setText(DefCodec->toUnicode("Транзакция выполнена"));
            //проверить, что без ошибок прошла транзакция и провести операцию аварийная отмена.
            int status = hw->PS_GetTransStatus();
            if (status == 1){
               qApp->processEvents();
               QApplication::restoreOverrideCursor();
               qApp-> processEvents(); 
					 
                int rres  = QMessageBox::information( this, DefCodec->toUnicode("Настройка оборудования"), DefCodec->toUnicode("Транзакция прошла успешно!\n Уберите карту из пин-пада\n Отменить проведенную транзакцию?"), DefCodec->toUnicode("&Да"), DefCodec->toUnicode("&Нет"), 0, 0 );
               QApplication::setOverrideCursor(waitCursor);
               switch(rres){
                  case 0:{
                     hw->PS_SetCodeOper(7);
                     res = hw->PS_SendRequest();
                     if (res == 0){
                        PSMessage->setPaletteForegroundColor(QColor(0,0,0));
                        PSMessage->setText(DefCodec->toUnicode("Аварийная отмена успешно проведена! "));		
                     }
                     else{
                        PSMessage->setPaletteForegroundColor(QColor(255,0,0));
                        PSMessage->setText(DefCodec->toUnicode(hw->PS_GetErrorDescr(res)));
                     }
                     break;
                  }
                  default:{}
               }
            }
         }
         else{
            PSMessage->setPaletteForegroundColor(QColor(255,0,0));
            PSMessage->setText(DefCodec->toUnicode(hw->PS_GetErrorDescr(res)));
         }
      }
	 
      hw->PS_Stop();
      Delay();
      qApp->processEvents();
      QApplication::restoreOverrideCursor();
      qApp-> processEvents(); 
   }
}

//=======================================================//
//проверка связи с пин-падом
void Configurations::CheckConnect()//only for pulsar
{
	 dl->PrintString(DebugLevel,INFO,"void Configurations::CheckConnect()\n");  	
	 QString er_str;
	 QString er_id;
	 //проверка занят ли текущий порт
	 if ( CheckPortNum() == -2)
	 {
		  DeviceConfErrorText(37,er_str,er_id); 
		  InterfaceErrorWin(er_str, er_id);
		  return;
	 }	
	 int PortNum = 0;
	 QString s = DefCodec->fromUnicode(PSPortVerifone->currentText());
	 int TypePS = CheckPSType();
	 switch(TypePS){
	 case 1:{
				PortNum = (s.right(s.length()-Com.length())).toInt();
				break;}
	 case 3:{
				PortNum = (s.right(s.length()-UsbCom.length())).toInt();
				break; }
	 case 4:{
				PortNum = (s.right(s.length()-Acm.length())).toInt();
				break;}
	 default:{break;}
	 }
	 int TypeSystem = PSType->currentItem();
	 hw->PS_SetTypeSystem(TypeSystem);
	 hw->PS_SetPortNumVerifone(PortNum);
	 if (TypeSystem == 0){
		  if ((TypePS == 1)||(TypePS == 3)||(TypePS == 4)){
				int BaudRate = GetBaudRate(PSBaudRateVerifone -> currentItem());
				hw->PS_SetBaudRateVerifone(BaudRate);
				hw->PS_CheckConnect();
		  }
	 }	
	 else{
		  PSMessage->setPaletteForegroundColor(QColor(255,0,0));
		  PSMessage->setText(DefCodec->toUnicode("Для данного вида платежной системы не поддерживается!"));
	 }
}


QString Configurations::GetECRAccessPassword(){
	 QString accessPassword = "";
	 QString accessPasswordForm = AccessPassword->text();
	 int length = accessPasswordForm.length();
	 if (length>4){
		  accessPassword = accessPasswordForm.right(4); 
		  return accessPassword;
	 }
	 while(length<4){
		  accessPassword += "0";
		  length++;
	 } 
	 accessPassword += accessPasswordForm;
	 return accessPassword;
}


QString Configurations::GetECRPassword(){
	 QString Password = "";
	 QString PasswordForm = AdminPassword->text();
	 printf("PasswordForm = |%s|\n",PasswordForm.ascii());
	 int length = PasswordForm.length();
	 if (length>8){
		  Password = PasswordForm.right(8); 
		  return Password;
	 }
	 while(length<8){
		  Password += "0";
		  length++;
	 } 
	 Password += PasswordForm;
	 printf("Password = |%s|\n",Password.ascii());
	 return Password;
}



int Configurations::SetParamECR(){
	 int FRType = CheckFRType();
	 QString EcrPortNum = FRPortNum->currentText();
	 int PortNumber = 0;
	 switch(FRType){
	 case 1:{
				PortNumber = (EcrPortNum.right(EcrPortNum.length()-Com.length())).toInt();
				break;
		  }
	 case 3:{
				PortNumber = (EcrPortNum.right(EcrPortNum.length()-UsbCom.length())).toInt();
				break;
		  }
	 case 4:{
				PortNumber = (EcrPortNum.right(EcrPortNum.length()-Acm.length())).toInt();
				break;
		  }
	 }
	 int LogicalNumber = 1;
	 int checkPrinter = FRTypeDevice->currentItem();
	 int Protocol = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()), 1);
	 int k =  hw->ECR_SetConnectOptions(FRType, LogicalNumber, PortNumber, GetBaudRate(FRBaudRate->currentItem()),  Protocol, UseAccessPassword->isChecked(),GetECRAccessPassword().ascii(), GetECRPassword().ascii(), checkPrinter);
	 return k;
}


//======================================================//
//открыть денежный ящик
void Configurations::OpenMoneyBoxButton_clicked(void)
{
	 //puts("//----------------------OpenMoneyBoxButton_clicked(void)-----------------//"); 
	 QString er_str;
	 QString er_id;
	 qApp-> processEvents();
	 FRTesttextLabel->clear();
	 qApp-> processEvents();
	 if ( CheckPortNum() == -2)
	 {
		  DeviceConfErrorText(6,er_str,er_id);
		  InterfaceErrorWin(er_str, er_id);
		  FRTestButton->setEnabled(TRUE);
		  return;
	 }
	 int k =  SetParamECR();
	 if (k!=0)
	 {
		  hw->ECR_ReadError(k);
		  errmes =QString( hw->ECR_GetErrNum())+QString(hw->ECR_GetErrDescription());
		  Delay();
		  QApplication::restoreOverrideCursor();
		  return;
	 }
	 int Prot = GetNumByNameAndType(DefCodec->fromUnicode(FRProtocol->currentText()), 1);
	 if (Prot  == 3 || Prot == 4){
		  hw->ECR_SetDrawerOptions(FRMBoxCNL->text(FRMBoxCNL->currentItem()).toInt(), FRMBoxI1->text().toInt(), FRMBoxI2->text().toInt(), FRMBoxICnt->text().toInt());
	 }
	 int res = 0;
	 if (Prot == 5) {
		  res = hw->ECR_ReadStatus();
		  if (res!=0){
				hw->ECR_ReadError(res);
				errmes =QString( hw->ECR_GetErrNum())+QString(hw->ECR_GetErrDescription());
				FRTesttextLabel->setText(errmes);
				Delay();
				qApp-> processEvents();
				QApplication::restoreOverrideCursor();
				qApp-> processEvents();
				hw->ECR_CloseConnection();
				return;
		  }
	 }
	 res = hw->ECR_OpenDrawer();
	 if (res!=0){
		  hw->ECR_ReadError(res);
		  errmes =QString( hw->ECR_GetErrNum())+QString(hw->ECR_GetErrDescription());
		  FRTesttextLabel->setText(errmes);
		  Delay();
		  qApp-> processEvents();
		  QApplication::restoreOverrideCursor();
		  qApp-> processEvents();
	 }
	 else{
		  errmes = DefCodec->toUnicode("Нет ошибок при открытии денежного ящика!");  
		  FRTesttextLabel->setText(errmes);
	 }
	 hw->ECR_CloseConnection();
}

//======================================================//
//проверка на активацию продукта
void Configurations::CheckRights(bool Var){
	 dl->PrintString(DebugLevel,INFO,"bool Configurations::CheckRights()\n"); 
	 if (Var == true){
		  int res_act = hw->GenerateKod();
		  if (res_act!=1){
				dl->PrintString(DebugLevel,INFO,"Продукт не активизирован, нет прав на использование всего функционала!\n");
				PSproperties(FALSE);
				PScheckBox->setChecked(FALSE);
				if (PS_on == true)
				{
					 show_message = false;
					 Save();
					 PS_on = false;
					 show_message = true;
				}
		  }
		  else{
				int res = hw->GetAccessMode();
				int b = ((res<<7)&128)>>7;
				if (b!=1)
				{	
					 dl->PrintString(DebugLevel,INFO,"Нет прав на использование всего функционала!\n");
					 PSproperties(FALSE);
					 PScheckBox->setChecked(FALSE);
					 if (PS_on == true)
					 {
						  show_message = false;
						  Save();
						  PS_on = false;
						  show_message = true;
					 }	

				}
		  }
	 }
}



//======================================================//
//изменить директорию обмена платежной системы
void Configurations::Open_Dir_clicked()
{
	 QString dir_name = PSNameDir->text();
	 dir_name = QFileDialog::getExistingDirectory(dir_name);
	 PSNameDir->setText(dir_name);
}


//=======================================================//
//обновить код активации
void Configurations::UpdateKodActivation_clicked()
{
	 dl->PrintString(DebugLevel,INFO,"void Configurations::UpdateKodActivation_clicked()\n"); 
	 InsertActivKod_clicked(); 
}


//=======================================================//
//проверка связи с весами
void Configurations::EditEWTest( void )
{
    dl->PrintString(DebugLevel,INFO,"void Configurations::EditEWTest()\n"); 
    QString er_str;
    QString er_id;
    if ( CheckPortNum() == -2)
   {
        DeviceConfErrorText(38,er_str,er_id);
        InterfaceErrorWin(er_str, er_id);
        return;
    }
    int LogNum = 1;//logical number device ewtype
    int PortNum = 0;
    QString s = DefCodec->fromUnicode(EWPortNum->currentText());
    int TypeEW = CheckEWType();
    switch(TypeEW){
        case 1:{
            PortNum = (s.right(s.length()-Com.length())).toInt();
            break;}
        case 3:{
            PortNum = (s.right(s.length()-UsbCom.length())).toInt();
            break; }
        case 4:{
            PortNum = (s.right(s.length()-Acm.length())).toInt();
            break;}
        default:{break;}
    }
				
     ew_t = new EWTest;
     ew_t->StartTest(TypeEW, LogNum, PortNum, GetBaudRate(EWBaudRate->currentItem()), EWModel->currentItem()+1);
     ew_t->exec();
     delete ew_t;
 //    hw->EW_Stop();
}

//=======================================================//
//отображение надписи о поддерживаемых моделях весов
void Configurations::EWModelCh()
{
    switch (EWModel->currentItem()+1) {
    case 1: {
	    ModelDescr->setText(DefCodec->toUnicode("<p align=\"center\">поддержка моделей: MK-TH, MK-TB, MK-A, TB-S, TB-M</p>"));
	    break;
	}
	case 2: {
    	    ModelDescr->setText(DefCodec->toUnicode("<p align=\"center\">поддержка моделей: Меркурий-313, Меркурий-314, Меркурий-315</p>"));
	    break;
	}
	case 3: {
    	    ModelDescr->setText(DefCodec->toUnicode("<p align=\"center\">поддержка моделей: AP, AD, DB, ER</p>"));
	    break;
	}
    default: {
    	    ModelDescr->setText("");
	  }
    }
}

//-----------Mirabell 19/12/11 
//=======================================================//
//отображение настроек платежной системы
void Configurations::PSTypeChanged(){  
   //puts("void Configuration::PSTypeChanged()");
   int cur_type = PSType->currentItem();
   //printf("cur_type = %d\n",cur_type);
   switch(cur_type){
       case 0:{
           //puts("PULSAR");
           PSMainSettingsGroupBox->show();
           PSPinPadSettingsGroupBox->show();
           PSPrintSettingsGroupBox->show();
           PSTypeSchemetextLabel->hide();
           PSTypeScheme->hide();
           InpasWidgetStack->show();
           PSTypeExchange_textLabel->show();
           PSTypeExchange->show();
           PSTimeout1_textLabel->show();
           PSTimeout1->show(); 
           PSModelChanged();
           break;
       }
       case 1:{
           // puts("SMARTSALE");
           PSMainSettingsGroupBox->show();
           PSPinPadSettingsGroupBox->show();
           PSPrintSettingsGroupBox->show();
           PSTypeSchemetextLabel->show();
           PSTypeScheme->show();
           InpasWidgetStack->hide();
           PSTypeExchange_textLabel->hide();
           PSTypeExchange->hide();
           PSTimeout1_textLabel->hide();
           PSTimeout1->hide();
           PSTimeout2_textLabel->setText(DefCodec->toUnicode("Таймаут получения ответа от пин-пада:"));
           PSTimeout3_textLabel->setText(DefCodec->toUnicode("Таймаут получения байтов внутри ответа от пин-пада:"));
           break;
       }
       case 2:{
           //puts("SBERBANK");
           PSMainSettingsGroupBox->hide();
           InpasWidgetStack->show();
           InpasWidgetStack->raiseWidget(0);
           PSPinPadSettingsGroupBox->hide();
           PSPrintSettingsGroupBox->hide();
           //printf("PSNameDir->text() = |%s|\n",PSNameDir->text().ascii());   
           //-----------Mirabell 11/01/12
           if ((PSNameDir->text().isEmpty()) || (PSNameDir->text() ==" ")){
	       
                PSNameDir-> setText("/usr/local/bin/SB/");
           }
           //-------------------  
           break; 
       }
   }
}
//---------------------------------------------------------

//========================================================//
//скрывает настройку скорости дисплея в случае выбора usb устройства
void Configurations::CheckOnUsb(){
	// dl->PrintString(DebugLevel,INFO,"void Configuration::CheckOnUsb()\n");
	 string cur_text = CDSPPortNum->text(CDSPPortNum->currentItem());
	// printf("cur_text = |%s|\n",cur_text.c_str());
	 int pos = cur_text.find("USB");
	// printf("pos = %d\n",pos);
	 //printf("cur_text[3] = %d\n",cur_text[3]);
	 if ((pos == 0)&&((cur_text[3]>48) && (cur_text[3]<58))){
		 // puts("we find usb device");
		  CDSPBaudRatetextLabel->hide();
		  CDSPBaudRate->hide();
	 }
	 else{
		  CDSPBaudRatetextLabel->show();
		  CDSPBaudRate->show(); 
	 }
}

//-----(+)-----Zuskin-----24-06-2011-----

void Configurations::EDSproperties( bool Var )
{
    EDS_IpAddrLabel->setEnabled(Var);
    EDS_IpAddr->setEnabled(Var);
    EDS_IpPortLabel->setEnabled(Var);
    EDS_IpPort->setEnabled(Var);
    TestEDSButton->setEnabled(Var);
}

int Configurations::CheckEDSType()
{
    if (EDScheckBox -> isChecked()) {
	return 1;
    }
    return 0;
}
//------------------------------------------------------
void Configurations::TestEDS()
{
    QApplication::setOverrideCursor(waitCursor);
    hw->EDS_SetType(1);
    if (EDS_IpAddr->text().length() > 6) {
	hw->EDS_SetIpAddr(CP1251Codec->fromUnicode(EDS_IpAddr->text()));
    }
    else {
	hw->EDS_SetIpAddr(DefCodec->fromUnicode("127.0.0.1"));
    }
    hw->EDS_SetIpPort(EDS_IpPort->value());
    hw->EDS_Start();
    int res = hw->EDS_GetStatus(1);
    hw->EDS_Stop();    
    QApplication::restoreOverrideCursor();
    if (res == 0) {
	int t =  QMessageBox::information( this, DefCodec->toUnicode("ОК"),
					   DefCodec->toUnicode("Связь с сервером установлена успешно"),DefCodec->toUnicode("Закрыть"));
	
    } 
    else {
	int t =  QMessageBox::information( this, DefCodec->toUnicode("Ошибка"),
					   DefCodec->toUnicode(hw->EDS_GetErrDescription(res)),DefCodec->toUnicode("Закрыть"));
    }
}

