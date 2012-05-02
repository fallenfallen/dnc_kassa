#include <stdio.h>
#include <sys/types.h>
//#include <usb.h>
using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#ifndef HWSRV_H
#define HWSRV_H

class USBDevice{
public:
   USBDevice(int Prod, int Vend){
     idProduct = Prod;
     idVendor = Vend;
   }
   int idProduct;
   int idVendor;
};

class HWSRV
{
private:
    bool SigBCS;
    bool SigMCR;
    bool SigEW;
    bool SigPS;
    int DSN;
    int MCRType, MCRPref1, MCRPref2, MCRSuf1, MCRSuf2, BCSType,
    CDSPType, EWType, BCSPref1, BCSPref2, BCSSuf1, BCSSuf2, ECRType, PSType, EDSType;

    char *BarCode;
    char *MCRCode1;
    char *MCRCode2;
    char *MCRCode3;

protected:
    int * AccessMode;

public:
    vector <USBDevice> usbDevice;
    vector <USBDevice> usbDeviceSupport;
    int ErrNum;
    double Weight;
    FILE* fp;
    char * way;
    bool startCDSP;
    bool startBCS;
    bool startMCR;
    bool startEW;
    bool startEDS;

private:
    int pre_start_DebugLevel(void);
    int PreStartDSP(void);
    int PreStartBCS(void);
    int PreStartEW(void);
    int PreStartMCR(void);
    int PreStartECR(void);
    int PreStartPS(void);
    int PreStartEDS(void);
    int READDEFAULTSCHEMENUM(void);

public:

    HWSRV();
    HWSRV(int Mode);
    ~HWSRV();

    void MD_Switch(void);
    void SetEducationMode(bool flag);
    const char * Get_ActiveKod(char * str);
    int FileAccess(const char *file_path);
    int GenerateKod();
    int GetAccessMode();

    int SigFromBCS();
    int SigFromMCR();
    int SigFromPS();
    int SigFromEW();

    void SetSigFromBCS();
    void SetSigFromMCR();
    void SetSigFromPS();
    void SetSigFromEW();

    //====================USB================================
    void InsertUsbSupportStruct();
    void InsertUsbStruct();
    string GetProductUsbDevice(int Prod, int Vend);

    //====================FR=================================
    int ECR_ReadConf(void);
    int ECR_SetConnectOptions(int Type, int LogNum, int PortNumber, int BaudRate, int   Protocol, bool UseAccessPassword, const char* AccessPassword, const char* Password, bool CheckPrinter = false);
    void ECR_CloseConnection(void);
    int ECR_ReadStatus(void);
    int ECR_ReadCurrentMode(void);
    int ECR_ReadDeviceMetrics(void);
    int ECR_SetMode(int Mode, const char* InspectorPassword = NULL);
    int ECR_Beep(void);
    int ECR_Sound(int Frequency, int Duration);
    int ECR_OpenDrawer(void);
    int ECR_FullCut(void);
    int ECR_PartialCut(void);
    int ECR_CashIncome(double Summ, bool TestMode = false);
    int ECR_CashOutcome(double Summ, bool TestMode = false);
    int ECR_CancelCheck(void);
    int ECR_Registration(double  Price, double  Quantity, const char * NameWare, int Department = 1, int WareGroup = 1, int NumberCharge = 0, double Percents = 0.00, double Charge = 0.00, bool TestMode = false);
    int ECR_Return(double  Price, double  Quantity, const char * NameWare, int Department = 1, int WareGroup = 1, int NumberCharge = 0, double Percents = 0.00, double Charge = 0.00, bool TestMode = false);
    int ECR_SummDiscount(double  Summ, int Destination = 0, bool TestMode = false);
    int ECR_SummCharge(double   Summ, int Destination = 0, bool TestMode = false);
    int ECR_CloseCheck(double Summ, double Percents = 0, double Summ2 = 0, double Summ3 = 0, double Summ4 = 0, int NumberCharge = 0, double Charge = 0.00, bool TestMode = false);
    int ECR_Report(int ReportType, int Day = 0, int Month = 0, int Year = 0, int EndDay = 0, int EndMonth = 0, int EndYear = 0, int Session = 0, int EndSession = 0);
    int ECR_PrintString(const char * Caption, bool PrintOnControl = true);
    int ECR_PrintWideString(const char * Caption, bool PrintOnControl = true);
    int ECR_PrintHighString(const char * Caption, bool PrintOnControl = true);
    int ECR_SetDate(int Day, int Month, int Year);
    int ECR_SetTime(int Hour, int Minute, int Second);
    int ECR_GetLicense(int License);
    int ECR_ResetSummary();
    int ECR_InitSettings();
    int ECR_ResetSettings();
    int ECR_SetSerialNumber(const char* SerialNumber);
    int ECR_SetLicense(int License, const char* LicensePassword);
    int ECR_ReadRange();
    int ECR_Fiscalization(const char* RegNumber, const char* INN, const char* NewPassword);
    int ECR_OpenSession(void);
    int ECR_OpenCheck(int DocumentType);

