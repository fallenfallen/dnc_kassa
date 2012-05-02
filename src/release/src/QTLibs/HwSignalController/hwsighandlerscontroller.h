#ifndef HW_SIGHANDLERS_CONTROLLER
#define HW_SIGHANDLERS_CONTROLLER

#include <qobject.h>

//-----------(*)------Zuskin------09.03.2011---------
//enum ControlledObject{NOCONTROLLED, AUTHORIZATION, REGISTRATION, VISUALSEARCH, NPASPAY, SETUP, WARE, WARESEARCH };
//-----------(*)------Zuskin------04.08.2011---------
//enum ControlledObject{NOCONTROLLED, AUTHORIZATION, REGISTRATION, VISUALSEARCH, NPASPAY, SETUP, WARE, WARESEARCH, ACCESSRIGHTS};
enum ControlledObject{NOCONTROLLED, AUTHORIZATION, REGISTRATION, VISUALSEARCH, NPASPAY, SETUP, WARE, WARESEARCH, ACCESSRIGHTS, EDSFORM};
//------------------------------------------------------------

class HwSignalHandlersController : public QObject
{
  Q_OBJECT
  private :
    HwSignalHandlersController();
  
  public :
    static HwSignalHandlersController instance;

    static HwSignalHandlersController * GetInstance();
    
  signals :
    void BarCode(const char * barcode);
    void TrackCode(const char * trackcode);
    void HZCode(const char* code);
    void PinPadSignal();

  public :
    void SetControlledObject(ControlledObject obj);
    void EmitBarCodeSignal(const char * barcode);
    void EmitTrackCodeSignal(const char * trackcode);
    void EmitHZSignal(const char * code);
    void EmitPinPadSignal();
    ControlledObject GetControlledObject();

  private :
    ControlledObject curobject;
};

#endif
