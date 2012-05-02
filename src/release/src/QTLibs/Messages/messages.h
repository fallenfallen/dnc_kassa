#ifndef ERR_STRINGS_FROM_AND_KKM
#define ERR_STRINGS_FROM_AND_KKM

#include <qstring.h> 
#include <qwidget.h>


void InterfaceErrorWin(QString er_str, QString er_id);
void InterfaceMessageWin(QString er_str, QString er_id);

void DeviceConfErrorText(int er, QString & er_str, QString & er_id);
void DeviceConfMessageText(int mes, QString & mes_str, QString & mes_id);

void HWSRVErrorText(int er, QString & er_str, QString & er_id);
//void HWSRVMessageText(int mes, QString & mes_str, QString & mes_id);

//void HandleUnloadErrorText(int er, QString & er_str, QString & er_id);
//void HandleUnloadMessageText(int mes, QString & mes_str, QString & mes_id);
	
void WareReferenceErrorText(int er, QString & er_str, QString & er_id);
void WareReferenceMessageText(int mes, QString & mes_str, QString & mes_id);

void VisualsearchErrorText(int er, QString & er_str, QString & er_id);
void RegistrationMessageText(int mes, QString & mes_str, QString & mes_id);
void RegistrationErrorText(int er, QString & er_str, QString & er_id);
void AuthorizationErrorText(int er, QString & er_str, QString & er_id);
void AccessRightsMessageText(int er, QString & er_str, QString & er_id);
void SetupErrorText(int er, QString & er_str, QString & er_id);
void SetupMessageText(int er, QString & er_str, QString & er_id);
void LoadUnloadErrorText(int er, QString & er_str, QString & er_id);
void kkm_Error(QString er_id, QString er_str);
#endif