    //----- added 31.01.11 by Zuskin -----
    int ECR_EKLZActivization(void);
    int ECR_EKLZCloseArchive(void);
    int ECR_EKLZPrintDocByKPK(const char* KPK);
    int ECR_EKLZInitArchive(void);
    //------------------------------------
    int ECR_ReportEKLZBySessionInTheRangeOfDates(int DayBegin, int MonthBegin, int YearBegin, int DayEnd, int MonthEnd, int YearEnd, int ReportType);
    int ECR_ReportEKLZBySessionInTheRangeOfNumSessions(int SessionBegin, int SessionEnd, int ReportType);
    int ECR_ReportEKLZCheckBeltForSession(int Session);
    int ECR_ReportEKLZOutcomeSessionByNumSession(int Session);
    int ECR_InrettupEKLZReport(void);
    int ECR_ReportEKLZOutcomeActivization(void);
    int ECR_RequestEKLZResultActivation(void);

    int ECR_ReadSettings(int Table, int Row, int Field, int FieldType);
    int ECR_WriteSettings(int Table, int Row, int Field, int FieldType, const char* TableValue);
    int ECR_ReadCashInDrawer();
    int ECR_ReadSessionSumm();
    int ECR_ReadLastSessionSumm();
    int ECR_ReadTimeAndDataSmena();
    int ECR_ReadCheckNumber();
    int ECR_ReadDocumentNumber(int DocType = 3);
    int ECR_GetOperator(void);
    int ECR_GetLogicalNumber(void);
    int ECR_GetYear(void);
    int ECR_GetMonth(void);
    int ECR_GetDay(void);
    int ECR_GetHour(void);
    int ECR_GetMinute(void);
    int ECR_GetSecond(void);
    double ECR_GetSumm(void);
    int ECR_GetCheckNumber(void);
    int ECR_GetEndYear(void);
    int ECR_GetEndMonth(void);
    int ECR_GetEndDay(void);
    bool ECR_GetFiscal(void);
    bool ECR_GetSessionOpened(void);
    bool ECR_GetDrawerOpened(void);
    bool ECR_GetCheckPaperPresent(void);
    bool ECR_GetControlPaperPresent(void);
    bool ECR_GetCoverOpened(void);
    const char* ECR_GetSerialNumber(void);
    //int ECR_GetModel(void);
    const char * ECR_GetROMVersion(void);
    int ECR_GetMode(void);
    int ECR_GetAdvancedMode(void);
    int ECR_GetSession(void);
    int ECR_GetEndSession(void);
    int ECR_GetCheckState(void);
    int ECR_GetPointPosition(void);
    bool ECR_GetOutOfPaper(void);
    bool ECR_GetPrinterConnectionFailed(void);
    int ECR_GetPortNumber(void);
    int ECR_GetECRBuild(void);
    int ECR_GetECRSoftDay(void);
    int ECR_GetECRSoftMonth(void);
    int ECR_GetECRSoftYear(void);
    bool ECR_GetSlipDocumentIsPresent(void);
    bool ECR_GetSlipDocumentIsMoving(void);
    bool ECR_GetEKLZIsPresent(void);
    bool ECR_GetJournalRibbonOpticalSensor(void);
    bool ECR_GetRecieptRibbonOpticalSensor(void);
    bool ECR_GetJournalRibbonLever(void);
    bool ECR_GetRecieptRibbonLever(void);
    bool ECR_GetIsPrinterLeftSensorFailure(void);
    bool ECR_GetIsPrinterRightSensorFailure(void);
    bool ECR_GetIsEKLZOverflow (void);
    bool ECR_GetQuantityPointPosition(void);
    const char* ECR_GetFMSoftVer(void);
    int ECR_GetFMSoftPack(void);
    int ECR_GetFMSoftDay(void);
    int ECR_GetFMSoftMonth(void);
    int ECR_GetFMSoftYear(void);
    const char * ECR_GetAccessPassword(void);
    const char * ECR_GetPassword(void);
    int ECR_GetBaudRate(void);
    int ECR_GetDocumentNumber(void);
    int ECR_ReadGain(void);
    bool ECR_GetFM1IsPresent(void);
    bool ECR_GetFM2IsPresent(void);
    bool ECR_GetLicenseIsPresent(void);
    bool ECR_GetFMOverflow(void);
    bool ECR_GetIsBatteryLow(void);
    bool ECR_GetIsLastRecordCorrupted(void);
    bool ECR_GetIsFMSessionOpen(void);
    bool ECR_GetIsFM24HoursOver(void);
    int ECR_GetFreeRecordInFM(void);
    int ECR_GetRegistrationNumber(void);
    int ECR_GetFreeRegistration(void);
    const char* ECR_GetINN(void);
    int ECR_GetUProtocolVersion(void);
    int ECR_GetUType(void);
    int ECR_GetUModel(void);
    int ECR_GetUMajorVersion(void);
    int ECR_GetUMinorVersion(void);
    int ECR_GetUCodePage(void);
    int ECR_GetUBuild(void);
    const char* ECR_GetUDescription(void);
    int ECR_GetUMinorType(void);
    int ECR_GetDrawerNumber(void);
    int ECR_ContinuePrinting(void);
    int ECR_PrintDocHeadLine(void);
    int ECR_SetExchangeParams(int PortNumber,int BaudRate, int TimeOut);
    void ECR_ReadError(int ErrNum);
    const char * ECR_GetErrNum(void);
    const char * ECR_GetErrDescription(void);
    int ECR_GetType(void);
    int ECR_GetCharLineLength(void);
    const char * ECR_GetTableValue(void);
    int ECR_GetProtocol(void);
    double ReadParam(const char * NameSearchingParam);
    const char * ReadStrParam(const char * NameSearchingParam, int length);
    int InsertAllParam(void);
    int SetParam(const char * name, bool double_flag, double value);
    int SetStrParam(const char * name, const char * value);
    void ECR_CreateFileWorkWithoutFR(void);
    bool ECR_GetEducationMode(void);
    int ECR_ReadTimeAndDataNow();
    void ECR_SetDrawerOptions(int cdrwchannel, int cdrvimp1, int cdrvimp2,int cdrvimpcnt);
    int ECR_PrintServiceDoc(void);
    void ECR_SetUseOneRegistration(bool flag);
    bool ECR_GetUseOneRegistration(void);
    bool ECR_GetTypePrintHeadLine(void);
    int ECR_CloseNotFiscalDoc(void);
    int ECR_GetCheckOpened(void);
    int ECR_AdvancePaper(void);
    int ECR_GetLogNum(void);
    int ECR_ReadTypePrintHeadLine(void);
    //bool ECR_GetIsFRReturnOK(void);
    //-----(+)-----Zuskin-----23/11/2011-----
    bool ECR_IsPortBuzy(void);
    //---------------------------------------
    

