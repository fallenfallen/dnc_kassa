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

#include "hwsighandlerscontroller.h"

#include <signal.h>

#include "blocktimer.h"
#include "setup_param.h"
#include "hwsrv.h"
//#include "addwarecontroller.h"


extern SP sp;
extern BlockTimer *bt;
extern HWSRV *hw;

static void catch_sig_registration(int sig_num);
static void catch_sig_authorization(int sig_num);
static void catch_sig_visualsearch(int sig_num);
static void catch_sig_npaspay(int sig_num);
static void catch_sig_setup(int sig_num);
static void catch_sig_ware(int sig_num);
static void catch_sig_waresearch(int sig_num);
static void catch_sig_dfl(int sig_num);
//-----(+)-----Zuskin-----09/03/2011-----
static void catch_sig_accessrights(int sig_num);
//---------------------------------------
//-----(+)-----Zuskin-----04/08/2011-----
static void catch_sig_edsform(int sig_num);
//---------------------------------------

HwSignalHandlersController HwSignalHandlersController::instance;

//======================================================================//
HwSignalHandlersController::HwSignalHandlersController()
{
//puts("HwSignalHandlersController::HwSignalHandlersController()");
  curobject = NOCONTROLLED;
  sigset(SIGUSR1, catch_sig_dfl);
}

//======================================================================//
HwSignalHandlersController * HwSignalHandlersController::GetInstance()
{
//puts("HwSignalHandlersController * HwSignalHandlersController::GetInstance()");
  return &instance;  
}
//======================================================================//
void HwSignalHandlersController::SetControlledObject(ControlledObject obj)
{
    //printf("----------------------SetControlledObject( obj = %d )----------------------------------\n", obj);
    curobject = obj;  
    
    switch ( curobject )  {
    case AUTHORIZATION :
	if ( hw != NULL )	    sigset(SIGUSR1, catch_sig_authorization);
	else 	    SetControlledObject(NOCONTROLLED);
	break;
    case REGISTRATION :
	if ( hw != NULL )	    sigset(SIGUSR1, catch_sig_registration);
	else         SetControlledObject(NOCONTROLLED);
	break;
    case VISUALSEARCH :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_visualsearch);
	else 	    SetControlledObject(NOCONTROLLED);
	break;
    case NPASPAY :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_npaspay);
	else 	    SetControlledObject(NOCONTROLLED);
	break;	
    case SETUP :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_setup);
	else 	    SetControlledObject(NOCONTROLLED);
	break;		
    case WARE :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_ware);
	else 	    SetControlledObject(NOCONTROLLED);
	break;		
    case WARESEARCH :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_waresearch);
	else 	    SetControlledObject(NOCONTROLLED);
	break;	
//-----(+)-----Zuskin-----09/03/2011-----	
    case ACCESSRIGHTS :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_accessrights);
	else 	    SetControlledObject(NOCONTROLLED);
	break;		
//---------------------------------------
//-----(+)-----Zuskin-----04/08/2011-----	
    case EDSFORM :
	if ( hw != NULL )        sigset(SIGUSR1, catch_sig_edsform);
	else 	    SetControlledObject(NOCONTROLLED);
	break;		
