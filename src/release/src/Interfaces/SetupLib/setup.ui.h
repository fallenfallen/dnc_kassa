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
** These will automatically be called by the forms constructor and
** destructor.
*****************************************************************************/


#include "Constants.h"

extern HWSRV * hw;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

const int BARCODE_LENGTH = 13;

SetupParameters* test ;
SetupDiscount* dis_db ;
SetupSamplebarcode* sam_db;
	
TDiscount * discounts;


QTextCodec * codec_sys =  QTextCodec::codecForName(SYSCODEC);
QTextCodec * codec_db;
int* param_id = NULL;
char** param_values = NULL;

const int param_count = 126;

char** param_names = NULL;

char* actscheme;
char** schemes_names;

QValidator * val_doublemoney;
QRegExpValidator * val_doubleprocent;
QValidator * val_schemename;
QValidator * val_text;
QValidator* val_double;
QValidator* val_prefix;
QValidator * val_integer;

QWidget ** controls;


	
DoubleSpinBox* maxstanddiscountDoubleSpinBox;
DoubleSpinBox* maxstandincreaseDoubleSpinBox;
DoubleSpinBox* maxchequediscountDoubleSpinBox;
DoubleSpinBox* maxchequeincreaseDoubleSpinBox;

UpDownLineEdit* sample_barcode[BARCODE_LENGTH];
TSampleBarcode * sambars;


QLineEdit* durationseansSuffixLabel;
QLineEdit* warnaboutendSuffixLabel;
QLineEdit* synchrofromSuffixLabel;
QLineEdit* synchrotoSuffixLabel;
QLineEdit* autoblocktimeSuffixLabel;

HwSignalHandlersController HwSignalHandlersController::instance;



//***********************************************************************//
void Setup::InitConnection() {
    test =  new SetupParameters(LINCASH_CONF_PATH);
// puts("init Setup_Parameters DB");
    dis_db = new SetupDiscount(LINCASH_CONF_PATH);
// puts("init Setup_Parameters_Disc DB");
    sam_db = new SetupSamplebarcode(LINCASH_CONF_PATH);
// puts("init Setup_Parameters_Disc DB");
 
}
/***********************************************************************/
void Setup::init() {
   puts("********************************Init Setup Form**************************************");
   
   autorecountdiscountCheckBox->hide();
   
   documentTable->setColumnWidth(0, 220);
   documentTable->setColumnWidth(1, 400);
   
   docList = NULL;
   
   //author_count = 0;
   dis_count = 0;
   schemes_count = 0;
   selectScheme = -1;
   change = false;
   tmp = 0;
   editDiscountOrder = false;
   old_num = 0;
   sambar_count = 0;
   
   codec_db = QTextCodec::codecForName(DBCODEC);
   
    itmp = 0;
    
    InitConnection();    
    //puts("InitConnection - OK");
    printf("test->er_con = %d \n", test->er_con );
    printf("dis_db->er_con = %d \n", dis_db->er_con );
    printf("sam_db->er_con = %d \n", sam_db->er_con );    
    
    
    Error_Connection = (test->er_con != 0 || dis_db->er_con != 0 || sam_db->er_con != 0) ;
    //printf("Error_Connection = %d \n", Error_Connection);
    if (Error_Connection) return;
    
    InitConnects();
    
    durationsessionSpinBox->setValue(25);
    
    initSampleBarcode();
    
    InitSpinBoxSuffix();
   // puts("   // ВАЛИДАТОРЫ  И ПРОЧИЕ ОГРАНИЧИТЕЛИ ");
    QRegExp rx("^\\d{1,8}[\\.\\,]{1}\\d{2}$");
    val_doublemoney = new QRegExpValidator(rx, this);
    
    QRegExp rx2("^(100\\.00)|(\\d{1,2}\\.\\d{2})|(\\d{1,2}\\,\\d{2})$");
    val_doubleprocent = new QRegExpValidator(rx2, this);
    
    char* tmp_str = (char*) calloc(100, sizeof(char));
    strcpy(tmp_str, "^[A-Za-zА-Яа-я]\[A-Za-zА-Яа-я0-9 _]{0,19}$");
    
    QRegExp rx3(codec_sys->toUnicode(tmp_str));
    val_schemename = new QRegExpValidator(rx3, this);
    free(tmp_str);
    
    val_integer = new QIntValidator( 1, 1000000, this );    
    maxmoneyLineEdit->setValidator(val_integer);
    
    //maxclientsumLineEdit->setValidator(val_doublemoney);    
    //maxmoneyboxLineEdit->setValidator(val_doublemoney);
    fdvalueLineEdit->setValidator(val_doubleprocent);
    
    nameschemeComboBox->lineEdit()->setValidator(val_schemename);
    nameschemeComboBox->lineEdit()->setMaxLength (MaxLen_SchemeName);
    
    QRegExp rx4("^\\d{1,7}[\\.\\,]{1}\\d{0,3}$");
    val_double = new QRegExpValidator(rx4, this);
    
    QRegExp rx5("^\\d{1,2}$");
    val_prefix = new QRegExpValidator(rx5, this);
    
    multipliersamplebarcodeLineEdit->setValidator(val_double);
    prefixsamplebarcodeLineEdit->setValidator(val_prefix);
    
    discount_Init();
    
    fdtextLineEdit->setMaxLength(MaxLen_ChequeText); 
    advertisementTextEdit-> setWrapColumnOrWidth (MaxLen_ChequeText) ;
    
    // флажок о выводе MessageBox'a при изменени галочки схемы по умолчанию
    show_message = false;
    int er;
    
    //заполнение списка схем
    er = InitSchemes() ;	
    if (er < 0) { errorMessage(1); return; }
    
    //посмотреть активную
    actscheme = (char*) calloc(50, sizeof(char));
    
    test->GetActiveScheme(actscheme); 
    
    QString codec_db_tmp = codec_db->toUnicode(actscheme);
    
    QString  tmp = codec_sys->fromUnicode(codec_db_tmp);
    strcpy(actscheme, tmp);

    //printf("act scheme = %s, len = %d \n", actscheme, strlen(actscheme));
    printf("act scheme = %s \n", actscheme);
    
    //если есть активная выделить ее в  списке и пометить галочкой
    if (strlen(actscheme)>0)  {
	//nameschemeComboBox->setCurrentText(codec_sys->toUnicode(actscheme));
	nameschemeComboBox->setCurrentText(codec_db_tmp);
	setdefaultCheckBox->setChecked(true);
	setdefaultCheckBox->setEnabled(false);
	oldName = nameschemeComboBox->currentText(); 
    }
    
    if (strlen(actscheme) > 0) printf("Имя активной схемы = %s\n", actscheme);
    
    if (nameschemeComboBox->count() > 0) selectScheme = nameschemeComboBox->currentItem();
    
    docList = new DocumentList(documentTable);
    
    //инициализация параметров
    er = InitParam();	
    
    if (er < 0) { errorMessage(2); return; }

    // флажки, галочки об показании messageBoxa и об измененииях в схеме
    show_message = true;
    Changed(false);
    
    if ( schemes_count == 0 ) {
	paramTabWidget->setEnabled(false);
	setdefaultCheckBox->setEnabled(false);    }
     
     //connect( this , SIGNAL(close()), this, SLOT(CloseForm()) );
    
    connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char *)));
    connect(HwSignalHandlersController::GetInstance(), SIGNAL(TrackCode(const char *)), SLOT(ReaderSignal( const char *)));
  
    HwSignalHandlersController::GetInstance()->SetControlledObject(SETUP);
        
    grabKeyboard();
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL ) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  ); 	
    //roundpowerSlider->setTickmarks( QSlider::Below );
    
    //puts("roundpowerLineEdit->setReadOnly(true)");
    //roundpowerLineEdit->setReadOnly(false);
    roundpowerLineEdit->setReadOnly(true);
    //if ( roundpowerLineEdit->isReadOnly() ) puts("READ ONLY true");
    //else puts("READ ONLY ---- false");
    
    printf("******************************End Init Setup************************************\n");
    puts("");
}