    //====================SCANNER=================================
    int BCS_ReadConf(void);
    int BCS_SetConnectOptions(int Type, int LogNum, int PortNumber,  int BaudRate,  int Pref1, int Pref2, int Suf1, int Suf2);
    int BCS_Start(void);
    void BCS_Stop(void);
    int BCS_GetType(void);
    int BCS_GetPref1(void);
    int BCS_GetPref2(void);
    int BCS_GetSuf1(void);
    int BCS_GetSuf2(void);
    const char* BCS_GetBarCode(void);
    pid_t BCS_GetPid(void);
    void setBarcode(const char * str);
    void setDeviceType(int dt);
    int BCS_GetLogNum(void);
    int BCS_GetInterSymbolTimeout(void);
    
    //====================READER=================================
    int MCR_ReadConf(void);
    int MCR_SetConnectOptions( int Type, int LogNum, int PortNumber, int BaudRate, int Pref1, int Pref2, int Suf1, int Suf2);
    int MCR_Start(void);
    void MCR_Stop(void);
    int MCR_GetType(void);
    int MCR_GetPref1(void);
    int MCR_GetPref2(void);
    int MCR_GetSuf1(void);
    int MCR_GetSuf2(void);
    const char* MCR_GetFirstTrack(void);
    const char* MCR_GetSecondTrack(void);
    const char* MCR_GetThirdTrack(void);
    pid_t MCR_GetPid(void);
    int  MCR_GetFIFO1(void);
    int  MCR_GetFIFO2(void);
    int  MCR_GetFIFO3(void);
    void setMCRcode1(const char * str);
    void setMCRcode2(const char * str);
    void setMCRcode3(const char * str);
    int MCR_GetLogNum(void);

