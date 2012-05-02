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

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "addon_func.h"
#include "copyinfo.h" 

//==================================================================================// 
CopyInfo::CopyInfo()
{
//puts("CopyInfo::CopyInfo()");
  Clear_CopyInfo();
}

//==================================================================================// 
CopyInfo::~CopyInfo()
{
//puts("CopyInfo::~CopyInfo()");

}

//==================================================================================// 
int CopyInfo::GetLengthCheckNum()
{
  return n_check.length();
}

//==================================================================================//
int CopyInfo::GetLengthDocumentNum()
{
  return n_document.length();
}

//==================================================================================// 
int CopyInfo::GetLengthCashNum()
{
  return n_cash.length();
}

//==================================================================================//
int CopyInfo::GetLengthCashierNum()
{
  return n_cashier.length();
}

//==================================================================================//
int CopyInfo::GetLengthDateTime()
{
  return datetime.length();
}

//==================================================================================//
const char * CopyInfo::GetCheckNum()
{
  return n_check.c_str();
}

//==================================================================================//
const char * CopyInfo::GetDocumentNum()
{
  return n_document.c_str();
}

//==================================================================================//
const char * CopyInfo::GetCashNum()
{
  return n_cash.c_str();
}

//==================================================================================//
const char * CopyInfo::GetCashierNum()
{
  return n_cashier.c_str();
}

//==================================================================================//
const char * CopyInfo::GetDateTime()
{
  return datetime.c_str();
}

//==================================================================================// 
int CopyInfo::GetReceiptType()
{
  return type;
}

//==================================================================================// 
double CopyInfo::GetNalSum()
{
  return nalsum;  
}
//==================================================================================// 
double CopyInfo::GetCheckSum()
{
    return checksum;
}
//==================================================================================// 
double CopyInfo::GetCredSum()
{
  return credsum;
}
//==================================================================================// 
double CopyInfo::GetContSum()
{
  return contsum;
}

//==================================================================================// 
double CopyInfo::GetClientSum()
{
  return clientsum;
}

//==================================================================================// 
void CopyInfo::SetNalSum(double s)
{
  nalsum = s;
}
//==================================================================================// 
void CopyInfo::SetCheckSum(double s)
{
    checksum = s;
}
//==================================================================================// 
void CopyInfo::SetCredSum(double s)
{
  credsum = s;
}

//==================================================================================// 
void CopyInfo::SetContSum(double s)
{
  contsum = s;
}

//==================================================================================// 
void CopyInfo::SetClientSum(double s)
{
  clientsum = s;
}

//==================================================================================// 
void CopyInfo::SaveInfo(const char *ncheck, const char *ndoc, const char *ncash, const char *ncashier, const char *dt, int type_id)
{
//puts("CopyInfo::SaveInfo(const char *ncheck, const char *ncash, const char *ncashier, const char *d, const char *t, int type_id)");
  n_check = ncheck;
  n_document = ndoc;
  n_cash = ncash;
  n_cashier = ncashier;
  datetime = dt;
  type = type_id;
}

//==================================================================================// 
void CopyInfo::Clear_CopyInfo()
{
//puts("CopyInfo::Clear_CopyInfo()");
  n_check = "";
  n_document = "";
  n_cash = "";
  n_cashier = "";
  datetime = "";
  
  nalsum = 0;
  credsum = 0;
  contsum = 0;
  clientsum = 0;
  checksum = 0;
}
//==================================================================================// 