//================================================================//
void Setup::ScanerSignal(const char* code)
{
    if ( paramTabWidget->currentPageIndex() !=  3 ) return;
    if ( DiscountToolBox->currentIndex() !=  0 ) return;
    if ( ! fdbarcodeLineEdit->isEnabled() ) return;
    fdbarcodeLineEdit->setText( code );
}
//================================================================//
void Setup::ReaderSignal(const char* code)
{
    if ( paramTabWidget->currentPageIndex() !=  3 ) return;
    if ( DiscountToolBox->currentIndex() !=  0 ) return;
    if ( ! fdcardcodeLineEdit->isEnabled() ) return;
    fdcardcodeLineEdit->setText( code );
}
//================================================================//
void Setup::InitConnects() {
    
    /*1 page*/
    connect(nameschemeComboBox, SIGNAL(textChanged(const QString&)), SLOT(nameschemeComboBox_textChanged(const QString&)));
    connect(nameschemeComboBox, SIGNAL(activated(const QString&)), SLOT(nameschemeComboBox_activated(const QString&)));
    
    connect(setdefaultCheckBox, SIGNAL(toggled(bool)), SLOT(setdefaultCheckBox_toggled(bool)));
    
    //connect(maxclientsumLineEdit, SIGNAL(lostFocus()), SLOT(maxclientsumLineEdit_EditEnd()));
    //connect(maxclientsumLineEdit, SIGNAL(returnPressed()), SLOT(maxclientsumLineEdit_EditEnd()));
    //connect(maxclientsumLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    //connect(maxmoneyboxLineEdit, SIGNAL(lostFocus()), SLOT(maxmoneyboxLineEdit_EditEnd()));
    //connect(maxmoneyboxLineEdit, SIGNAL(returnPressed()), SLOT(maxmoneyboxLineEdit_EditEnd()));
    //connect(maxmoneyboxLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(learnmodeCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(learnmodeCheckBox, SIGNAL(toggled(bool)), SLOT(learnmodeCheckBox_toggled(bool)));
    connect(writetransactionCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(limitclientsumCheckBox, SIGNAL(toggled(bool)), SLOT(limitclientsumCheckBox_toggled(bool)));
    connect(limitclientsumCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    
    connect(maxmoneyLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(maxmoneyLineEdit, SIGNAL(textChanged(const QString&)), SLOT(CommaTest(const QString&)));
    connect(maxmoneyLineEdit, SIGNAL(lostFocus()), SLOT(maxmoneyLineEdit_EditEnd()));
    
    connect(autoblockCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(autoblockCheckBox, SIGNAL(toggled(bool)), SLOT(autoblockCheckBox_toggled(bool)));    
    
    connect(autoblocktimeSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    connect(autoblocktimeSpinBox, SIGNAL(valueChanged( const QString & )), SLOT(ParametersChanged()));
    //connect(autoblocktimeSpinBox, SIGNAL(textChanged()), SLOT(ParametersChanged()));

    
    connect(blockmouseCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(blockmouseCheckBox, SIGNAL(toggled(bool)), SLOT(blockmouseCheckBox_toggled(bool)));
    
    connect(useoneclickCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    
    connect(authorbarcodeCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(authorbarcodeCheckBox, SIGNAL(toggled(bool)), SLOT(author_toggled(bool)));
    
    connect(authorcardreaderCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(authorcardreaderCheckBox, SIGNAL(toggled(bool)), SLOT(author_toggled(bool)));
    
    connect(authorpasswordCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(authorpasswordCheckBox, SIGNAL(toggled(bool)), SLOT(author_toggled(bool)));
    
    connect(authorlackrightsCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(picturepathLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(picturepathPushButton, SIGNAL(clicked()), SLOT(GetPicturePath()));
    
    /*2 page*/
    connect(returncountincCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(showproductafterregCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(saveaftercloseCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(showabsentgoodsCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(vsmanualinputCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(grouprecordComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(errormessageComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(findaskcountCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(closefindafter1regCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(findrecordcountSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    //connect(findrecordcountSpinBox, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(findsortComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(controlsaleComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlbackComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controldoubleComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlpriceeditComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlexpirationComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlminpriceComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlremainsComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(controlnegativeremainsComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(roundpowerSlider, SIGNAL(valueChanged(int)), SLOT(roundPowerChanged(int)));
    connect(roundpowerLineEdit, SIGNAL(textChanged(const QString&)), SLOT(setRoundPower(const QString&)));    
    
    connect(roundGroupBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(roundpowerLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(roundpowerLineEdit, SIGNAL(textChanged(const QString&)), SLOT(RoundPowerChanged()));
    connect(roundtypeComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    
    
    /*3 page*/
    /* I */
    connect(fixeddiscountTable, SIGNAL(currentChanged(int, int)), SLOT(fixeddiscountTable_currentChanged(int, int)));
    
    connect(fdtextLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged()));
    
    connect(fdtypeComboBox, SIGNAL(activated(const QString&)), SLOT(fdtypeComboBox_activated(const QString &)));
    connect(fdtypeComboBox, SIGNAL(activated(const QString&)), SLOT(fixeddiscountChanged()));
    connect(fdtypeComboBox, SIGNAL(highlighted(const QString&)), SLOT(fixeddiscountChanged()));
    
    connect(fdnameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged()));
    connect(fdcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged()));
    
    connect(fdvalueLineEdit, SIGNAL(lostFocus()), SLOT(fdvalueLineEdit_EditEnd())); 
    connect(fdvalueLineEdit, SIGNAL(returnPressed()), SLOT(fdvalueLineEdit_EditEnd()));
    connect(fdvalueLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged())); 
    connect(fdvalueLineEdit, SIGNAL(textChanged(const QString&)), SLOT(CommaTest(const QString&))); 
    
    connect(fdfixingComboBox, SIGNAL( activated(int)), SLOT(fixeddiscountChanged()));
    connect(fdfixingComboBox, SIGNAL( highlighted( int )), SLOT(BlockCode()));
    connect(fdfixingComboBox, SIGNAL( activated(int)), SLOT(BlockCode()));
    
    connect(fdbarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged()));
    //connect(fdbarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(enableshowfd(const QString&)));
    connect(fdcardcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(fixeddiscountChanged()));    
    //connect(fdbarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(enableshowfd(const QString&)));
    
    connect(fdshowCheckBox, SIGNAL(toggled(bool)), SLOT(fixeddiscountChanged()));
     
    
    
    connect(newfixeddiscountPushButton, SIGNAL(clicked()), SLOT(newfixeddiscountPushButton_clicked()));
    connect(savefixeddiscountPushButton, SIGNAL(clicked()), SLOT(savefixeddiscountPushButton_clicked()));
    connect(deletefixeddiscountPushButton, SIGNAL(clicked()), SLOT(deletefixeddiscountPushButton_clicked()));
    
    /*3*/
    connect(resultdiscountComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(resultdiscountComboBox, SIGNAL(activated(const QString&)), SLOT(resultdiscountComboBox_activated(const QString& )));
    
    connect(basediscountorderTable, SIGNAL(clicked(int, int, int, const QPoint&)), SLOT(basediscountorderTable_clicked(int, int, int, const QPoint&)));
    connect(basediscountorderTable, SIGNAL(currentChanged(int, int)), SLOT(basediscountorderTable_currentChanged(int, int)));
    
    connect(basediscountorderupPushButton, SIGNAL(clicked()), SLOT(basediscountorderupPushButton_clicked()));
    connect(basediscountorderupPushButton, SIGNAL(clicked()), SLOT(ParametersChanged()));
    
    connect(basediscountorderdownPushButton, SIGNAL(clicked()), SLOT(basediscountorderdownPushButton_clicked()));
    connect(basediscountorderdownPushButton, SIGNAL(clicked()), SLOT(ParametersChanged()));
    
    connect(withoutstanddiscountCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    /*4page*/
    connect(wordwrapCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(printtaxCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(printoneregCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(printoneregCheckBox, SIGNAL(toggled(bool)), SLOT(PrintOneRegChanged(bool)));
    
    /*5 page*/
    connect(autotakereportCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(autotakereportCheckBox, SIGNAL(toggled(bool)), SLOT(autotakereportCheckBox_toggled(bool) ));

    connect(reportXCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportsectionCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportcountCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportbyhourCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportcashierCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportwareCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportunloadCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(reportpsreconsiliationCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    /*6 page*/
    /*1*/
    connect(standdownstringLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(standupstringLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(offdownstringLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    connect(offupstringLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(standdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(standdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(standdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(standdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    /*2*/
    connect(displayproductGroupBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(displayproductdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(displayproductupstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(displayproductnameCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displayproductnamestringComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(displayproductpriceCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displayproductquantityCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displayproductsumCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displayproductpricestringComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(displayproducttotalCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displayproducttotalstringComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    /*3*/
    connect(displaydiscountGroupBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(displaydiscountdownstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    connect(displaydiscountupstringrunComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(displaydiscountnameCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displaydiscountnamestringComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    connect(displaydiscountsumCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(displaydiscountsumstringComboBox, SIGNAL(activated(const QString&)), SLOT(ParametersChanged()));
    
    /*4*/
    connect(synchroCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(synchroCheckBox, SIGNAL(toggled(bool)), SLOT(synchroCheckBox_toggled(bool)));
    
    connect(autosynchroCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(synchrofromSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    //connect(synchrofromSpinBox, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(synchrofromSpinBox, SIGNAL(valueChanged(int)), SLOT(synchrotimechanged()));
    //connect(synchrofromSpinBox, SIGNAL(textChanged(const QString&)), SLOT(synchrotimechanged()));
    
    connect(synchrotoSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    //connect(synchrotoSpinBox, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(synchrotoSpinBox, SIGNAL(valueChanged(int)), SLOT(synchrotimechanged()));
    //connect(synchrotoSpinBox, SIGNAL(textChanged(const QString&)), SLOT(synchrotimechanged()));
    
    connect(notifysynchrotimefieldCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(notifysynchromessageCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(openboxbyclosechequeCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(openboxbyclosechequeCheckBox, SIGNAL(toggled(bool)), SLOT(ob_closecheck_cb_toggled(bool)));
    connect(openboxbyrestCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(openboxbychangemoneyCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    connect(openboxbyclearreportCheckBox, SIGNAL(toggled(bool)), SLOT(ParametersChanged()));
    
    connect(durationsessionSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    //connect(durationsessionSpinBox, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(warnaboutendSpinBox, SIGNAL(valueChanged(int)), SLOT(ParametersChanged()));
    //connect(warnaboutendSpinBox, SIGNAL(textChanged(const QString&)), SLOT(ParametersChanged()));
    
    connect(newPushButton, SIGNAL(clicked()), SLOT(newPushButton_clicked()));
    connect(savePushButton, SIGNAL(clicked()), SLOT(savePushButton_clicked()));
    connect(deletePushButton, SIGNAL(clicked()), SLOT(deletePushButton_clicked()));
    
    
    /*sample barcode*/
    connect(newsamplebarcodePushButton, SIGNAL(clicked()), SLOT(NewSampleBarcode()));
    connect(savesamplebarcodePushButton, SIGNAL(clicked()), SLOT(SaveSampleBarcode()));
    connect(deletesamplebarcodePushButton, SIGNAL(clicked()), SLOT(DeleteSampleBarcode()));
    
    connect(samplebarcodeTable, SIGNAL(currentChanged(int, int)), SLOT(samplebarcodeTable_currentChanged(int, int)));
      
    connect(prefixsamplebarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(samplebarcodeChanged()));
    connect(multipliersamplebarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(samplebarcodeChanged()));
    connect(multipliersamplebarcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(CommaTest(const QString&)));
    
    
    /* Document Print */
    connect(adddocumentPushButton, SIGNAL(clicked()), SLOT(AddDocument()));
    connect(editdocumentPushButton, SIGNAL(clicked()), SLOT(EditDocument()));
    connect(removedocumentPushButton, SIGNAL(clicked()), SLOT(RemoveDocument()));
    
    //connect(adddocumentPushButton, SIGNAL(clicked()), SLOT(ParametersChanged()));
    connect(editdocumentPushButton, SIGNAL(clicked()), SLOT(ParametersChanged()));
    connect(removedocumentPushButton, SIGNAL(clicked()), SLOT(ParametersChanged()));
    
    connect(documentTable, SIGNAL(currentChanged(int, int)), SLOT(documentTable_currentChanged(int, int)));
    connect(documentpathPushButton, SIGNAL(clicked()), SLOT(SetDocumentPath()));
    
    connect(documentTable, SIGNAL(valueChanged(int, int)), SLOT(documentTable_valueChanged(int, int)));
    
}
//================================================================//	
void Setup::Changed(bool b) {
    change = b;
    savePushButton->setEnabled( b );
    // шрифт
    QFont f = nameschemeComboBox->lineEdit()->font();
    f.setBold( b );
    nameschemeComboBox->lineEdit()->setFont(f);
}
//================================================================//
void Setup::initDoubleSpinBox( QSpinBox* dsb, const QRect& rect)
{
    dsb->setGeometry(rect);
    dsb->setMaxValue( 10000 );
    dsb->setLineStep( 100 );
    connect( dsb, SIGNAL( valueChanged(int) ), this, SLOT( ParametersChanged() ) );
}
//================================================================//
void Setup::discount_Init() 
{
    //puts("DiscountInit()");
    QPixmap pxm = QPixmap::fromMimeSource("arrow_up.png");
    
    fixeddiscountTable->setColumnWidth(0, 30);
    fixeddiscountTable->setColumnWidth(1, 190);
    fixeddiscountTable->setColumnWidth(2, 180);
    fixeddiscountTable->setColumnWidth(3, 50);
    fixeddiscountTable->setColumnWidth(4, 90);
    fixeddiscountTable->setColumnWidth(5, 100);
    fixeddiscountTable->setColumnWidth(6, 110);
    fixeddiscountTable->setColumnWidth(7, 130);
    fixeddiscountTable->hideColumn(8);
    
    // максимальная произвольная скидка на позицию
    maxstanddiscountDoubleSpinBox = new DoubleSpinBox( standdiscountGroupBox, "maxstanddiscountDoubleSpinBox" );
    initDoubleSpinBox((QSpinBox*)maxstanddiscountDoubleSpinBox, QRect( 200, 25, 100, 25 ));
    
    //((QGridLayout*)(standdiscountGroupBox->layout()))->addWidget(maxstanddiscountDoubleSpinBox, 0, 1);
  
    // максмальная произвольная надбавка на позицию
    maxstandincreaseDoubleSpinBox = new DoubleSpinBox( standdiscountGroupBox, "maxstandincreaseDoubleSpinBox" );
    initDoubleSpinBox((QSpinBox*)maxstandincreaseDoubleSpinBox, QRect( 200, 65, 100, 25 ));
    
    //((QGridLayout*)(standdiscountGroupBox->layout()))->addWidget(maxstandincreaseDoubleSpinBox, 1, 1);
    
    //максимальная скидка на чек
    maxchequediscountDoubleSpinBox = new DoubleSpinBox( chequediscountGroupBox, "maxchequediscountDoubleSpinBox" );
    initDoubleSpinBox((QSpinBox*)maxchequediscountDoubleSpinBox, QRect( 200, 25, 100, 25 ));
    
    //максимальная произвольная надбавка на чек
    maxchequeincreaseDoubleSpinBox = new DoubleSpinBox( chequediscountGroupBox, "maxchequeincreaseDoubleSpinBox" );
    initDoubleSpinBox((QSpinBox*)maxchequeincreaseDoubleSpinBox, QRect( 200, 65, 100, 25 ));
    
    //maxclientsumLineEdit->hide();
    //maxclientsumTextLabel->hide();
    //maxmoneyboxLineEdit->hide();
    //maxmoneyboxTextLabel->hide();
    //controlGroupBox->hide();
    PrintCheck->hide();
    
    DiscountToolBox->setCurrentIndex(2);
    DeviceToolBox->setCurrentIndex(1);
    DisplayToolBox->setCurrentIndex(2);
    
    
    fixeddiscountTable->verticalHeader()->setMovingEnabled(false);
    
    basediscountorderTable->setColumnWidth( 0, 200 );
    for (uint i=0; i<CountDiscountOrder; i++) basediscountorderTable->setText( i, 0, codec_sys->toUnicode(DiscountOrder[i]) );
    EditDiscountOrder(false);
    
}
//======================================================//
void Setup::initSuffixLabel(QLineEdit* le, const char* suffix )
{
    le->setGeometry( QRect(20,5,30,15) );
    le->setFocusPolicy( QLabel::NoFocus );
    le->setText(codec_sys->toUnicode(suffix));
    
    le->setFrame(false);
    
}
//======================================================//
void Setup::InitSpinBoxSuffix()
{
    QRegExp rx("^\\d{2}$");
    QRegExpValidator * val_spinbox = new QRegExpValidator(rx, this);
    durationsessionSpinBox->setValidator(val_spinbox); 
    durationseansSuffixLabel = new QLineEdit( durationsessionSpinBox, "durationseansSuffixLabel" );
    initSuffixLabel(durationseansSuffixLabel, "ч");
    warnaboutendSpinBox->setValidator(val_spinbox);
    warnaboutendSuffixLabel = new QLineEdit( warnaboutendSpinBox, "warnaboutendSuffixLabel" );
    initSuffixLabel(warnaboutendSuffixLabel, "мин");
    synchrofromSpinBox->setValidator(val_spinbox);
    synchrofromSuffixLabel = new QLineEdit( synchrofromSpinBox, "synchrofromSuffixLabel" );
    initSuffixLabel(synchrofromSuffixLabel, "мин");
    synchrotoSpinBox->setValidator(val_spinbox);
    synchrotoSuffixLabel = new QLineEdit( synchrotoSpinBox, "synchrotoSuffixLabel" );
    initSuffixLabel(synchrotoSuffixLabel, "мин");
    autoblocktimeSpinBox->setValidator(val_spinbox);
    autoblocktimeSuffixLabel = new QLineEdit(autoblocktimeSpinBox, "autoblocktimeSuffixLabel");
    initSuffixLabel(autoblocktimeSuffixLabel,  "мин");
}


//***********************************************************************//
void Setup::initSampleBarcode() {
//    puts("void Setup::initSampleBarcode() ");
    
    samplebarcodeTable->setColumnWidth(1, 200);
    samplebarcodeTable->hideColumn(2);
    samplebarcodeTable->verticalHeader()->setMovingEnabled(false);
    
    for ( int i=0; i < BARCODE_LENGTH; i++ )    {
	sample_barcode[i] = new UpDownLineEdit( samplebarcodeFrame );
	sample_barcode[i]->setPosition(10+i*UpDownLineEdit::EDIT_WIDTH, 10);    
	connect(sample_barcode[i], SIGNAL(textChanged(const QString&)), SLOT(samplebarcodeChanged()));    } 
    
    connect(sample_barcode[0], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change0()));
    connect(sample_barcode[1], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change1()));
    connect(sample_barcode[2], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change2()));
    connect(sample_barcode[3], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change3()));
    connect(sample_barcode[4], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change4()));
    connect(sample_barcode[5], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change5()));
    connect(sample_barcode[6], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change6()));
    connect(sample_barcode[7], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change7()));
    connect(sample_barcode[8], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change8()));
    connect(sample_barcode[9], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change9()));
    connect(sample_barcode[10], SIGNAL(textChanged(const QString&)), SLOT(samplebarcode_change10()));  
    
    sample_barcode[0]->setPossibleValue(codec_sys->toUnicode("КН"));
    
    sample_barcode[0]->setValue(1);
    sample_barcode[1]->setValue(1);
    
    sample_barcode[12]->setPossibleValue(codec_sys->toUnicode("Н"));
    sample_barcode[12]->setValue(0);
}
//***********************************************************************//
void Setup::NewSampleBarcode() {
//     puts("NewSampleBarcode()"); 
     samplebarcodeTable->insertRows(samplebarcodeTable->numRows());
     sambar_count++;
     sambars = (TSampleBarcode*) realloc(sambars, sambar_count * sizeof(TSampleBarcode));
     QString pref_sam = getDefaultPrefix();
//     printf("getDefaultPrefix() return = %s \n", pref_sam.ascii());
     SetSambarControls( pref_sam, codec_sys->toUnicode("ННКККККМММММН"), QString("1") );
     SaveSambar(sambar_count - 1);
//     printf("******************* sambars[sambar_count - 1].sample = %s **************************\n", sambars[sambar_count - 1].sample);
     sambars[sambar_count - 1].id = -1;
//     printf("samplebarcodeTable->setCurrentCell(%d) \n", samplebarcodeTable->numRows()-1);
//     puts("-----------------------------------------------------------------------------------------------------------------------");
     samplebarcodeTable->setCurrentCell(samplebarcodeTable->numRows()-1, 1);
//     puts("-----------------------------------------------------------------------------------------------------------------------");
     Changed(true);
     sbGroupBox->setEnabled(true);
//     puts(" ))) New barcode ok");
}
//***********************************************************************//
QString Setup::getDefaultPrefix() {
    int i = 1;
    while (  !samplebarcodeValidPrefix(QString::number(i), -1 ) ) { 
	//printf("----samplebarcodeValidPrefix return false \n" );  
	i++;  }
    //printf("----------------------------getdefaultParam - i = %d \n ", i);
    return QString::number(i);
}
//***********************************************************************//
void Setup::SetSambarControls(QString pref, QString sam, QString mult) {
    //puts("SetSambarControls(QString pref, QString sam, QString mult)");
    prefixsamplebarcodeLineEdit->setText(pref);
    //printf("prefixsamplebarcodeLineEdit->setText( ) = %s \n", prefixsamplebarcodeLineEdit->text().ascii());
    SetSampleBarcode(sam);
    multipliersamplebarcodeLineEdit->setText(mult);
    //puts("void Setup::SetSambarControls(QString pref, QString sam, QString mult) --------------------------------- ok");
}
//***********************************************************************//
void Setup::SaveSambar(int num)
{
    //printf("void Setup::SaveSambar(int num = %d )\n", num);
    if (num < 0)  return;
    //printf("prefixsamplebarcodeLineEdit->text() = <%s> \n", prefixsamplebarcodeLineEdit->text().ascii() );
    //printf("table rows = %d, columns = %d \n", samplebarcodeTable->numRows(), samplebarcodeTable->numCols());
    
    samplebarcodeTable->setText(num, 0, prefixsamplebarcodeLineEdit->text());
    samplebarcodeTable->setText(num, 1, GetSampleBarcode());
    samplebarcodeTable->setText(num, 2, multipliersamplebarcodeLineEdit->text());
    //QString tmp = codec_sys->fromUnicode(fdnameLineEdit->text());
    QString qstr_db = codec_db->fromUnicode(prefixsamplebarcodeLineEdit->text());
    strcpy( sambars[num].prefix, qstr_db);    
        
    qstr_db = GetSampleBarcode() ;
    qstr_db = codec_db->fromUnicode( qstr_db );
    // textLabel1_4->setText(qstr_db);
    strcpy( sambars[num].sample,  qstr_db);
   
    qstr_db = multipliersamplebarcodeLineEdit->text() ;
    //qstr_db = codec_db->fromUnicode( qstr_db );
    strcpy( sambars[num].multiplier, qstr_db);
   
    //tmp = codec_sys->fromUnicode(fdtextLineEdit->text());

    //puts("void Setup::SaveSambar(int num) --------------------- ok");
}
//***********************************************************************//
void Setup::samplebarcodeNotAvaiable() {
              //puts("void Setup::samplebarcodeNotAvaiable() "); 
    	sbGroupBox->setEnabled(false);
	//SetDiscountControls(QString(), QString(), QString("- %"), QString(), codec_sys->toUnicode("на позицию"));
	savesamplebarcodePushButton->setEnabled(false);
	deletesamplebarcodePushButton->setEnabled(false);
}
//================================================================//
void Setup::samplebarcodeTable_currentChanged( int row, int col ) {
    //printf("samplebarcodeTable_currentChanged( int row = %d, int col = %d  )\n", row, col);
    if ( row == -1 ) return;    
    if ( samplebarcodeTable->text(row, col) == "" ) return;
    sbGroupBox->setEnabled(true);
    
    SetSambarControls(  samplebarcodeTable->text(row, 0), 
		   samplebarcodeTable->text(row, 1), 
		   samplebarcodeTable->text(row, 2));
    deletesamplebarcodePushButton->setEnabled(true);
    savesamplebarcodePushButton->setEnabled(false);
    
}
//***********************************************************************//
int Setup::samplebarcodeValidPrefix(QString pref, int num_pref) {
    //puts("int Setup::samplebarcodeValidPrefix(QString pref, int num_pref)");
    if ( pref.isNull()  || pref.isEmpty() ) return false;
    int count_pref = samplebarcodeTable->numRows();
    if ( count_pref == 0) return true;
    
    int i = 0;
    //printf("i = %d \n, count_pref = %d \n", i, count_pref);
    if ( (num_pref != -1) && (i == num_pref) ) i++;
    //printf("pref = %s \n", pref.ascii());
    //printf("i = %d \n, count_pref = %d \n", i, count_pref);
    while ( (samplebarcodeTable->text(i,0) != pref) && ( i< count_pref) ) {
	i++;
	if ( i == num_pref ) i++;    }
    //printf("i = %d \n, count_pref = %d \n", i, count_pref);
    if ( i== count_pref ) return true; else return false;
}
//***********************************************************************//
void Setup::SaveSampleBarcode() {
    //puts("SaveSampleBarcode");
    if (multipliersamplebarcodeLineEdit->text().isEmpty()) {

	errorMessage(3);
	 // путой множитель
	//error
	return; }
    if ( ! samplebarcodeValidPrefix(prefixsamplebarcodeLineEdit->text(), samplebarcodeTable->currentRow()) )  { 
	// ошибка error

	errorMessage(4);
	return; }
	
    SaveSambar(samplebarcodeTable->currentRow());
    //printf("******************* sambars[samplebarcodeTable->currentRow()].sample = %s **************************\n", sambars[samplebarcodeTable->currentRow()].sample);
    savesamplebarcodePushButton->setEnabled(false);
    Changed(true);
}
//***********************************************************************//
void Setup::DeleteSampleBarcode() {
    //puts("Setup::DeleteSampleBarcode()");
    int num = samplebarcodeTable->currentRow();
    if (num == -1) return;
    samplebarcodeTable->removeRow(num);
    sambar_count--;
    //printf("sambar_count-- count = %d \n", sambar_count);
    
    for (int j = num; j < sambar_count; j++) { sambars[ j ] = sambars[j+1];    }
    
    sambars = (TSampleBarcode*) realloc(sambars, sambar_count *sizeof(TSampleBarcode));
    Changed(true);
    if ( samplebarcodeTable->numRows() ==0 )  samplebarcodeNotAvaiable();
}
//***********************************************************************//
QString Setup::GetSampleBarcode() {
    //puts("Setup::GetSampleBarcode()");
    QString str;
    for (int i=0; i<BARCODE_LENGTH; i++) {
	str = str + sample_barcode[i]->text();     }
    return str;
}
//***********************************************************************//	
void Setup::SetSampleBarcode(QString  sample_barcode_string) {
    //puts("Setup::SetSampleBarcode(QString  sample_barcode_string)");
    //printf("sample_barcode_string.length() = %d\n", sample_barcode_string.length());
    if ( sample_barcode_string.length() != (int) BARCODE_LENGTH ) {
	//ошибка error
                //printf("Штрих код не верной длины\n");
	errorMessage(5);
	return;  }
    QString breakpoint = GetSampleBarcode();
    int i = 0;
    while  ( (i<BARCODE_LENGTH) && (sample_barcode[i]->setText( sample_barcode_string[i] )!= -1) ) i++;
    if ( i != BARCODE_LENGTH ) {
	/// errorr ошибка   (шаблон штрих кода не верен)
	errorMessage(6);
	SetSampleBarcode(breakpoint);
	return;	}
}
//***********************************************************************//
void Setup::samplebarcode_change(int num) {
    if ( sample_barcode[num]->text() == codec_sys->toUnicode("К") ) {
	sample_barcode[num+1]->setPossibleValue(codec_sys->toUnicode("КМ"));
	sample_barcode[num+1]->setValue(0);    }    
    if ( sample_barcode[num]->text() == codec_sys->toUnicode("М") ) {
	sample_barcode[num+1]->setPossibleValue(codec_sys->toUnicode("МН"));
	sample_barcode[num+1]->setValue(0);  }
    if ( sample_barcode[num]->text() == codec_sys->toUnicode("Н") ) {
	int i = 0;
	while ( (i < num) && (sample_barcode[i]->text() != codec_sys->toUnicode("М")) ) i++;
	if ( i != num) {
	    sample_barcode[num+1]->setPossibleValue(codec_sys->toUnicode("Н"));  
	} else  {
	    sample_barcode[num+1]->setPossibleValue(codec_sys->toUnicode("КН"));   
	}
	sample_barcode[num+1]->setValue(0);    }   
}
//***********************************************************************//
void Setup::samplebarcode_change0() {
    if ( sample_barcode[0]->text() == codec_sys->toUnicode("Н") ) {
	sample_barcode[1]->setPossibleValue(codec_sys->toUnicode("КН"));
	sample_barcode[1]->setValue(0);    }    
    if ( sample_barcode[0]->text() == codec_sys->toUnicode("К") ) {
	sample_barcode[1]->setPossibleValue(codec_sys->toUnicode("КМ"));
	sample_barcode[1]->setValue(0);     }
}
//***********************************************************************//
void Setup::samplebarcodeChanged() { savesamplebarcodePushButton->setEnabled(true); }
//***********************************************************************//
void Setup::samplebarcode_change1() {  samplebarcode_change(1); }
//***********************************************************************//
void Setup::samplebarcode_change2() {  samplebarcode_change(2);  }
//***********************************************************************//
void Setup::samplebarcode_change3()  { samplebarcode_change(3);  }
//***********************************************************************//
void Setup::samplebarcode_change4()  {  samplebarcode_change(4); }
//***********************************************************************//
void Setup::samplebarcode_change5()  {  samplebarcode_change(5); }
//***********************************************************************//
void Setup::samplebarcode_change6()  {  samplebarcode_change(6); }
//***********************************************************************//
void Setup::samplebarcode_change7()  {  samplebarcode_change(7); }
//***********************************************************************//
void Setup::samplebarcode_change8()   {  samplebarcode_change(8); }
//***********************************************************************//
void Setup::samplebarcode_change9() {
    if ( sample_barcode[9]->text() == codec_sys->toUnicode("К") ) {
	sample_barcode[10]->setPossibleValue(codec_sys->toUnicode("КМ"));
	sample_barcode[10]->setValue(0);     }    
    if ( sample_barcode[9]->text() == codec_sys->toUnicode("М") ) {
	sample_barcode[10]->setPossibleValue(codec_sys->toUnicode("МН"));
	sample_barcode[10]->setValue(0);   }
    if ( sample_barcode[9]->text() == codec_sys->toUnicode("Н") ) {
	int i = 0;
	while ( (i< 9) && (sample_barcode[i]->text() != codec_sys->toUnicode("М")) ) i++;
	if ( i != 9 )  {
	    sample_barcode[10]->setPossibleValue(codec_sys->toUnicode("Н")); 
	} else {
	    sample_barcode[10]->setPossibleValue(codec_sys->toUnicode("К"));
	}
	sample_barcode[10]->setValue(0);    }  
}
//***********************************************************************//
void Setup::samplebarcode_change10() {
   if ( sample_barcode[10]->text() == codec_sys->toUnicode("К") ) {
	sample_barcode[11]->setPossibleValue(codec_sys->toUnicode("М"));
	sample_barcode[11]->setValue(0);      }    
    if ( sample_barcode[10]->text() == codec_sys->toUnicode("М") ) {
	sample_barcode[11]->setPossibleValue(codec_sys->toUnicode("МН"));
	sample_barcode[11]->setValue(0);  }
    if ( sample_barcode[10]->text() == codec_sys->toUnicode("Н") ) {
	sample_barcode[11]->setPossibleValue(codec_sys->toUnicode("Н"));	
	sample_barcode[11]->setValue(0);    }  
}
//***********************************************************************/
int Setup::InitSchemes()
{
    puts("************************************Init Schemes********************************");
    
    // вычисление количества схем в БД
    test->GetCountSchemes(&schemes_count);
    printf("количество схем =  %d\n", schemes_count);
    
    // заполнение массива имен схем
    if (schemes_count >0)  { 
	// выделение памяти под переменные
	schemes_names = (char**) calloc(schemes_count, sizeof(char*));
	
	for (int i=0; i<schemes_count; ++i)  {
	    schemes_names[i] = (char*) calloc(45, sizeof(char)); }	
	
	// заполнение 
	int k = test->GetSchemesNames(schemes_names, schemes_count);
	if (k !=2) {
	    //puts("Ошибка получения списка имен схем!!!");
	    return k; }
	
	// добавление имен в контрол
	for (int i=0; i<schemes_count; ++i) {
	    //printf("scheme names %d = %s \n", i, schemes_names[i]);
	    //nameschemeComboBox->insertItem(QString(codec_sys->toUnicode(schemes_names[i])),i); 	
	    nameschemeComboBox->insertItem(codec_db->toUnicode(schemes_names[i]), i); 	
	}
	
	 int i = schemes_count;
	 while (--i>=0)    {
	     //printf("%s free \n", schemes_names[i]);
	    free(schemes_names[i]);    
	 }
	 free(schemes_names); 
	 //printf("schemes_names free()\n");
    }
    puts("****************************************End Init Schemes********************************" );
    return 0;
}
//======================================================//
// связывание параметра с определенным контрольным элементом
void  Setup::FillParam(const char* name, QWidget* control) {
    param_names[itmp] = (char*) calloc(strlen(name) + 5, sizeof(char));
    strcpy(param_names[itmp], name);
    controls[itmp] = control;
    itmp++;
}
//======================================================//
//инициализация параметров
int Setup::InitParam() {
    puts("***********************Setup::Init parameters*******************************");
    //puts("Setup::Init parameters");
    
    //заполнение массива имен параметров и связывание их с определенным контролом
    param_id = (int*) calloc(param_count, sizeof(int));
    param_names = (char**) calloc(param_count, sizeof(char*));
    
    controls = new QWidget* [ param_count ];
    
    
    //base
    //FillParam("MaxClientSum", maxclientsumLineEdit);
    //FillParam("MaxMoneyBox", maxmoneyboxLineEdit);
    FillParam("LearnMode", learnmodeCheckBox);
    FillParam("WriteTransaction", writetransactionCheckBox);
    FillParam("LimitClientSum", limitclientsumCheckBox);
    FillParam("MaxMoney", maxmoneyLineEdit);
    FillParam("DurationSession", durationsessionSpinBox);
    FillParam("WarnAboutEnd", warnaboutendSpinBox);
    FillParam("AutoBlock", autoblockCheckBox);
    FillParam("AutoBlockTime", autoblocktimeSpinBox);
    FillParam("BlockMouse", blockmouseCheckBox);
    FillParam("UseOneClick", useoneclickCheckBox);
    FillParam("AuthorLackRights", authorlackrightsCheckBox);
    FillParam("AuthorPassword", authorpasswordCheckBox);
    FillParam("AuthorCardReader", authorcardreaderCheckBox);
    FillParam("AuthorBarCode", authorbarcodeCheckBox);
    FillParam("PicturePath", picturepathLineEdit);
    
    //registration
    FillParam("ReturnCountInc", returncountincCheckBox);
    FillParam("SaveAfterClose", saveaftercloseCheckBox);
    FillParam("ShowProduct", showproductafterregCheckBox);
    FillParam("ShowAbsentGoods", showabsentgoodsCheckBox);
    FillParam("VSManualInput", vsmanualinputCheckBox);
    FillParam("ErrorMessage", errormessageComboBox); 
    FillParam("GroupRecord", grouprecordComboBox);
    FillParam("CloseFindAfter1reg", closefindafter1regCheckBox);
    FillParam("FindAskCount", findaskcountCheckBox);
    FillParam("FindSort", findsortComboBox);
    FillParam("FindRecordCount", findrecordcountSpinBox);
    FillParam("ControlSale", controlsaleComboBox );
    FillParam("ControlBack", controlbackComboBox);
    FillParam("ControlDouble", controldoubleComboBox);
    FillParam("ControlPriceEdit", controlpriceeditComboBox);
    FillParam("ControlExparation", controlexpirationComboBox);
    FillParam("ControlMinPrice", controlminpriceComboBox);
    FillParam("ControlRemains", controlremainsComboBox);   
    FillParam("ControlNegativeRemains", controlnegativeremainsComboBox);
    
    FillParam("RoundCheckSum", roundGroupBox);
    FillParam("RoundPower", roundpowerLineEdit);
    FillParam("RoundType", roundtypeComboBox);
    
    //reports
    FillParam("AutoTakeReport", autotakereportCheckBox);
    FillParam("ReportX", reportXCheckBox);
    FillParam("ReportSection", reportsectionCheckBox);
    FillParam("ReportCount", reportcountCheckBox);
    FillParam("ReportByHour", reportbyhourCheckBox);
    FillParam("ReportCashier", reportcashierCheckBox);
    FillParam("ReportWare", reportwareCheckBox);
    FillParam("ReportUnload", reportunloadCheckBox);
    FillParam("ReportPSReconsiliation", reportpsreconsiliationCheckBox);
    
    //скидкп
    //произвольные скидки и надбавки
    FillParam("MaxStandDiscount", maxstanddiscountDoubleSpinBox);
    FillParam("MaxStandIncrease", maxstandincreaseDoubleSpinBox);
    FillParam("MaxChequeDiscount", maxchequediscountDoubleSpinBox);
    FillParam("MaxChequeIncrease", maxchequeincreaseDoubleSpinBox);
    
     //общие скидки
     FillParam("ResultDiscount", resultdiscountComboBox);
     FillParam("BaseDiscountOrder", basediscountorderTable);
     FillParam("AutoRecountDiscount", autorecountdiscountCheckBox);
     FillParam("WithoutStandDiscount", withoutstanddiscountCheckBox);
     
    //печать чека
    //общие
    FillParam("WordWrap", wordwrapCheckBox);
    FillParam("UseSetupPOS", usesetupPOSCheckBox);
    usesetupPOSCheckBox->hide();
    FillParam("PrintTax", printtaxCheckBox);
    FillParam("PrintOneReg", printoneregCheckBox);
    
    // печать шапки
    FillParam("TitlePCName", titlepcnameCheckBox);
    FillParam("TitleCashierName", titlecashiernameCheckBox);
    FillParam("TitleDocType", titledoctypeCheckBox);
    FillParam("TitleWorkTime", titleworktimeCheckBox);
    FillParam("TitleDocNumber", titledocnumberCheckBox);
    FillParam("TitleSessionNumber", titlesessionnumberCheckBox);
    FillParam("TitleSeparator", titleseparatorCheckBox);
    FillParam("TitleSeparatorType", titleseparatortypeComboBox);
    
    // печать позиции
    FillParam("StandName", standnameComboBox);
    FillParam("StandCount", standcountComboBox);
    FillParam("StandPrice", standpriceComboBox);
    FillParam("StandSeparator", standseparatorCheckBox);
    FillParam("StandSeparatorType", standseparatortypeComboBox);
    FillParam("StandNumber", standnumberCheckBox);
    FillParam("StandCode", standcodeCheckBox);
    FillParam("StandBarCode", standbarcodeCheckBox);
    FillParam("StandTaxRate", standtaxrateCheckBox);
    FillParam("StandResultTax", standresulttaxCheckBox);
    FillParam("StandDiscount", standdiscountCheckBox);
    FillParam("StandSumDiscount", standsumdiscountCheckBox);
    FillParam("StandResultDiscount", standresultdiscountCheckBox);
    FillParam("StandTotal", standtotalCheckBox);
    
    // печать подвала
    FillParam("BottomCountStand", bottomcountstandCheckBox);
    FillParam("BottomCountBuying", bottomcountbuyingCheckBox); 
    FillParam("BottomTaxRates", bottomtaxratesCheckBox);
    FillParam("BottomResultTax", bottomresulttaxCheckBox);
    FillParam("BottomDiscount", bottomdiscountCheckBox);
    FillParam("BottomSumDiscount", bottomsumdiscountCheckBox);
    FillParam("BottomResultDiscount", bottomresultdiscountCheckBox);
    FillParam("BottomSeparator", bottomseparatorCheckBox);
    FillParam("BottomSeparatorType", bottomseparatortypeComboBox);
    
    //длинные параметры
    FillParam("Advertisement", advertisementTextEdit);
    FillParam("StandUpString", standupstringLineEdit);
    FillParam("StandDownString", standdownstringLineEdit);
    FillParam("OffUpString", offupstringLineEdit);
    FillParam("OffDownString", offdownstringLineEdit);
    
    //параметры работы с оборудованием, сообщения при простоях
    FillParam("StandUpStringRun", standupstringrunComboBox);
    FillParam("StandDownStringRun", standdownstringrunComboBox);
    FillParam("OffUpStringRun", offupstringrunComboBox);
    FillParam("OffDownStringRun", offdownstringrunComboBox);
    
    // детализация при регистрации
    //товар
    FillParam("DisplayProduct", displayproductGroupBox);
    FillParam("DisplayProductName", displayproductnameCheckBox);
    FillParam("DisplayProductPrice", displayproductpriceCheckBox);
    FillParam("DisplayProductQuantity", displayproductquantityCheckBox);
    FillParam("DisplayProductSum", displayproductsumCheckBox);
    FillParam("DisplayProductTotal", displayproducttotalCheckBox);
    
    FillParam("DisplayProductNameString", displayproductnamestringComboBox);
    FillParam("DisplayProductPriceString", displayproductpricestringComboBox);
    FillParam("DisplayProductTotalString", displayproducttotalstringComboBox);
    
    FillParam("DisplayProductUpStringRun", displayproductupstringrunComboBox);
    FillParam("DisplayProductDownStringRun", displayproductdownstringrunComboBox);
   
    // скидки
    FillParam("DisplayDiscount", displaydiscountGroupBox);
    FillParam("DisplayDiscountName", displaydiscountnameCheckBox);
    FillParam("DisplayDiscountSum", displaydiscountsumCheckBox);
    FillParam("DisplayDiscountNameString", displaydiscountnamestringComboBox);
    FillParam("DisplayDiscountSumString", displaydiscountsumstringComboBox);
    
    FillParam("DisplayDiscountUpStringRun", displaydiscountupstringrunComboBox);
    FillParam("DisplayDiscountDownStringRun", displaydiscountdownstringrunComboBox);
    
    // ККМ
    // синхронизация по времени
    FillParam("Synchro", synchroCheckBox);
    FillParam("AutoSynchro", autosynchroCheckBox);
    FillParam("SynchroFrom", synchrofromSpinBox);
    FillParam("SynchroTo", synchrotoSpinBox);
    FillParam("NotifySynchroTimeField", notifysynchrotimefieldCheckBox);
    FillParam("NotifySynchroMessage", notifysynchromessageCheckBox);
    
    // открытие денежного ящика
    FillParam("OpenBoxByRest", openboxbyrestCheckBox);
    FillParam("OpenBoxByCloseCheque", openboxbyclosechequeCheckBox);
    FillParam("OpenBoxByChangeMoney", openboxbychangemoneyCheckBox);
    FillParam("OpenBoxByClearReport", openboxbyclearreportCheckBox);
    
    
    
    int k = test->GetParamID(param_id, param_names, param_count);
    
    //   -1 - error
    if (k !=0)  return k;
    
    // выделение памяти под хранение значений параметров
    param_values = (char**) calloc(param_count, sizeof(char*));
    for (int i=0; i<param_count; ++i)  {
	param_values[i] = (char*) calloc(45, sizeof(char));
    }

    if (nameschemeComboBox->count()==0) {
	//если количество схем = 0, то загрузить параметры по умолчанию
	k = test->GetDefaultParam(param_values, param_id, param_count);
	if ( k < 0) return k;
    } else  {
	//иначе заоплнить параметры из выделенной схемы
	QString scheme_name = codec_sys->fromUnicode( nameschemeComboBox->currentText() );
	k = GetFromDB((char*) scheme_name.ascii());
	if (k < 0 ) return k;
    }
    
    //передача значений параметров  на форму
    SetParameters();
    printf ("	*********************End Setup::InitParam*******************************\n\n");
    return 0;
}
//======================================================//
void Setup::errorMessage(const char * str) {
    releaseKeyboard();
    QMessageBox::information( this, codec_sys->toUnicode("Настройка приложения"), codec_sys->toUnicode(str), QMessageBox::Ok  );
    grabKeyboard();
}
//======================================================//
void Setup::errorMessage(int er)
{
    QString mes_id, mes_str;
    SetupErrorText(er, mes_str, mes_id);
    
    releaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    InterfaceErrorWin(mes_str, mes_id);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(SETUP);
    grabKeyboard();
}
//======================================================//
// заполнение контрольных элементов значениями из массива параметров
void Setup::FillControl(char* value, QWidget*& control) 
{
    
    if ( control == showabsentgoodsCheckBox ) {
	if ( atoi(value) == 1 ) ( (QCheckBox*) control)->setChecked(false); else ( (QCheckBox*) control)->setChecked(true);
	return;
    }
    
    if (strcmp(control->className(), "QLineEdit") == 0)    {
	// обработка QLineEdit
	( (QLineEdit*) control)->setText(QString(codec_db->toUnicode(value)));
	return;    }
    
    if (strcmp(control->className(), "QSpinBox") == 0 )  {
	// обработка для QSpinBox
	( (QSpinBox*) control )->setValue(atoi(value));
	return;   }
    
    if (strcmp(control->className(), "DoubleSpinBox") == 0)  {
	// обработка для QSpinBox
	( (QSpinBox*) control )->setValue( (int) (atof(value)*100));
	return;   }
    
    if (strcmp(control->className(), "QCheckBox") == 0)   {
	// обработка для QCheckBox
	//printf("name of check box = %s \n", control->name());
	if (atoi(value) == 1) ( (QCheckBox*) control)->setChecked(true); else ( (QCheckBox*) control)->setChecked(false);
	return;   }
    
    if (strcmp(control->className(), "QComboBox") == 0)   {
	// обработка для QComboBox
	( (QComboBox*) control)->setCurrentItem(atoi(value));
	return;    }
    
    if (strcmp(control->className(), "QGroupBox") == 0 ) {
	// обработка для QGroupBox
	if ( ((QGroupBox*) control)->isCheckable() ) ( (QGroupBox*) control)->setChecked(strcmp(value, "1")==0 ? true : false);
	return;    }
    
    if (strcmp(control->className(), "QTextEdit") == 0)    {
	// обработка QTextEdit
	QString str = QString( codec_db->toUnicode(value) ) ;
	if (control == advertisementTextEdit) str = AdvertisementFromDB(value);
	( (QTextEdit*) control)->setText( str );
	return;    }
    
    if ( control == basediscountorderTable ) {
	
	setBaseDiscountOrderEnabled( resultdiscountComboBox->currentText() == codec_sys->toUnicode("Объединяется")); 
	if ( strlen(value) != CountDiscountOrder ) return;
	
	char * tmp = (char*) calloc(5, sizeof(char));
	for (uint i = 0; i < CountDiscountOrder; i++) {
	    *(tmp) = *(value + i);
	    if ( atoi(tmp) > (int)CountDiscountOrder ) break;
	    basediscountorderTable->setText( i, 0, codec_sys->toUnicode( DiscountOrder[ atoi(tmp)-1 ] )) ;	}
	free(tmp);
    }
    
    
}
//======================================================//
// эта процедура удаляет пробелы для поля рекламы
QString Setup::AdvertisementFromDB(char* instr)
{    
    uint i = 0;
    int k = 0;
    char * str = (char* ) calloc ( strlen(instr) + 5, sizeof(char));
    
    while ( i<strlen(instr) )   {
	if (k  > MaxLen_ChequeText-1 && instr[i] == 10)   k = 0;  else *(str + strlen(str)) = instr[i] ; 
	k++;	i++;
    }
    return codec_sys->toUnicode(str);
}
//======================================================//
// показать данные на форме
void Setup::SetParameters() {
    puts("******Setup::SetParameters()******");
    
    
     //printf("берем значения из value и заполняем елементыменты \n");
    for (int i = 0; i < param_count; i++)   {
	//printf("param_values[%d] = %s \n", i, param_values[i]);
	FillControl(param_values[i], controls[i]);  
    }

    // фиксированные скидки
    //printf("count of fixed discount = %d \n", dis_count);
    fixeddiscountTable->setNumRows(dis_count);
    
    for (int i=0; i < dis_count; i++)   {
	//printf("%d : name = %s, text = %s, type = %s, value = %s, fixing = %s barcode = %s cardcode = %s \n", i, discounts[i].name, discounts[i].text, discounts[i].type, discounts[i].value, discounts[i].fixing, discounts[i].barcode, discounts[i].cardcode );
	fixeddiscountTable->setText(i, 0, codec_db->toUnicode(discounts[i].external_id) );
	fixeddiscountTable->setText(i, 1, codec_db->toUnicode(discounts[i].name) );
	fixeddiscountTable->setText(i, 2, codec_db->toUnicode(discounts[i].text) );
	fixeddiscountTable->setText(i, 3, fdtypeComboBox->text(atoi(discounts[i].type)));
	//QString dstr = QString::number(atoi(discounts[i].value)); 
	//printf("*(discounts[i].value+0) = <%c> = %d, *(discounts[i].value+1) = <%c> = %d, '-' = %d \n", *(discounts[i].value), *(discounts[i].value), *(discounts[i].value+1), *(discounts[i].value+1), '-');
	if ( (atoi(discounts[i].type) > 1) && *(discounts[i].value) == '-' ) {
	    // удалить первый символ - "-"
	    int lenstr = strlen(discounts[i].value);
	    for (int k = 0; k != lenstr; k++) discounts[i].value[k] = discounts[i].value[k+1];	}
	fixeddiscountTable->setText(i, 4, discounts[i].value);
	fixeddiscountTable->setText(i, 5, fdfixingComboBox->text( strcmp(discounts[i].fixing, "t") == 0 ? 1 : 0 ) );
	fixeddiscountTable->setText(i, 6, discounts[i].barcode);
	fixeddiscountTable->setText(i, 7, discounts[i].cardcode); 
	fixeddiscountTable->setText(i, 8, discounts[i].show);
    }
    
    if ( fixeddiscountTable->numRows() == 0 )  fixeddiscountNotAvaiable(); 
    else fixeddiscountTable->setCurrentCell( 0, 0 );
    
    //printf("count of sambar = %d \n", sambar_count);
    samplebarcodeTable->setNumRows( sambar_count );
    for (int i=0; i<sambar_count; i++) {
	//printf("%d: pref = <%s> , sam = <%s>, mult = <%s> \n", i, sambars[i].prefix, sambars[i].sample, sambars[i].multiplier );
	samplebarcodeTable->setText(i, 0, codec_db->toUnicode( sambars[i].prefix ));
	samplebarcodeTable->setText(i, 1, codec_db->toUnicode( sambars[i].sample )); 
	samplebarcodeTable->setText(i, 2, codec_db->toUnicode( sambars[i].multiplier )); 
    }
     if ( samplebarcodeTable->numRows() == 0 ) samplebarcodeNotAvaiable();
     else samplebarcodeTable->setCurrentCell( 0 , 0 ); 
    
    deletePushButton->setEnabled( strcmp(nameschemeComboBox->currentText().ascii(), actscheme) );
    
    QString scheme_name = codec_sys->fromUnicode( nameschemeComboBox->currentText() );
    
    if ( docList->Load(nameschemeComboBox->currentText()) != 0 ) errorMessage(30);
    
    puts("		**********End Setup::SetParametrs()******");
}
//======================================================//
// взятие значения параметра из контрольных элементов
void Setup::FillValue(QWidget* control, char*& value)
{
    if  ( control == showabsentgoodsCheckBox ) {
	strcpy(value, ((QCheckBox*) control)->isChecked() ? "0" : "1");
	return;
    }
    
    if (strcmp(control->className(), "QLineEdit") == 0)    {
	// обработка QLineEdit
	/*QString str = codec_sys->fromUnicode(  ( (QLineEdit*) control )->text() );
	value = (char*) realloc(value, sizeof(char)*(str.length() + 2));*/
	QString str = codec_db->fromUnicode( ( (QLineEdit*) control )->text() );
	value = (char*) realloc(value, sizeof(char)*(str.length() + 2));
	strcpy(value, str);
	//strcpy(value, str.ascii());
	return;    }
    
    if (strcmp(control->className(), "QSpinBox") == 0)   {
	//printf("%s  = %d \n",  control->name(), ( (QSpinBox*) control )->value());
	// обработка для QSpinBox 
	value = (char*) realloc(value, sizeof(char) * (QString::number(( (QSpinBox*) control )->value()).length() + 1));
	strcpy(value, QString::number( ((QSpinBox*) control)->value()));
	return;    }
    
    if (strcmp(control->className(), "DoubleSpinBox") == 0)   {
	//printf("%s  = %d \n",  control->name(), ( (QSpinBox*) control )->value());
	 // обработка для DoubleSpinBox 
	value = (char*) realloc(value, sizeof(char) *((QSpinBox*)control)->text().length() +1);
	//(QString::number(( (QSpinBox*) control )->value()).length() + 1));
	strcpy(value, ((QSpinBox*)control)->text());
	//printf("value for double spin box = %s \n", value);
	return;    }
    
    if (strcmp(control->className(), "QCheckBox") == 0)    {
	// обработка для QCheckBox
	strcpy(value, ((QCheckBox*) control)->isChecked() ? "1" : "0");
	return;    }
    
    if (strcmp(control->className(), "QComboBox") == 0)    {
	//обработкп для QComboBox
	value = (char*) realloc(value, sizeof(char) * (QString::number(( (QComboBox*) control )->currentItem()).length() + 1));
	strcpy(value, QString::number( ((QComboBox*) control)->currentItem()) );
	return;    }
    
    if (strcmp(control->className(), "QGroupBox") == 0)    {
	//обработкп для checkabled QGroupBox 
	strcpy(value, ((QGroupBox*) control)->isChecked() ? "1" : "0" );
	return;    }
    
    if (strcmp(control->className(), "QTextEdit") == 0)    {
	// обработка QTextEdit
	QString str = ( (QTextEdit*) control )->text();
	if (control == advertisementTextEdit) str = AdvertisementToDB(str);
	str = codec_db->fromUnicode(str);
	value = (char*) realloc(value, sizeof(char)*(str.length() + 2));
	strcpy(value, str.ascii());
	return;    }
	
    if  ( control == basediscountorderTable ) {
	 value = (char*) realloc(value, sizeof(char)*CountDiscountOrder + 1);
	 for (int i=0; i<basediscountorderTable->numRows(); i++) {
	     uint j = 0;
	     QString str = codec_sys->fromUnicode(basediscountorderTable->text(i, 0));
	     while ((str != QString(DiscountOrder[j]) ) && (j<CountDiscountOrder)) 
		j++;
	     *( value+i ) = 49+j;
	     //printf("value = %s \n", value);
	 }
    }
}
//======================================================//
// вставляет пробелы в поле рекламы
QString Setup::AdvertisementToDB(QString str) {
    int k = 0;
    for (uint i = 0; i < str.length(); i++ ) {
	if (str[i] != QChar('\n') ) k++; else k = 0;
	if ( k > MaxLen_ChequeText ) { 
	    str.insert(i, "\n"); 
	    k = 0; }
    }
    return str;
}
//======================================================//
// заполнение массива значений из контролов
void Setup::GetParameters() {
    puts("		******************Setup::GetParameters()**********");
    // заполнение массива значений из контролов
    for (int i = 0; i < param_count; i++)    { FillValue( controls[i], param_values[i] );    }
    
    puts("		*************End Setup::GetParameters()*******");
}
//=========================================================//
void Setup::maxmoneyLineEdit_EditEnd()
{
    QString s = QString::number(maxmoneyLineEdit->text().toDouble(), 'f', 2);
    maxmoneyLineEdit->setText(s);
}
//=========================================================//
void Setup::DoubleTextLostFocus()
{
    //puts("-----------------------------dddddddaaaaaa-------------------------");
    QWidget * focus_widget = this->focusData()->focusWidget();
    
    QString text;
    
    if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) { 
	//puts("-----------------lineedit------------------");
	text = ((QLineEdit*)focus_widget)->text();
	double d = text.toDouble();
	//printf("d = %f \n", d);
	text = QString::number(d, 'f', 2);
	
//	if ( text.isEmpty() ) return;
//	if ( str == QString(".") ) return 0;
//	str = QString::number( , 'f', 2);
	//printf("str = %s \n", str.ascii());
	//int res = str.toInt();
	
	((QLineEdit*)focus_widget)->setText(text);
    }
    
/*    
    if ( strcmp(focus_widget->className(), "QSpinBox" ) == 0 ) {
	
    }
    
    if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0  ) { 
	((QLineEdit*)focus_widget)->setText(str);	
    }	
    
    QString str = text();
    //printf("text of double spin box = %s \n", str.ascii());
    //printf("text of double spin box = %s \n", str.ascii()):
    if ( str.isEmpty() ) return 0;
    if ( str == QString(".") ) return 0;
    str = QString::number( 100*str.toDouble(ok), 'f', 0);
    //printf("str = %s \n", str.ascii());
    int res = str.toInt();*/
}
//=========================================================//
void Setup::CommaTest( const QString & kstr)
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
//=========================================================//
void Setup::makeKeyPress(QKeyEvent * k) 
{
    QWidget * focus_widget = this->focusData()->focusWidget();
    
    //printf("focusWidget Class Name = %s \n", focus_widget->className());
    
    //printf("focusWidget->parent->class name = %s \n", focus_widget->parent()->className())    
    if ( ! this->isActiveWindow()  ) return;
    
    switch(k->key())    {
        case Qt::Key_F12 : //ВЫХОД
	close();
	break;
    case Qt::Key_Enter :
	//printf("Key_Enter\n");
	Enter_Handler();
	if ( strcmp(focus_widget->className(), "QPushButton" ) == 0 )   ((QPushButton*)focus_widget)->animateClick();
	if ( focus_widget == documentpathPushButton )    SetDocumentPath();
	break;	
    case Qt::Key_Return :
	//printf("Key_Enter\n");
	Enter_Handler();
	if ( focus_widget == documentpathPushButton )    SetDocumentPath();
	if ( strcmp(focus_widget->className(), "QPushButton" ) == 0 )   ((QPushButton*)focus_widget)->animateClick();
	break;	
    case Qt::Key_Escape :
	break;
    case Qt::Key_Tab :
	//printf("Key_Tab\n");
	focusData()->next();
	break;	
    case Qt::Key_Space :
	//printf("Key_Space\n");
	Space_Handler();
	if ( strcmp(focus_widget->className(), "QPushButton" ) == 0 )   ((QPushButton*)focus_widget)->animateClick();
	
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) {    ((QLineEdit*)focus_widget)->insert(k->text());   }
	if ( focus_widget == documentpathPushButton )    SetDocumentPath();
	
	if ( strcmp(focus_widget->className(), "QCheckBox") == 0 ) {
	    //puts("qcheckbox space press");
	    //((QCheckBox *) focus_widget)->setChecked(((QCheckBox *) focus_widget)->isChecked());
	    ((QCheckBox *) focus_widget)->toggle();
	}
	
	break;	
	
      case Qt::Key_Left :
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)focus_widget)->cursorBackward( k->state() == Qt::ShiftButton );	    }
	if ( strcmp(focus_widget->className(), this->className()) == 0 ) break;
	if ( strcmp(focus_widget->parent()->className(), "QTabWidget") == 0 ) 
	    ((QTabWidget*) focus_widget->parent())->setCurrentPage(((QTabWidget*) focus_widget->parent())->currentPageIndex() - 1);
	break;
    case Qt::Key_Right :
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)focus_widget)->cursorForward( k->state() == Qt::ShiftButton );	    }
	if ( strcmp(focus_widget->className(), this->className()) == 0 ) break;
	if ( strcmp(focus_widget->parent()->className(), "QTabWidget") == 0 ) 
	    ((QTabWidget*) focus_widget->parent())->setCurrentPage(((QTabWidget*) focus_widget->parent())->currentPageIndex() + 1);
	break;	
    case Qt::Key_Up :
	if ( strcmp(focus_widget->className(), "QToolBox") == 0 ) 
	    ((QToolBox*) focus_widget)->setCurrentIndex(((QToolBox*) focus_widget)->currentIndex()-1);
	if ( strcmp(focus_widget->className(), "QComboBox" ) == 0 )    
	    ((QComboBox*)focus_widget)->setCurrentItem( ((QComboBox*)focus_widget)->currentItem() - 1 );  
	if ( strcmp(focus_widget->className(), this->className()) == 0 ) break;
	if ( strcmp(focus_widget->name(), "qt_spinbox_edit" ) == 0 )    {
	    QWidget * parent_widget = (QWidget*) focus_widget->parent();
	    parent_widget = (QWidget*) parent_widget->parent();
	    //printf("parent_widget->className() = %s \n", parent_widget->className());
	    if ( strcmp( parent_widget->className(), "QSpinBox" ) == 0 ||  strcmp( parent_widget->className(), "DoubleSpinBox" ) == 0 ) {
		((QSpinBox*)parent_widget)->stepUp();
	    }
	}
	if ( strcmp(focus_widget->className(), "UpDownLineEdit") == 0 ) {
	    ((UpDownLineEdit * ) focus_widget)->GoUp();
	}
	
	break;	
    case Qt::Key_Down :
	//printf("Key_Down\n");
	if ( strcmp(focus_widget->className(), "QToolBox") == 0 ) 
	    ((QToolBox*) focus_widget)->setCurrentIndex(((QToolBox*) focus_widget)->currentIndex()+1);
	if ( strcmp(focus_widget->className(), "QComboBox" ) == 0 )    
	    ((QComboBox*)focus_widget)->setCurrentItem( ((QComboBox*)focus_widget)->currentItem() + 1 );  
	if ( strcmp(focus_widget->className(), this->className()) == 0 ) break;
	if ( strcmp(focus_widget->name(), "qt_spinbox_edit" ) == 0 )    {
	    QWidget * parent_widget = (QWidget*) focus_widget->parent();
	    parent_widget = (QWidget*) parent_widget->parent();
	    if ( strcmp( parent_widget->className(), "QSpinBox" ) == 0 ||  strcmp( parent_widget->className(), "DoubleSpinBox" ) == 0 ) {
		((QSpinBox*)parent_widget)->stepDown();
	    }
	}
	if ( strcmp(focus_widget->className(), "UpDownLineEdit") == 0 ) {
	    ((UpDownLineEdit * ) focus_widget)->GoDown();
	}
	break;	
    case Qt::Key_Delete :	
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 )  ((QLineEdit*)focus_widget)->del();  
	break;
    case Qt::Key_Home :
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) 	
	    ((QLineEdit*)focus_widget)->home( k->state() == Qt::ShiftButton ); 
	break;
    case Qt::Key_End :
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) 
	    ((QLineEdit*)focus_widget)->end( k->state() == Qt::ShiftButton );	    
	break;
    case Qt::Key_Backspace :	
	if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 )   ((QLineEdit*)focus_widget)->backspace();   
	break;	
    case Qt::Key_Shift : break;
    case Qt::Key_Control : break;
    case Qt::Key_Alt : break;	
    default : 
	 if ( strcmp(focus_widget->className(), "QLineEdit" ) == 0 ) {    ((QLineEdit*)focus_widget)->insert(k->text());   }  
	 //puts("other");
    }

}	
//==================================================================================//
void Setup::GetKeySequence()
{
   // puts("-----------------GetKeySequence()---------------------");

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
//======================================================//
void Setup::keyPressEvent(QKeyEvent * k)
{  
   //puts("void Setup::keyPressEvent(QKeyEvent * k)");
   // printf("key code = %d \n", k->key());
    
    int state = 0;
    
    if ( hw->BCS_GetType() == 2  )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	    char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
	    strcpy(barcode, barcode_str);
	    // printf("wtrix kod = <%s> \n", barcode);
	    if ( paramTabWidget->currentPageIndex() ==  3 && DiscountToolBox->currentIndex() ==  0 && fdbarcodeLineEdit->isEnabled() ) 
		 fdbarcodeLineEdit->setText(barcode);
	    free(barcode);
	}
    }
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    int num_track;
	    QString cardcode_str = ((KeyboardReader*) kbcode_reader)->getCode(num_track);
	    char* cardcode = (char*) calloc(cardcode_str.length()+1, sizeof(char));
	    strcpy(cardcode, cardcode_str);
	   // printf("card code = <%s> \n", cardcode);
	    //printf("num_track = %d \n", num_track);
	     if ( paramTabWidget->currentPageIndex() ==  3 && DiscountToolBox->currentIndex() ==  0 && fdcardcodeLineEdit->isEnabled() ) 
		 if (num_track == 2) fdcardcodeLineEdit->setText(cardcode);
	    free(cardcode);
	}
    }
    
    //printf("state of find code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )  {  GetKeySequence();  return;  }
    
}	
//======================================================//
void Setup::CloseForm() {   
    printf("*****************************CloseForm()***************************** \n");
    
    // сохранить изменения ?
     int k = questionSaveScheme();
    //если ерроер, сказать что еррор
    if ( k < 0 )   { 
	errorMessage(7 ); 
	return;
    }
    
    // освобождение памяти
   // printf("free pointers\n");
    
    if (docList != NULL) delete docList;
    
    int i = param_count;
    while (--i>=0)    {
	//printf("%s free \n", param_names[i]);
	free(param_names[i]);    }
    
    free(param_names);
    //printf("param_names free()\n");
    
    free(param_id);
    //printf("param_id free()\n");
    
    //printf("actscheme = %s \n", actscheme);
    free(actscheme);
    //printf("actscheme free()\n");
    
    i = param_count;
    while (--i>=0)    {
	//printf("param_values[%d] = %s  \n", i, param_values[i]);
	free(param_values[i]); 
	//printf("its free");
    }
    
    free(param_values);
    //printf("param_values free()\n");
    
    delete controls;
    
    delete maxstanddiscountDoubleSpinBox;
    delete maxstandincreaseDoubleSpinBox;
    delete maxchequediscountDoubleSpinBox;
    delete maxchequeincreaseDoubleSpinBox;
    
    delete synchrofromSuffixLabel;
    delete synchrotoSuffixLabel;
    delete durationseansSuffixLabel;
    delete warnaboutendSuffixLabel;
    delete autoblocktimeSuffixLabel;
    
    
    for (int i=BARCODE_LENGTH-1; i>=0; i--) delete sample_barcode[i];
    
    //printf("free controls\n");
    
    delete test;
    delete dis_db;
    delete sam_db;
    
    
    
    puts("*********************************End CloseForm()*********************************");
    accept();
}
//======================================================//
void Setup::closeEvent(QCloseEvent * ce) {
    CloseForm();
    //close();
    //accept();
    
}
//======================================================//
void Setup::destroy()  { 
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    puts("destroy---ok");
}
//======================================================//
// 0 все ОК
// <0 - еррор
// заполнить массив параметров данныи из БД
int Setup::GetFromDB(char * scheme_name)
{
    //printf("Получение параметров от схемы (%s)\n", scheme_name);
   // puts("-------------------------------------v KOI8R----------------------------------------------------");    
    char* sn_tmp_codec = (char*) calloc(strlen(scheme_name)+3, sizeof(char));
    QString str = codec_db->fromUnicode(codec_sys->toUnicode(scheme_name));
    strcpy(sn_tmp_codec, str);
    //printf("scheme_name = %s \n", scheme_name);
    //printf("sn_tmp_codec = %s \n", sn_tmp_codec);
    int k = test->GetSchemeParam(sn_tmp_codec, param_values, param_id, param_count);
    
    if (k < 0) return k;
    
    k = dis_db->GetCountDiscounts(sn_tmp_codec, &dis_count);
    if (k < 0) return k;
    
    k = sam_db->GetCountSamples(sn_tmp_codec, sambar_count);
    if ( k < 0)  return k;
    
    //printf("количество фиксированных скидок = %d\n", dis_count);
    if (dis_count ==0)  { k = 0; }else {
	discounts = (TDiscount* ) calloc(dis_count + 2, sizeof(TDiscount));
	k = dis_db->GetDiscounts(sn_tmp_codec, discounts, dis_count);	
	if ( k < 0 ) return k;    }
    
    if (sambar_count ==0) return 0;
    sambars = (TSampleBarcode* ) calloc(sambar_count+2, sizeof(TSampleBarcode));
    k = sam_db->GetSamples(sn_tmp_codec, sambars, sambar_count);
    if ( k < 0 ) return k;
    
    free(sn_tmp_codec);    
    //puts("------------------------------------------------------was get from db----------------------------------------------------");
    /*for (int i=0; i< sambar_count;i++) {
	printf("sambar[%d] prefix = <%s>, name = <%s>, multi = <%s>\n", i, sambars[i].prefix, sambars[i].sample, sambars[i].multiplier);
    }

    printf("End get scheme param\n");*/
    return 0;
}

//======================================================//
// 0 все ОК
// <0 - еррор
//сохранить массив параметров в БД
int Setup::SetToDB(char* scheme_name)
{
    printf("Сохранение параметров схемы (%s) в БД \n", scheme_name);
    
    char* sn_tmp_codec = (char*) calloc(strlen(scheme_name)+3, sizeof(char));
    QString str = codec_db->fromUnicode(codec_sys->toUnicode(scheme_name));
    strcpy(sn_tmp_codec, str);
    
    //printf("scheme_name = %s \n", scheme_name);
    //printf("sn_tmp_codec = %s \n", sn_tmp_codec);
    
    
    if ( atoi(maxmoneyLineEdit->text()) < 1 ) {
	errorMessage(31);
	return -1;
    }
    
    int k = test->SetSchemeParam(sn_tmp_codec, param_id, param_values, param_count);
    
    if ( k < 0 ) {  errorMessage(8);   return k;  }
    
    //printf("Dis count = %d\n", dis_count);
    for ( int i=0; i<dis_count; i++ ) {
	//-----(*)-----Zuskin-----28/07/2011-----
	//if ( atoi(discounts[i].type) > 1 && *(discounts[i].value) != '-' ) {
	if ( atoi(discounts[i].type) > 1 && atoi(discounts[i].type) < 4 && *(discounts[i].value) != '-' ) {
	//-------------------------------------------------    
	    char* str_tmp = (char*) calloc( strlen(discounts[i].value)+3, sizeof(char) );
	    strcat(str_tmp, "-");
	    strcat(str_tmp, discounts[i].value);
	    strcpy(discounts[i].value, str_tmp);
	    free(str_tmp);
	}
    }
    
    k = dis_db->SaveInScheme(sn_tmp_codec, discounts, dis_count);
    
    printf("dis_db->SaveInScheme = %d \n", k);
    
    if ( k == 8 ) {
	errorMessage(9);
	free(sn_tmp_codec); 
	return -1;  
    }
    if ( k == 9 ) {
	errorMessage(10);
	free(sn_tmp_codec); 
	return -1;  
    }
    if ( k < 0 ) { 
	errorMessage(11);
	free(sn_tmp_codec); 
	return k;  
    }
    
   // puts("------------------------------------------------------to db----------------------------------------------------");
   // for (int i=0; i< sambar_count;i++) {
//	printf("sambar[%d] prefix = <%s>, name = <%s>, multi = <%s>\n", i, sambars[i].prefix, sambars[i].sample, sambars[i].multiplier);    }
    
    k = sam_db->SaveInScheme(sn_tmp_codec, sambars, sambar_count);
    
    if ( k < 0 ) {
	errorMessage(12);
	free(sn_tmp_codec);
	return k;
    }
    
    free(sn_tmp_codec);
    
    if ( docList->Save(scheme_name) != 0 ) {
	errorMessage(29);
	return -1;
    }
    
    
    return 0;
}

//======================================================//
// переименование схемы, проверка на уникальность имени
int Setup::testRenameScheme()
{
    // Если имя схемы было изменено, переименовать ее 
    QString str = nameschemeComboBox->currentText();
    
    if (str.length() == 0) {
	//ругнуться
	//printf("null scheme name\n");
	int act_num = nameschemeComboBox->currentItem();
	nameschemeComboBox->setCurrentItem(act_num);;
	//puts("null");
	return 0;
    }
    
    //printf("str =  %s  *** oldName =%s \n", str.ascii(), oldName.ascii());
    if ( codec_sys->fromUnicode(oldName) != codec_sys->fromUnicode(str) ) {	
	int k = 0;
	//printf("Переименование имени схемы %s на имя %s\n", oldName.ascii(), (str.ascii()));
	
	QString qstr_koi8r = codec_db->fromUnicode(str);
	QString qoldname_koi8r = codec_db->fromUnicode(oldName);
	
	char* str_koi8r = (char*) calloc(2*str.length()+3, sizeof(char));
	char* oldname_koi8r = (char*) calloc(2*oldName.length()+3, sizeof(char));
	
	strcpy(str_koi8r, qstr_koi8r);
	strcpy(oldname_koi8r, qoldname_koi8r);
	
	//printf("str_koi8r = %s \n", str_koi8r);
	//printf("oldname_koi8r = <%s> \n", oldname_koi8r);
	
	str = codec_sys->fromUnicode(str);
	// посмотреть схему с таким именем в БД
	bool name_exist = false;
	
	k = test->CheckSchemeName(str_koi8r, &name_exist);
	if ( k < 0 )  { errorMessage(13);    return -1; }
	
	// если имя существует
	if (name_exist) {
	    //printf("Такое имя уже существует в БД\n");
	    errorMessage(14);
	    return 1; } 
	
	//изменить данные в БД
	//k = test->RenameScheme( oldNamestr.ascii(), tmp);
	//printf("new name  = %s \n", str_koi8r);
	//printf("old name = %s \n", oldname_koi8r);
	//printf("------------------old name = -------------- %s---------------tmp = -----------------%s \n ", stmp_codec, stmp_codec);
	k = test->RenameScheme(oldname_koi8r, str_koi8r);
	
	free(str_koi8r);
	free(oldname_koi8r);
	
	if ( k < 0 )   { errorMessage(15); return -1; }
	
	//printf("rename OK \n");
	
	//изменить запись в списке имен схем
	nameschemeComboBox->changeItem( codec_sys->toUnicode(str.ascii()), selectScheme );
	// переименовать активную схему
	
	if (strcmp(oldName, actscheme) == 0) strcpy(actscheme, str);
	/*5555555555555555555555555555555555555555555555555555555555555555555555*/
	//printf("actscheme name = %s\n", actscheme);
	oldName = codec_sys->toUnicode(str);
	//printf("-----------------------------oldName = str = %s------------------------------\n", oldName.ascii());
	deletePushButton->setEnabled( strcmp(str.ascii(), actscheme) );
	Changed(change);
    }
    return 0;
}
//======================================================//
// кнопочка "Сохранить"
void Setup::savePushButton_clicked() 
{
    ///printf("	********Setup::savePushButton_clicked()*****\n");
    int k = 0;
    if (nameschemeComboBox->count() == 0) {return;}
    
    // посмотреть не изменилась ли имя схемы
    if ( testRenameScheme() != 0 ) return;
       
    
    // Если были введены изменения сохранить их без вопросов
    if (change) {
	//printf("Сохранение параметров схемы\n");    
	//сохранить параметры
	GetParameters();
	QString scheme_name = codec_sys->fromUnicode(nameschemeComboBox->currentText());
	k = SetToDB((char*)scheme_name.ascii());
	if ( k < 0 )  {  errorMessage(16); return ; }
	
	Changed(false);
    }
    
   // printf("			**********End Setup::savePushButton_clicked()*******\n");
   // puts("");
}
//======================================================//
// проверяет имя схема на правильность, для задания имени по умолчанию
// newname - проверяемое имя
// num - номер проверяемого имени, при переименовании схем (для нового имени == -1) 
bool Setup::validSchemeName(QString newname, int num)
{
    if ( newname.isNull()  || newname.isEmpty() ) return false;
    if ( nameschemeComboBox->count() == 0) return true;
    
    int i = 0;
    if ( num == 0 )  i = 1;
    while (   i < nameschemeComboBox->count() ) {
	QString tmp = codec_sys->fromUnicode( nameschemeComboBox->text(i) );
	if (newname ==  tmp) break;
	i++; 
	if (i==num) i++;     }
    
    return  ( i == nameschemeComboBox->count() );
}
//======================================================//
// придумать имя схемы по умолчанию
QString Setup::getDefaultSchemeName()
{
    QString defaultname;
    int i = 0;
    while (  ! validSchemeName (defaultname, -1 ) ) {
	i++;
	defaultname = QString("Схема");
	defaultname.append(QString::number( i ));  
    }
    return defaultname;
}
//======================================================//
// создание новой схемы
void Setup::newPushButton_clicked()
{
   // printf("			***********Setup::newPushButton_clicked()*******\n");
    
    show_message = false;
    int k;
    if ( questionSaveScheme() != 0) return;
    char* sname = NULL;
    sname = (char*) calloc(45,sizeof(char));
    
    // заполнить имя новой схемы по умолчанию
    //test.GetNewSchemeName(sname);
    QString s = getDefaultSchemeName();
    strcpy(sname, s.ascii());
    //printf("sname = <%s> \n", sname);
    
    releaseKeyboard();
    //диалоговое окно для запроса имени новой схемы
    DialogNewScheme* nscheme = new DialogNewScheme();
    nscheme->setNewSchemeName(sname);

    // если ОК, то взять это имя
    if (nscheme->exec() != QDialog::Accepted)    { 
	//puts("Setup::reject");  
	free(sname); 
	return; }
    
    grabKeyboard();
    
    QString scheme_name = nscheme->getSchemeName();
    sname = (char*) realloc(sname, sizeof(char) * ( scheme_name.length() + 1  ) );
    strcpy(sname, scheme_name.ascii());
    //printf("sname = <%s> \n", sname);
    nscheme->close();
    delete nscheme;
    
    if (strlen(sname)==0) {
	errorMessage(17);
	//printf("Имя схемы пусто!!!\n"); 
	free(sname);   
	show_message = true;
	return;    }
    
    char* sname_koi8r = (char*) calloc(2*strlen(sname)+2, sizeof(char));
    QString qsname_koi8r = codec_db->fromUnicode(codec_sys->toUnicode(sname));
    strcpy(sname_koi8r, qsname_koi8r);
    //printf("----------in koi8r-------sname = %s \n", sname_koi8r);
    
    k = test->AddNewScheme(sname_koi8r);
    
    free(sname_koi8r);
    
    if ( k < 0 ) {
	errorMessage(18);
	printf("Схема не была добавлена в БД \n"); 
	free(sname);   
	show_message = true;
	return;     }
    //printf ("если имя уже есть значит k =1; k = %d\n", k);
    if (k == 1) {
	printf("Имя схемы уже занято \n");
	errorMessage(14);
	free(sname);   
	show_message = true;
	return;     }
    
    //printf("имя новой схемы = %s \n", sname);
    // добавить имя новой схемы в общий список схем
    nameschemeComboBox->insertItem(codec_sys->toUnicode(sname), nameschemeComboBox->count()-1);
    nameschemeComboBox->setCurrentText(codec_sys->toUnicode(sname));
    //puts("-----4444444444444444444444-----");
    setdefaultCheckBox->setChecked(false);
    setdefaultCheckBox->setEnabled(true);
    
    // загрузить параметры от новой схемы
    k = GetFromDB(sname);	
    if ( k < 0)    {  errorMessage(2);  }  else    SetParameters();
    
    selectScheme  = nameschemeComboBox->currentItem();
    Changed(false);
    //change = false;
    //savePushButton->setEnabled( change );
    
    oldName = nameschemeComboBox->currentText();
    free(sname);  
    
    
    paramTabWidget->setEnabled(true);
    //setdefaultCheckBox->setEnabled(true);
    if ( schemes_count == 1 )   setdefaultCheckBox_toggled(true);
    else setdefaultCheckBox_toggled(false);
	
    show_message = true;
    
    printf("			******End Setup::newPushButton_clicked()*******\n");
}

//======================================================//
void Setup::deletePushButton_clicked()
{
    //printf("			********Setup::deletePushButton_clicked()*******\n");
    show_message = false;
    if (nameschemeComboBox->count()==0) {return;}
    if ( QMessageBox::question ( this, QString(codec_sys->toUnicode("Настройка приложения")), QString(codec_sys->toUnicode("Вы уверены что хотите удалить текущюю схему?")), QMessageBox::Yes, QMessageBox::Cancel ) != QMessageBox::Yes ) return;  
    
    // если удаляется активная схема, ругаться
    QString tmp = codec_sys->fromUnicode(nameschemeComboBox->currentText());
    if(strcmp(tmp.ascii(), actscheme) == 0)   {
	printf("Попытка удаления активной схемы\n");
	errorMessage(19);
	return;    }
    
    // иначе, удалить схему
    QString scheme_name = codec_sys->fromUnicode(nameschemeComboBox->currentText());

    char* scheme_name_koi8r = (char*) calloc(2*strlen(scheme_name)+2, sizeof(char));
    QString qscheme_name_koi8r = codec_db->fromUnicode(codec_sys->toUnicode(scheme_name));
    strcpy(scheme_name_koi8r, qscheme_name_koi8r);
    
    //printf("scheme_ name -----------in -----------koi8r----------- = %s \n", scheme_name_koi8r);
    //int k = test->DeleteScheme((char*) scheme_name.ascii());
    int k = test->DeleteScheme(scheme_name_koi8r);
    
    free(scheme_name_koi8r);
    
    if ( k < 0 )  {
	//printf("k = res = %d \n", k);
	errorMessage(20);
	return;   }
    
    nameschemeComboBox->removeItem(nameschemeComboBox->currentItem());
    
    // если случайно не осталось ни одной схемы, то загрузить параметры по умолчанию
    if (nameschemeComboBox->count() == 0)    {
	k = test->GetDefaultParam(param_values, param_id, param_count);
	if ( k < 0) {
	    errorMessage(21);
	    return; 	
	}
	SetParameters();
	return;   
    }
    
    // активация самой первой схемы 
    nameschemeComboBox->setCurrentItem(0);
    scheme_name = codec_sys->fromUnicode(nameschemeComboBox->text(0));
    k = GetFromDB((char*) scheme_name.ascii());
    if ( k < 0)   {   errorMessage(22); }
    SetParameters();
    
    // мелочь всякая от новой схемы и др
    setdefaultCheckBox->setEnabled(strcmp((char*) scheme_name.ascii(), actscheme) != 0);
    setdefaultCheckBox->setChecked(strcmp((char*) scheme_name.ascii(), actscheme) == 0);
    
    selectScheme = nameschemeComboBox->currentItem();
    oldName = nameschemeComboBox->currentText();    
    
    Changed(false);
    
    show_message = true;
    //printf("			**********End Setup::deletePushButton_clicked()********\n");
}

//======================================================//
void Setup::setdefaultCheckBox_toggled( bool  b)
{
    printf(" **********Setup::setdefaultCheckBox_toggled( bool  b)********\n");
    // переспросить, хочет ли юзер действительно сделать эту схему активной. 
    int k;
    if ( nameschemeComboBox->count() > 0 && b )    {
	if (show_message)  {
	    QMessageBox mb (QString(codec_sys->toUnicode("Настройка приложения")), 
			    QString(codec_sys->toUnicode("Вы правда хотите сделать текущюю схему схемой по умолчанию?")),
			    QMessageBox::Question,
			    QMessageBox::Yes | QMessageBox::Default,
			    QMessageBox::No  | QMessageBox::Escape,
			    QMessageBox::NoButton );
	    
	    // если да, то ни че не поделаешь, придется сделать ее схемой по умолчанию
	    if ( mb.exec() == QMessageBox::Yes )    {		
		QString scheme_name = codec_sys->fromUnicode(nameschemeComboBox->currentText());
		
		QString qscheme_name_koi8r = codec_db->fromUnicode(codec_sys->toUnicode(scheme_name));
		char* scheme_name_koi8r = (char*) calloc(strlen(scheme_name) + 2, sizeof(char));
		strcpy(scheme_name_koi8r, qscheme_name_koi8r);
		
		//k = test->SetActiveScheme((char*) scheme_name.ascii());
		k = test->SetActiveScheme(scheme_name_koi8r);;
		free(scheme_name_koi8r);
		
		if ( k < 0)	{
		    errorMessage(23);
		    return; 	}
		strcpy(actscheme, (char*) scheme_name.ascii());

		setdefaultCheckBox->setChecked(true);
		setdefaultCheckBox->setEnabled(false);    
		
		deletePushButton->setEnabled( strcmp(nameschemeComboBox->currentText().ascii(), actscheme) );
	    }   else   {
		setdefaultCheckBox->setChecked(false);    }
	    
	} else {
	    setdefaultCheckBox->setChecked(true);
	    setdefaultCheckBox->setEnabled(false);  }
	
    } 
    //printf("			**********End Setup::setdefaultCheckBox_toggled( bool  b)********\n"); 
}
//======================================================//
void Setup::autoblockCheckBox_toggled( bool b) {    autoblocktimeSpinBox->setEnabled( b );  }
//======================================================//
void Setup::blockmouseCheckBox_toggled( bool b) {    useoneclickCheckBox->setEnabled( ! b );}
//======================================================//
/*void Setup::maxclientsumLineEdit_EditEnd() {
    //printf("edit end \n ");
    // хитрый способ оставить только 2 цифры после запятой в денежных полях
    QString s = QString::number(maxclientsumLineEdit->text().toDouble(), 'f', 2);
    maxclientsumLineEdit->setText(s);
}*/
//======================================================//
/*void Setup::maxmoneyboxLineEdit_EditEnd() {
    //printf("edit end \n");
    // хитрый способ оставить только 2 цифры после запятой в денежных полях
    QString s = QString::number(maxmoneyboxLineEdit->text().toDouble(), 'f', 2);
    maxmoneyboxLineEdit->setText(s);
}*/
//======================================================//
// спрашивает "Сохранить ли изменения сх-емы" если изменения не были сохранены явно
// возвращает 0 если все хорошо,
// -1 если какой нибудь еррор,
int Setup::questionSaveScheme()  
{
    //printf("int Setup::SaveCurrentScheme()\n");
    if (change)   {		
	// "Некоторые параметры были изменены, сохранить изменения?"
	printf("были внесены изменения\n");
	QString sn = codec_sys->fromUnicode(nameschemeComboBox->text(selectScheme));
	releaseKeyboard();
	int res = QMessageBox::question(this, QString(codec_sys->toUnicode("Настройка приложения")), 
				  QString(codec_sys->toUnicode("Некоторые параметры схемы '"+ sn 
						       +"' были изменены. Сохранить изменения?")),
				  QMessageBox::Yes, 
				  QMessageBox::No);
	grabKeyboard();
	if ( res == QMessageBox::Yes)  {
	    // посевить текущюю схему
	    //printf("Сохранение схемы\n");
	    
	    int k;
	    GetParameters();
	    oldName = codec_sys->fromUnicode(oldName);
		k = SetToDB((char*) oldName.ascii());	
	    if ( k < 0)  {  errorMessage(16); return -1;	}
	    Changed(false);
	} 
    }
    printf("никто ничего не менял !!! ");
    return 0;
}
//======================================================//
// при изменении значения любого параметра
// реагирует на изменеие значения любого элемента. Одна для всех 
void Setup::ParametersChanged() { 
    //puts("change");
    Changed(true);
}

//======================================================//
void Setup::nameschemeComboBox_activated(const QString & str) {
    QString string = codec_sys->fromUnicode(str);     
    //printf("		********Setup::nameschemeComboBox_activated(%s)******\n", string.ascii());
    
    char* str_utf = (char*) calloc( string.length()+1, sizeof(char));
    strcpy(str_utf, string);
    //printf("str utf = %s\n", str_utf);
    
    
    //printf("s");
    deletePushButton->setEnabled( strcmp(str_utf, actscheme) );
    //puts("sdf-----------11111111111-----------");
    //если не было никаких изменений и выделенной осталась старая схема, то ничё  не делать
    if ((!change)&&(nameschemeComboBox->currentItem() == selectScheme))    return;  
   // printf("select Scheme = %d \n", selectScheme);
    // на случай если будет выделена активная схема, не спрашивать юзера соглачсен ли он сделать активную схему активной
    show_message = false;
    int k ;
    //printf("Save Scheme\n");
    k = questionSaveScheme();
    //если ерроер, сказать что еррор, вернуть как было
    if ( k < 0 )   {
	errorMessage(7);
	nameschemeComboBox->setCurrentItem(selectScheme);
	show_message = true;
	oldName = str;
	//puts("		**********End Setup::nameschemeComboBox_activated()********");
	return;  }
    
    // загрузить параметры новой схемы
    k = GetFromDB((char*)string.ascii());
    if ( k < 0 )    {
	errorMessage(24);
	nameschemeComboBox->setCurrentItem(selectScheme);
	oldName = str;
	//puts("		**********End Setup::nameschemeComboBox_activated()********");
	return;   }
    
    SetParameters();
    
    // мелочь всякая от новой схемы и др    
    string = codec_sys->fromUnicode(nameschemeComboBox->currentText());
    if (strcmp((char*) string.ascii(), actscheme) != 0)    {
	setdefaultCheckBox->setEnabled(true);
	setdefaultCheckBox->setChecked(false);
    } else   {
	setdefaultCheckBox->setChecked(true);
	setdefaultCheckBox->setEnabled(false);
    }
    selectScheme  = nameschemeComboBox->currentItem();
    show_message = true;
    
    Changed(false);
    oldName = str;
    
    fixeddiscountTable->setCurrentCell(0,1);
    editdocumentPushButton->setEnabled(documentTable->currentRow() != -1);
    removedocumentPushButton->setEnabled(documentTable->currentRow() != -1);
    
   // puts("		**********End Setup::nameschemeComboBox_activated()********");
}
//================================================================//
void Setup::Enter_Handler()  {    testRenameScheme();	}
//================================================================//
void Setup::autotakereportCheckBox_toggled( bool b)  {
    reportGroupBox->setEnabled(b);
    ParametersChanged();
}
//================================================================//	
void Setup::testDiscountType()
{
    //printf("void Setup::testDiscountType() \n");
    fdGroupBox->setTitle( (fdtypeComboBox->currentText() == QString("- %") || 
			 fdtypeComboBox->currentText() == QString("- S") )? codec_sys->toUnicode( "Скидка" ) : codec_sys->toUnicode( "Надбавка" ) );
    //-----(+)-----Zuskin-----28/07/2011-----
    
    if (fdtypeComboBox->currentText() == codec_sys->toUnicode( "бонус")) {
	fdGroupBox->setTitle(codec_sys->toUnicode( "Бонус" )); 
    }
    //--------------------------------------------------
    //-----(*)-----Zuskin-----28/07/2011-----
    //if (fdtypeComboBox->currentText() == QString("- %") || fdtypeComboBox->currentText() == QString("+ %") ) {
    if (fdtypeComboBox->currentText() == QString("- %") || fdtypeComboBox->currentText() == QString("+ %") || fdtypeComboBox->currentText() == QString("бонус")) {
    //-------------------------------------------------	
	fdvalueLineEdit->setValidator( val_doubleprocent );
	int pos = 0;
	QString s =  fdvalueLineEdit->text();  
	if  ( val_doubleprocent->validate( s , pos ) != QValidator::Acceptable ) 
	   fdvalueLineEdit->setText("10.00");
	fdvalueLineEdit->setMaxLength(MaxLen_ProcentText); 
    } else {
	fdvalueLineEdit->setValidator( val_doublemoney);
	fdvalueLineEdit->setMaxLength(MaxLen_MoneyText); 
    }
    
}
//================================================================//
void Setup::SetDiscountControls(QString code, QString name, QString text,  QString type, QString value, QString fixing, QString barcode, QString cardcode, bool show)
{
    //puts("void Setup::SetDiscountControls(QString name, QString text, QString type, QString value, QString fixing)");
    //printf("void Setup::SetDiscountControls(QString name, QString text, QString type, %s, QString fixing) \n", value.ascii());
    //printf("value string = %s \n", value.ascii());
    
    fdnameLineEdit->setText(name);
    fdcodeLineEdit->setText(code);
    fdtextLineEdit->setText(text);
    fdtypeComboBox->setCurrentText(type);
    fdvalueLineEdit->setText(value);
    fdfixingComboBox->setCurrentText(fixing);
    fdbarcodeLineEdit->setText(barcode);
    fdcardcodeLineEdit->setText(cardcode);
    fdshowCheckBox->setChecked(show);
    
    testDiscountType();
}

//================================================================//
void Setup::SaveDiscount(int num)
{
  /*  puts("void Setup::SaveDiscount(int num)\n");
    if (num < 0)  return;
    fixeddiscountTable->setText(num, 0, fdnameLineEdit->text());
    fixeddiscountTable->setText(num, 1, fdtextLineEdit->text());
    fixeddiscountTable->setText(num, 2, fdtypeComboBox->currentText());
    fixeddiscountTable->setText(num, 3, fdvalueLineEdit->text());
    fixeddiscountTable->setText(num, 4, fdfixingComboBox->currentText());
    
    // вот здесь вот
    //QString tmp = codec_sys->fromUnicode(fdnameLineEdit->text());
    QString qstr_koi8r = codec_db->fromUnicode(fdnameLineEdit->text());
    strcpy( discounts[num].name, qstr_koi8r);
    //tmp = codec_sys->fromUnicode(fdtextLineEdit->text());
    qstr_koi8r = codec_db->fromUnicode(fdtextLineEdit->text());
    strcpy( discounts[num].text,  qstr_koi8r);
    strcpy( discounts[num].type, itoa(fdtypeComboBox->currentItem()));
    printf("the %d item is select \n", fdtypeComboBox->currentItem());
    //strcpy(discounts[num].type , fdtypeComboBox->currentText() );
    strcpy(discounts[num].value, "");
    //if ( fdtypeComboBox->currentItem() > 1 ) {  puts("надбавочка");  strcat( discounts[num].value, "-" ); }
    strcat( discounts[num].value, fdvalueLineEdit->text() );
    printf("type  of discount = %s, value = %s \n", discounts[num].type, discounts[num].value);
    //printf("current item = %d \n", fdfixingComboBox->currentItem());
    strcpy( discounts[num].fixing, itoa(fdfixingComboBox->currentItem()) );*/
    
        //puts("void Setup::SaveDiscount(int num)\n");
    if (num < 0)  return;
    fixeddiscountTable->setText(num, 0, fdcodeLineEdit->text());
    fixeddiscountTable->setText(num, 1, fdnameLineEdit->text());
    fixeddiscountTable->setText(num, 2, fdtextLineEdit->text());
    fixeddiscountTable->setText(num, 3, fdtypeComboBox->currentText());
    fixeddiscountTable->setText(num, 4, fdvalueLineEdit->text());
    fixeddiscountTable->setText(num, 5, fdfixingComboBox->currentText());
    fixeddiscountTable->setText(num, 6, fdbarcodeLineEdit->text());
    fixeddiscountTable->setText(num, 7, fdcardcodeLineEdit->text());
    if (fdshowCheckBox->isChecked()){
	fixeddiscountTable->setText(num, 8, "t");
    } else {
	fixeddiscountTable->setText(num, 8, "f");
    }
    
    // вот здесь вот
    //QString tmp = codec_sys->fromUnicode(fdnameLineEdit->text());
    QString qstr_koi8r = codec_db->fromUnicode(fdnameLineEdit->text());
    strcpy( discounts[num].name, qstr_koi8r);
    //tmp = codec_sys->fromUnicode(fdtextLineEdit->text());
    qstr_koi8r = codec_db->fromUnicode(fdtextLineEdit->text());
    strcpy( discounts[num].text,  qstr_koi8r);
    strcpy( discounts[num].type, itoa(fdtypeComboBox->currentItem()));
   // printf("the %d item is select \n", fdtypeComboBox->currentItem());
    //strcpy(discounts[num].type , fdtypeComboBox->currentText() );
    int code = 0;
    code = fdcodeLineEdit->text().toInt();
    strcpy(discounts[num].external_id, QString::number(code).ascii());
    strcpy(discounts[num].value, "");
    //if ( fdtypeComboBox->currentItem() > 1 ) {  puts("надбавочка");  strcat( discounts[num].value, "-" ); }
    strcat( discounts[num].value, fdvalueLineEdit->text() );
    //printf("type  of discount = %s, value = %s \n", discounts[num].type, discounts[num].value);
    //printf("current item = %d \n", fdfixingComboBox->currentItem());
    strcpy( discounts[num].fixing, itoa(fdfixingComboBox->currentItem()) );
    strcpy( discounts[num].barcode, fdbarcodeLineEdit->text() );
    strcpy( discounts[num].cardcode, fdcardcodeLineEdit->text() );
    if (fdshowCheckBox->isChecked()) strcpy(discounts[num].show, "t");  else strcpy(discounts[num].show, "f");
    
}
//================================================================//
void Setup::SaveSample(int num)
{
    //puts("void Setup::SaveSample(int num)\n");
    if (num < 0)  return;
    samplebarcodeTable->setText(num, 0, prefixsamplebarcodeLineEdit->text());
    samplebarcodeTable->setText(num, 1, GetSampleBarcode());
    samplebarcodeTable->setText(num, 2, multipliersamplebarcodeLineEdit->text());
       
    QString qstr_db = prefixsamplebarcodeLineEdit->text();    
    strcpy( sambars[num].prefix, qstr_db );
    qstr_db = codec_db->fromUnicode(GetSampleBarcode());
    strcpy(sambars[num].sample, qstr_db );
    
    qstr_db = multipliersamplebarcodeLineEdit->text();
    strcpy(sambars[num].multiplier, qstr_db);
    
}
//================================================================//
void Setup::newfixeddiscountPushButton_clicked() {    
    /*puts("void Setup::newfixeddiscountPushButton_clicked()");
    printf("count of fixed table = %d \n", fixeddiscountTable->numRows());
    fixeddiscountTable->insertRows(fixeddiscountTable->numRows());
    dis_count ++;
    discounts = (TDiscount*) realloc(discounts, dis_count * sizeof(TDiscount));
    SetDiscountControls(codec_sys->toUnicode("новая скидка"), codec_sys->toUnicode("текст для чека"), 
			QString("- %"), QString("10.00"), codec_sys->toUnicode("на позицию"));
    SaveDiscount(dis_count - 1);
    discounts[dis_count - 1].id = -1;
    fixeddiscountTable->setCurrentCell(fixeddiscountTable->numRows()-1, 0);
    Changed(true);*/
    
    //puts("void Setup::newfixeddiscountPushButton_clicked()");
    //printf("count of fixed table = %d \n", fixeddiscountTable->numRows());
    //puts("----------------222222222-------------");
    //printf("dis_count = %d \n", dis_count);
    //printf("fixeddiscountTable->numRows = %d \n", fixeddiscountTable->numRows());
    //fixeddiscountTable->insertRows(fixeddiscountTable->numRows());
    //fixeddiscountTable->insertRows(0);
    fixeddiscountTable->setNumRows( fixeddiscountTable->numRows()+1 );
    //printf("dis_count = %d \n", dis_count);
    dis_count ++;
    //printf("dis_count = %d \n", dis_count);
    discounts = (TDiscount*) realloc(discounts, dis_count * sizeof(TDiscount));
    //printf("dis_count = %d \n", dis_count);
    SetDiscountControls(QString("0"), codec_sys->toUnicode("новая скидка"), codec_sys->toUnicode("текст для чека"), 
			QString("- %"), QString("10.00"), codec_sys->toUnicode("на позицию"), QString(""), QString(""), true);
    //puts("-----------1111111111------------");
    SaveDiscount(dis_count - 1);
    discounts[dis_count - 1].id = -1;
    strcpy(discounts[dis_count - 1].external_id, "0");
    fixeddiscountTable->setCurrentCell(fixeddiscountTable->numRows()-1, 0);
    Changed(true);
} 
//================================================================//
void Setup::savefixeddiscountPushButton_clicked() {
/*    puts("void Setup::savefixeddiscountPushButton_clicked()");
    if ( fdvalueLineEdit->text() == QString("0.00") ) { 
	errorMessage("Скидка/надбавка не может иметь нулевое значение!!!"); 
	fdvalueLineEdit->setFocus();
	return; }
    SaveDiscount(fixeddiscountTable->currentRow());
    savefixeddiscountPushButton->setEnabled(false);
    Changed(true);*/
        //puts("void Setup::savefixeddiscountPushButton_clicked()");
    /*if ( fdvalueLineEdit->text() == QString("0.00") ) { 
	errorMessage(25); 
	//Error(15);
	fdvalueLineEdit->setFocus();
	return; }*/
    SaveDiscount(fixeddiscountTable->currentRow());
    savefixeddiscountPushButton->setEnabled(false);
    Changed(true);
}
//================================================================//
void Setup::deletefixeddiscountPushButton_clicked() {
/*    puts("void Setup::deletefixeddiscountPushButton_clicked()");
    int num = fixeddiscountTable->currentRow();
    if (num == -1) return;
    fixeddiscountTable->removeRow(num);
    dis_count--;
    
    for (int j = num; j < dis_count; j++) { discounts[ j ] = discounts[j+1];    }    
    
    discounts = (TDiscount*) realloc(discounts, dis_count *sizeof(TDiscount));
    //printf("name of last discount  = %s \n", discounts[dis_count-1].name);
    Changed(true);
    if ( fixeddiscountTable->numRows() ==0 )  fixeddiscountNotAvaiable();
    //printf("discounts[num].id = %d doljno bit -1 \n", discounts[num]. );	*/
    
      // puts("void Setup::deletefixeddiscountPushButton_clicked()");
    int num = fixeddiscountTable->currentRow();
    if (num == -1) return;
    fixeddiscountTable->removeRow(num);
    dis_count--;
    
    for (int j = num; j < dis_count; j++) { discounts[ j ] = discounts[j+1];    }    
    
    discounts = (TDiscount*) realloc(discounts, dis_count *sizeof(TDiscount));
    //printf("name of last discount  = %s \n", discounts[dis_count-1].name);
    Changed(true);
    if ( fixeddiscountTable->numRows() ==0 )  fixeddiscountNotAvaiable();
    //printf("discounts[num].id = %d doljno bit -1 \n", discounts[num]. );
    
}
//================================================================//
void Setup::fixeddiscountNotAvaiable()
{
    //puts("void Setup::fixeddiscountNotAvaiable()");
    fdGroupBox->setEnabled(false);
    SetDiscountControls(QString("0"), QString(""), QString(""), QString("- %"), QString(""), codec_sys->toUnicode("на позицию"), QString(""), QString(""), false);
    savefixeddiscountPushButton->setEnabled(false);
    deletefixeddiscountPushButton->setEnabled(false);
}
//================================================================//
void Setup::fixeddiscountTable_currentChanged( int row, int )
{
    /*puts("void Setup::fixeddiscountTable_currentChanged( int row, int )");
    if (row == -1) return;    
    fdGroupBox->setEnabled(true);
    
    if ( fixeddiscountTable->text(row, 2) == "" ) fixeddiscountTable->setText(row, 2, "- %"); 
    if ( fixeddiscountTable->text(row, 4) == "" ) fixeddiscountTable->setText(row, 4, codec_sys->toUnicode("на позицию")); 
        
    SetDiscountControls(  fixeddiscountTable->text(row, 0), 
		   fixeddiscountTable->text(row, 1), 
		   fixeddiscountTable->text(row, 2),
		   fixeddiscountTable->text(row, 3), 
		   fixeddiscountTable->text(row, 4));
    //QString s = QString::number(fdvalueLineEdit->text().toDouble(), 'f', 2);
    //fdvalueLineEdit->setText(s);
    deletefixeddiscountPushButton->setEnabled(true);
    savefixeddiscountPushButton->setEnabled(false);*/
        //puts("void Setup::fixeddiscountTable_currentChanged( int row, int )");
    if (row == -1) return;    
    fdGroupBox->setEnabled(true);
    
    if ( fixeddiscountTable->text(row, 3) == "" ) fixeddiscountTable->setText(row, 3, "- %"); 
    if ( fixeddiscountTable->text(row, 5) == "" ) fixeddiscountTable->setText(row, 5, codec_sys->toUnicode("на позицию")); 
        
    SetDiscountControls(  fixeddiscountTable->text(row, 0), 
		   fixeddiscountTable->text(row, 1), 
		   fixeddiscountTable->text(row, 2),
		   fixeddiscountTable->text(row, 3), 
		   fixeddiscountTable->text(row, 4),
		   fixeddiscountTable->text(row, 5),
		   fixeddiscountTable->text(row, 6),
		   fixeddiscountTable->text(row, 7),
		   fixeddiscountTable->text(row, 8) == "t");
    
    //QString s = QString::number(fdvalueLineEdit->text().toDouble(), 'f', 2);
    //fdvalueLineEdit->setText(s);
    deletefixeddiscountPushButton->setEnabled(true);
    savefixeddiscountPushButton->setEnabled(false);
}
//================================================================//
void Setup::fdtypeComboBox_activated( const QString & )	{
    testDiscountType();
    QString s = QString::number( fdvalueLineEdit->text().toDouble(), 'f', 2 );
    fdvalueLineEdit->setText(s);
}
//================================================================//
void Setup::fixeddiscountChanged() { 
    //puts("fixeddiscountChanged()"); 
    savefixeddiscountPushButton->setEnabled(true); }
//================================================================//
void Setup::author_toggled( bool b) {    
    
    
    if (this->focusData()->focusWidget() != authorpasswordCheckBox &&
	this->focusData()->focusWidget() != authorcardreaderCheckBox &&
	this->focusData()->focusWidget() != authorbarcodeCheckBox) return; 
    int author_count = 0;
    
    if (authorpasswordCheckBox->isChecked()) author_count++;
    if (authorcardreaderCheckBox->isChecked())author_count++;
    if (authorbarcodeCheckBox->isChecked()) author_count++;
    printf("author_toggled = %d \n", author_count);
    if ( author_count < 1 ) {
	((QCheckBox *)this->focusWidget())->setChecked(true);
	errorMessage(26);
    }
}
//================================================================//
void Setup::fdvalueLineEdit_EditEnd() {
    //printf("edit end \n");
    QString s = QString::number(fdvalueLineEdit->text().toDouble(), 'f', 2);
    fdvalueLineEdit->setText(s);
}
//================================================================//
void Setup::synchroCheckBox_toggled( bool b )	{
    //puts("void Setup::synchroCheckBox_toggled( bool b )	");
    autosynchroCheckBox->setEnabled( ! b );
    notifysynchroneedGroupBox->setEnabled( ! b ); 
    //synchrorangeGroupBox->setEnabled( autosynchroCheckBox->isChecked() && !b );
    synchrorangeGroupBox->setEnabled( !b );
}
//================================================================//
/*void Setup::autosynchroCheckBox_toggled( bool b) {
    //synchrorangeGroupBox->setEnabled( ! synchroCheckBox->isChecked() && b );
}*/
//================================================================//
void Setup::synchrotimechanged() {
    if ( synchrotoSpinBox->value() < synchrofromSpinBox->value() ) {
	synchrotoSpinBox->setValue( synchrofromSpinBox->value() );     }
}
//================================================================//
void Setup::standseparatorCheckBox_toggled( bool b ) {	standseparatortypeComboBox->setEnabled( b );	}
//================================================================//
void Setup::bottomseparatorCheckBox_toggled( bool b ) {	bottomseparatortypeComboBox->setEnabled( b );	}
//================================================================//
void Setup::titleseparatorCheckBox_toggled( bool b )  {	titleseparatortypeComboBox->setEnabled( b ); 	}
//================================================================//
void Setup::nameschemeComboBox_textChanged( const QString & ) { 	
    savePushButton->setEnabled( true );	
}
//================================================================//
void Setup::usesetupPOSCheckBox_toggled( bool b )  { setupchequeGroupBox->setEnabled( !b ); }
//================================================================//
void Setup::Space_Handler() {
    if ( this->focusData()->focusWidget() == basediscountorderTable ) EditDiscountOrder( ! editDiscountOrder);
}
//================================================================//
void Setup::basediscountorderupPushButton_clicked() {
    int num = basediscountorderTable->currentRow();
    if ( num < 1 ) return;
    
    SwapDiscountOrder(num, num - 1);
    EditDiscountOrder(false);
}
//================================================================//
void Setup::basediscountorderdownPushButton_clicked() {
    int num = basediscountorderTable->currentRow();
    if ( num > basediscountorderTable->numRows() - 2 ) return;
    
    SwapDiscountOrder(num, num + 1);
    EditDiscountOrder(false);
}
//================================================================//
void Setup::SwapDiscountOrder(int num_from, int num_to) {
    //printf("Swap from num = %d to num = %d \n ", num_from, num_to);
    
    basediscountorderTable->swapRows(num_from, num_to);
    basediscountorderTable->updateContents();
    basediscountorderTable->clearSelection();
    QTableSelection ts;
    ts.init(num_to, 0);    
    ts.expandTo(num_to, 0);
    basediscountorderTable->addSelection(ts);	
    basediscountorderTable->repaintSelections();
}
//================================================================//
void Setup::EditDiscountOrder(bool b) {
    if (b) ParametersChanged();
    editDiscountOrder = b;  
    QPalette p ( basediscountorderTable->palette() );
    QColor clSel("Grey") ; 
    QColor clMov("Blue");
    QColor cl;
    if (b) cl = clMov; else  cl = clSel;
    p.setColor(QPalette::Active, QColorGroup::Highlight, cl); 
    basediscountorderTable->setPalette(p);
}
//================================================================//
/*void Setup::nameschemeComboBox_textChanged( const QString & str )
{
    QString hhh = codec_sys->fromUnicode(str);
    printf("\nnameschemeComboBox(%s) \n", hhh.ascii());
    for (uint i = 0 ; i < hhh.length(); i++)
    {
	printf("char of str[%d] = %c = %d \n", i, (char) ((QChar) hhh[i]), (char) ((QChar) hhh[i] ) );
    }
    printf("end of text changed\n");
}*/
//================================================================//
void Setup::basediscountorderTable_clicked( int, int, int, const QPoint & ) {   EditDiscountOrder(false);  }
//================================================================//
void Setup::basediscountorderTable_currentChanged( int num, int )	{
    //printf("num = %d \n", num);
    if (editDiscountOrder && old_num != num) SwapDiscountOrder(old_num, num); 
    else basediscountorderTable->clearSelection();
    old_num = num;
    basediscountorderTable->setCurrentCell(num, 0);
}
//================================================================//
void Setup::resultdiscountComboBox_activated( const QString & str) {
    setBaseDiscountOrderEnabled(str == codec_sys->toUnicode("Объединяется"));  }
//================================================================//
void Setup::setBaseDiscountOrderEnabled( bool b) {
    basediscountorderTable->setEnabled(b);
    basediscountorderupPushButton->setEnabled(b);
    basediscountorderdownPushButton->setEnabled(b);
}
//================================================================//
void Setup::learnmodeCheckBox_toggled( bool b )  {	
    //puts("----------000000000------------");
    if ( ! this->isActiveWindow()  ) return;
    if ( b ) {
	//puts("-----------1111111111------------");
	int kkm_ret = hw->ECR_ReadStatus();
	//puts("-----------22222222------------");
	if ( kkm_ret != 0 ) {
	    errorMessage(27);
	    learnmodeCheckBox->setChecked(false);
	    writetransactionCheckBox->setEnabled(false); 	
	    return;
	} else if ( hw->ECR_GetSessionOpened() ) {
	    errorMessage(28);
	    learnmodeCheckBox->setChecked(false);
	    writetransactionCheckBox->setEnabled(false); 	
	    ParametersChanged();
	    return;
	}
	
    } 
    //puts("-------1111111111-----------------");
    writetransactionCheckBox->setEnabled(b); 	
}
//================================================================//
void Setup::limitclientsumCheckBox_toggled(bool b)  
{	
    maxmoneyLineEdit->setEnabled(b);  
    maxmoneyTextLabel->setEnabled(b);    }
//================================================================//
void Setup::BlockCode() 
{
    printf("*****************************fdfixingComboBox->currentItem() = %d \n", fdfixingComboBox->currentItem() );
    if ( fdfixingComboBox->currentItem() == 1 ) {
	//puts("на чек???");
	fdbarcodeLineEdit->setText("");
	fdcardcodeLineEdit->setText("");        }    
    fdbarcodeLineEdit->setEnabled( fdfixingComboBox->currentItem() == 1 );
    fdcardcodeLineEdit->setEnabled( fdfixingComboBox->currentItem() == 1 );
        
}
//================================================================//
void Setup::PrintOneRegChanged(bool b)
{
    wordwrapCheckBox->setEnabled(b);
    printtaxCheckBox->setEnabled(b);
}
//================================================================//
void Setup::documentTable_currentChanged(int, int)
{
    if ( docList == NULL ) return;
    if ( (documentTable->currentRow() < 0) ) {
	editdocumentPushButton->setEnabled(false);
	removedocumentPushButton->setEnabled(false);
	return;
    }
    documentnameLineEdit->setText(docList->GetCurrentDocument()->GetName());
    documentpathLineEdit->setText(docList->GetCurrentDocument()->GetPath());
    editdocumentPushButton->setEnabled(documentTable->currentRow() != -1);
    removedocumentPushButton->setEnabled(documentTable->currentRow() != -1);    
}
//================================================================//
void Setup::AddDocument()
{
    if ( docList == NULL ) return;
    if ( documentnameLineEdit->text().isEmpty() ) return;
    if ( documentpathLineEdit->text().isEmpty() ) return;
    docList->AddDocument(documentnameLineEdit->text(), documentpathLineEdit->text());
    ParametersChanged();
}
//================================================================//
void Setup::EditDocument()
{
    if ( docList == NULL ) return;
    if ( documentnameLineEdit->text().isEmpty() ) return;
    if ( documentpathLineEdit->text().isEmpty() ) return;    
    docList->EditDocument(documentnameLineEdit->text(), documentpathLineEdit->text());   
}
//================================================================//
void Setup::RemoveDocument()
{
    if ( docList == NULL ) return;
    docList->RemoveDocument();
    editdocumentPushButton->setEnabled(documentTable->currentRow() != -1);
    removedocumentPushButton->setEnabled(documentTable->currentRow() != -1);
}
//================================================================//
void Setup::SetDocumentPath()
{
    releaseKeyboard();
    QString filename = QFileDialog::getOpenFileName( documentpathLineEdit->text(), QString::null, this );
    if ( filename.length() < 500 )   documentpathLineEdit->setText(filename);
    grabKeyboard();
}
//================================================================//
void Setup::GetPicturePath()
{
    releaseKeyboard();
    QString filename = QFileDialog::getOpenFileName( picturepathLineEdit->text(), "Images (*.png *.jpg *.gif *.PNG *.JPG *.GIF)", this );
    if ( filename.length() < 500 )   picturepathLineEdit->setText(filename);
    grabKeyboard();
}
//================================================================//
void Setup::documentTable_valueChanged(int row, int col)
{
    editdocumentPushButton->setEnabled(documentTable->currentRow() != -1);
    removedocumentPushButton->setEnabled(documentTable->currentRow() != -1);
}
//================================================================//
void Setup::roundPowerChanged(int v) 
{
    //printf("roundPowerChanged(%d)  roundpowerSlider->value() = %d \n", v, roundpowerSlider->value());
    switch(v) {
    case 0: roundpowerLineEdit->setText("0.05"); break;
    case 1: roundpowerLineEdit->setText("0.1"); break;
    case 2: roundpowerLineEdit->setText("0.5"); break;
    case 3: roundpowerLineEdit->setText("1"); break;
    case 4: roundpowerLineEdit->setText("5"); break;
    case 5: roundpowerLineEdit->setText("10"); break;
    }
}
//================================================================//
void Setup::setRoundPower(const QString& round_power_string) 
{
    if ( round_power_string == QString("0.05") ) {
	roundpowerSlider->setValue(0);
	return;
    }
    if ( round_power_string == QString("0.1") ) {
	roundpowerSlider->setValue(1);
	return;
    }    
    if ( round_power_string == QString("0.5") ) {
	roundpowerSlider->setValue(2);
	return;
    }
    if ( round_power_string == QString("1") ) {
	roundpowerSlider->setValue(3);
	return;
    }
    if ( round_power_string == QString("5") ) {
	roundpowerSlider->setValue(4);
	return;
    }
    if ( round_power_string == QString("10") ) {
	roundpowerSlider->setValue(5);
	return;
    }
    
    roundpowerSlider->setValue(0);
}
//================================================================//
void Setup::RoundPowerChanged() {
    if ( roundpowerLineEdit->text() != "0"  && roundpowerLineEdit->text() != "0.05" && roundpowerLineEdit->text() != "0.1" &&
	 roundpowerLineEdit->text() != "0.5" && roundpowerLineEdit->text() != "1" && roundpowerLineEdit->text() != "5" && 
	 roundpowerLineEdit->text() != "10") {
	roundpowerLineEdit->setText("0.05");
    }
}
//================================================================//
void Setup::ob_closecheck_cb_toggled(bool b) {
    openboxbyrestCheckBox->setEnabled(b);
}