    //====================DISPLAY=================================
    int DSP_ReadConf(void);
    int DSP_Start(void);
    int DSP_SetConnectOptions(int Type, int LogNum, int IdProd, int IdVend, int PortNumber, int BaudRate,int NumberPerecod, int Protocol, int CodeTable, int Speed, int LengthStr, int Brightness,int UpAlign, int UpScrollMode, int DownAlign, int DownScrollMode, const char * DemonUpString, const char * DemonDownString);
    const char * DSP_DemonUpString(void);
    const char * DSP_DemonDownString(void);
    void DSP_AdjustBrightness(int Brightness);
    void DSP_ClearLine(int Line);
    void DSP_Up(const char * Caption);
    void DSP_Down(const char * Caption);
    void DSP_Stop(void);
    pid_t DSP_GetPid(void);
    int DSP_GetType(void);
    int DSP_GetLogNum(void);
    void DSP_Up(const char * Caption, bool Run_Mode);
    void DSP_Down(const char * Caption, bool Run_Mode);
    void DSP_SetLengthStr(int LenStr);
    //===================ELECTRONIC WEIGHTER=====================
    int EW_ReadConf(void);
    int EW_Start(void);
    int EW_SetConnectOptions(int Type, int LogNum, int PortNumber, int BaudRate, int Modelint);
    void EW_Stop(void);
    int EW_GetLogNum(void);
    //-----(+)-----Zuskin-----23/03/2011-----
    int EW_GetType(void);
    //---------------------------------------

    //====================PAYMENT SYSTEM==========================
    int PS_ReadConf(void);
    
