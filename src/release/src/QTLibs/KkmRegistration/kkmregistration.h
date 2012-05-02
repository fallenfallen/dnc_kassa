#ifndef KKM_REGISTRATION_FUNC
#define KKM_REGISTRATION_FUNC

#include <qapplication.h>

#include "hwsrv.h"
#include "debug_level_log.h"
#include "receipt.h"
#include "copyinfo.h"
//-----(+)-----Zuskin-----27/07/2011-----
#include "reg_pgsql.h"
//--------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class KkmRegistrations
{
  private :
    KkmRegistrations();
  public :
    KkmRegistrations(HWSRV * hw_ptr/*, Receipt * rec_ptr*/, DebugLevelLog * log_ptr);
    ~KkmRegistrations();

  private :
    HWSRV * hw;
    SP* sp;
   // Receipt * receipt;

    DebugLevelLog * log;
		
    int stringwide;
		
    bool init_flag;
    bool ctrl_state;
    


  public :
	  
    int PrintCenterString(QString str);
    int PrintBoldCenterString(QString str);
    int CreditStatus;
    
    void SetSP(SP* asp);
    
    void SetCreditStatus(int cs);
    int GetCreditStatus();
    //void setApplicationRef(QApplication * a);
    void KkmSetCtrlState(bool state);
    bool KkmGetCtrlState();
    int KkmSetMode(int m);
    int PrintCreditInfo(bool client_check);
			
    int KkmRegistration(bool test_mode, double s = 0);
    int KkmReturn(bool test_mode, double s = 0);

    int KkmPrintCheckPositions(Receipt * receipt, int section = -1, bool copy_of_return = false);
    int KkmPrintCheckReductions(Receipt * receipt, int section = -1);
    
    int KkmPrintTotalSumBySection(Receipt* receipt, double nalsum, double credsum, double contsum);

 //   int KkmCloseCheck(double nal, double cred, double cont);
    int KkmCloseCheck(double nal, double cred, double cont, Receipt * receipt);  //--irina
    int KkmPrintCopyCheck(CopyInfo * copy, Receipt * receipt, int section_num = -1);
    int KkmPrintString(const char *str, bool print_control = false, bool wrap_flag = false);	
    int KkmPrintWideString(const char *str, bool print_control); 
    
    int KkmPosRegistration(Receipt * receipt);//-------------irina
    int KkmPosReturn(Receipt * receipt);//----------------irina
    int KkmPrintReturnNotNalCheck(CopyInfo * copy, Receipt * receipt, const char* seller);//--irina
    
    
    /*----Added by Shonda-----*/
    int KkmPrintDocumentInfo();	
    int KkmPrintHighString(const char *str, bool print_control);
    int KkmPrintSeparatorString(QString str1, QString str2, QString sep = " ", int type = 0);
    int KkmPrintPositionInfo(const char* ware_shortcut, double quantity, double price, bool wrap_flag = false);
    
    int KkmPartialCutPrintDocHeadLine(int PrintHeadLineAfterDoc = 0); 
    
    int KkmPrintDocHeadLine();
    int KkmPartialCut();
    
    int KkmPrintSeveralStrings(const string& str);
    
    // oper_type = 0 - getting 1 - entering
    int KkmPrintSectionGettingEntering(double sum, int oper_type, const char* seller, int section_num = -1);
    
  private :
    //QApplication * app_ref;
    int KkmSumDiscount(int i, int d, Receipt * receipt);
    int KkmSumCharge(int i, int d, Receipt * receipt);
 
   // int KkmPrintPositions();
    int KkmPositionTaxes(int i, Receipt * receipt);
    
    int KkmPrintCopyCheckInfoByProtocol(CopyInfo * copy, Receipt * receipt, int section_num = -1);
        
};

#endif