//---------------------------------------
    default :
	    sigset(SIGUSR1, catch_sig_dfl);
    }
}
//======================================================================//
ControlledObject HwSignalHandlersController::GetControlledObject()
{
    return curobject;
}
//======================================================================//
void HwSignalHandlersController::EmitBarCodeSignal(const char * barcode)
{
//printf("void HwSignalHandlersController::EmitBarCodeSignal(barcode = %d)\n", barcode);
    emit BarCode(barcode);
}
//======================================================================//
void HwSignalHandlersController::EmitTrackCodeSignal(const char * trackcode)
{
    emit TrackCode(trackcode);
}
//======================================================================//
void HwSignalHandlersController::EmitPinPadSignal()
{
    emit PinPadSignal();
}
//======================================================================//
void HwSignalHandlersController::EmitHZSignal(const char * code)
{
    //puts("void HwSignalHandlersController::EmitBarCodeSignal(const char * trackcode)");
    emit HZCode(code);
}
//======================================================================//
void catch_sig_dfl(int sig_num)
{
    sigset(sig_num, catch_sig_dfl);
}
//======================================================================//
void catch_sig_registration(int sig_num)
{
    sigset(sig_num, catch_sig_registration);
    
    
    if ( hw->SigFromBCS() == 1 )  {
	if (sp.autoblock_flag) bt->action_time = QTime::currentTime();
	puts("------------------------SCANER SIGNAL-----------------------------------------------");
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());   
    }
    
    
    if ( hw->SigFromEW() == 1 )  {
	puts("---------------------ELECTRONIC WEIGHTER SIGNAL--------------------------------");
	if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
	HwSignalHandlersController::GetInstance()->EmitHZSignal("");
    }

    if ( hw->SigFromMCR() == 1 )  {
	if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
	puts("-------------------------READER SIGNAL---------------------------");
	HwSignalHandlersController::GetInstance()->EmitTrackCodeSignal(hw->MCR_GetSecondTrack());  
    }   

}

//======================================================================//
void catch_sig_authorization(int sig_num)
{
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	if ( sp.barcode_authorization )      HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }
    if ( hw->SigFromMCR() == 1 )   {
	if ( sp.card_authorization )	    HwSignalHandlersController::GetInstance()->EmitTrackCodeSignal(hw->MCR_GetSecondTrack());
    }
    sigset(sig_num, catch_sig_authorization);
}
//======================================================================//
void catch_sig_setup(int sig_num)
{
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }
    if ( hw->SigFromMCR() == 1 )   {
	HwSignalHandlersController::GetInstance()->EmitTrackCodeSignal(hw->MCR_GetSecondTrack());
    }
    sigset(sig_num, catch_sig_setup);
}
//======================================================================//
void catch_sig_ware(int sig_num)
{
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }	
    sigset(sig_num, catch_sig_ware);   
}
//======================================================================//
void catch_sig_waresearch(int sig_num)
{
    //puts("-----------catch_sig_waresearch--------------");
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	//puts("EmitBarCodeSignal");
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }	
    sigset(sig_num, catch_sig_waresearch);   
}
//======================================================================//
void catch_sig_visualsearch(int sig_num)
{
    sigset(sig_num, catch_sig_visualsearch);
    if ( hw->SigFromBCS() == 1 )  {
	if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }
}
//======================================================================//
void catch_sig_npaspay(int sig_num)
{
    if ( hw->SigFromPS() == 1 ) {
	if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
	puts("-------------------------PINPAD SIGNAL---------------------------");
	HwSignalHandlersController::GetInstance()->EmitPinPadSignal();  
    }
}
//======================================================================//
//-----(+)-----Zuskin-----09/03/2011-----
void catch_sig_accessrights(int sig_num)
{
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }
    if ( hw->SigFromMCR() == 1 )   {
	HwSignalHandlersController::GetInstance()->EmitTrackCodeSignal(hw->MCR_GetSecondTrack());
    }
    sigset(sig_num, catch_sig_accessrights);
}
//---------------------------------------
//-----(+)-----Zuskin-----04/08/2011-----
void catch_sig_edsform(int sig_num)
{
    sigset(SIGUSR1, SIG_IGN);
    if (  hw->SigFromBCS() == 1 )  {
	HwSignalHandlersController::GetInstance()->EmitBarCodeSignal(hw->BCS_GetBarCode());
    }
    if ( hw->SigFromMCR() == 1 )   {
	HwSignalHandlersController::GetInstance()->EmitTrackCodeSignal(hw->MCR_GetSecondTrack());
    }
    sigset(sig_num, catch_sig_edsform);
}
//---------------------------------------


//======================================================================//

