#include <qobject.h>

#ifndef ADD_WARE_OBJECT
#define ADD_WARE_OBJECT

#include "receipt.h"
#include "Decoder.h"
#include "WareSystem.h"
#include "WareCatalog.h"
#include "Ware.h"

class AddWareController : public QObject 
{
  Q_OBJECT
  private :
    AddWareController(QWidget *parent=0, const char *name=0);
  
  public : //Static Public Member
    static AddWareController instance;

    static AddWareController * GetInstance();
    
  signals :
    void QuantitySet(double q, int parental);
    void WareAdded(int index, bool code_type, bool newsellreceipt);
    void AddWareError(int er);
    void AuthRequest();

  public :
    QString ware_barcode;
    void SetQuantity(double q, int parental);
    //-----(*)-----Zuskin-----13/10/2011-----
    //-----(*)-----Zuskin-----03/08/2011-----
    //int  AddWare(const char * code, bool code_type, int parental, const char* addition_id_from_vs = "");
    //int  AddWare(const char * code, bool code_type, int parental, const char* addition_id_from_vs = "", bool price = false);
    //---------------------------------------
    int  AddWare(const char * code, bool code_type, int parental, const char* addition_id_from_vs = "", bool price = false, bool qttyset = false);
    //---------------------------------------
    void EscapeQuantity();
    
    double GetQuantity();
    
    //void ClearModel();
    int GetResultAddWare();

  private :
    bool AuthorizationRequest(int r);
    bool GetAccess(bool code_type, int parental);
    bool CheckCode(const char * code, bool code_type);
    //-----(*)-----Zuskin-----13/10/2011-----
    //int AddNewPosition(const char * code, bool code_type, int & npos, const QString & addition_id);
    int AddNewPosition(const char * code, bool code_type, int & npos, const QString & addition_id, bool qttyset);
    //---------------------------------------
    void PositionCustomerDisplay(int pos);
    QString RegistrationByAdditions(const char* ware_code, const QString & ware_reg_barcode);

  private :
    double quantity;
    int er;
};


#endif