    int PS_SetIpAddress(const char * IpAddress);
    int PS_SetIpPort(int IpPort);
    int PS_SetIpAddressVer(const char * IpAddressVer);
    int PS_SetIpPortVer(int IpPortVer);
    void PS_SetPrintCancelCheck(bool printCancelCheck);
    int PS_SetCardMask(const char * CardMask);  
    void PS_SetLogNum(int LogNum);
    int PS_GetLogNum(void);
    void PS_SetTypeSystem(int Type);
    void PS_SetTypeScheme(int Scheme);
    int PS_SetCodeOper(int CodeOper);
    int PS_SetSumm(double summ);
    int PS_SetAdditSumm(double summ);
    int PS_SetCodCurrency(int cod);
    int PS_SetModeEnterCard(int mode);
    int PS_SetModeCodingPinBlock(int mode);
    int PS_SetCardNumber(const char * card);
    int PS_SetCardValid(int year, int month);
    int PS_SetTrack2(const char * track2);
    int PS_SetCodAuth(const char * authcod);
    int PS_SetReferNum(const char * refernum);
    int PS_SetPIN(const char * pin);
    int PS_SetUniqNumTrans(int numtrans);
    int PS_SetTerminalId(const char * terminal_id);
    int PS_SetTypeExchange(int type_exchange);
    int PS_SetTimeout1(int timeout1);
    int PS_SetTimeout2(int timeout2);
    int PS_SetTimeout3(int timeout3);
    int PS_SetNameDirExchange(const char * name_dir);
    int PS_SetBank(const char * bank);
    int PS_SetTradeName(const char * trade_name);
    int PS_SetTownTrade(const char * town_trade);
    int PS_SetAddressTrade(const char * address_trade);
    int PS_SetIdAssistant(const char * id_assist);
    int PS_SetMAC(const char * mac);
    int PS_SetModeEnterTrack2(int mode);
    int PS_SetProcessingFlag(int flag);
    int PS_SetUniqTransNumOnHost(string num);
    int PS_SetEMV(const char * emv);
    int PS_SetTypeAccount(const char * type);
    int PS_SetCodPayment(const char * pay_cod);
    int PS_SetDescrPayment(const char * pay_descr);
    int PS_SetOriginalCodOper(int OriginalCodOper);
    int PS_SetAskAssistant(const char * ask);
    void PS_SetStatus(int st);
    void PS_SetAnswerAssistant(const char * str);
    int PS_SetCardAmericanExpress(bool flag);
    void PS_SetPortNumVerifone(int PortNum);
    void PS_SetBaudRateVerifone(int BaudRate);
    void PS_SetType(int Type);
    const char * PS_GetAddDataAnswer(void);
    const char * PS_GetReferNum(void);
    int PS_GetType();
    int PS_GetMode(void);
    const char * PS_GetAskAssistant(void);
    const char * PS_GetAnswerAssistant(void);
    int PS_GetCodeOper(void);
    const char * PS_GetErrorDescr(int n);
    int PS_GetTransStatus(void);
    const char * PS_GetCodAnswer(void);
    const char * PS_GetTerminalId(void);
    const char * PS_GetIDAssistant(void);
    int PS_GetOriginalCodOper(void);
    double PS_GetSumm(void);
    double PS_GetAdditSumm(void);
    int PS_GetModeCodingPinBlock(void);
    const char * PS_GetCardNumber(void);
    int PS_GetCardValidYear(void);
    int PS_GetCardValidMonth(void);
    const char * PS_GetCodAuth(void);
    const char * PS_GetBank(void);
    const char * PS_GetTradeName(void);
    const char * PS_GetTownTrade(void);
    const char * PS_GetAddressTrade(void);
    bool PS_GetPrintCancelCheck(void);
    const char * PS_GetCardMask(void);
    int PS_GetTypeExchange(void);
    int PS_GetTypeScheme(void);
    int PS_GetTypeSystem(void);
    void PS_GenerateFileNames(void);
    int PS_SendRequest(void);
    vector <string> PS_InsertMasForPrinting(void);
    vector <string> PS_InsertMas( const char * str);//возвращает вектор подстрок из строки с разделителями 0x0A
    int PS_WaitForTimeout2AnswerFromKash(void);
    int PS_Start(void);
    void PS_Stop(void);
    int PS_CheckConnect(void);
    void PS_SetEncryptionType(int Type);
    bool PS_GetPacketTypeAnswer(void);
    //-----------------------------------------
    //====================PAYMENT SYSTEM=========================
    
    //-----(+)-----Zuskin-----15/06/2011-----
    //====================EXTERNAL DISCOUNT SYSTEM ==============
    int EDS_Start(void);
    int EDS_ReadConf(void);
    int EDS_Stop(void);
    int EDS_GetType(void);
    int EDS_GetStatus(int rmk);    
    int EDS_GetClientByBarcode(int rmk, const char* barcode);
    
    const char* EDS_GetErrDescription(int err);
    const char* EDS_GetClientName();
    double EDS_GetDiscount();
    const char* EDS_GetDiscountId();
    const char* EDS_GetUserId();
    
    int EDS_GetRequestsList(int rmk);
    int EDS_SendInfoAboutSale(int rmk, const char* dsc_id, const char* barcode, double sum, double dsum);
    int EDS_GetRequest(int idx);    
    
    void EDS_SetType(int type);
    void EDS_SetIpAddr(const char* ipaddr);
    void EDS_SetIpPort(int port);
    //-------------------------EDS-------------------------------
    //----------------------------------------
};


#endif

