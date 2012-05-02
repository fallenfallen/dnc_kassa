/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "statestore.h"

#include "stdio.h"

StateStore StateStore::instance;

//======================================================================//
StateStore::StateStore()
{
//puts("StateStore::StateStore()");
  receipt_state = FREE;
  session_state = CLOSED;
  view_state = GENERAL;
  prev_state = GENERAL;
}

//======================================================================//
StateStore::~StateStore()
{
//puts("StateStore::~StateStore()");

}

//======================================================================//
StateStore * StateStore::GetInstance()
{
  return &instance;
}

//======================================================================//
void StateStore::SetReceiptState(ReceiptState st)
{
//puts("void StateStore::SetReceiptState(ReceiptState st)");
  receipt_state = st;
  
  switch(st)
  {
    case FREE:
      //puts("FREE");
      break;
    case SELL:
      //puts("SELL");
      break;
    case RETURN:
      //puts("RETURN");
      break;
    case COPY:
      //puts("COPY");
      break;
  }
}

//======================================================================//
void StateStore::SetSessionState(SessionState st)
{
//puts("void StateStore::SetSessionState(SessionState st)");
  session_state = st;
  switch(st)
  {
    case CLOSED :
      //puts("CLOSED");
      break;
    case OPENED:
      //puts("OPENED");
      break;
    case ENDING:
      //puts("ENDING");
      break;
   case BLOCKED:
      //puts("BLOCKED");
      break;
  }
}

//======================================================================//
void StateStore::SetViewState(ViewState st)
{
//puts("void StateStore::SetViewState(ViewState st)");
  
  //-----(+)-----Zuskin-----16/12/2011-----
  //prev_state = st; 
  prev_state = view_state;
  //---------------------------------------
  
  switch(st)
  {
    case GENERAL:
      //-----(+)-----Zuskin-----16/12/2011-----
      prev_state = st;
      //---------------------------------------
      //puts("GENERAL");
      break;
    case INS_TEXT:
      //puts("INS_TEXT");
      break;
    case INS:
      //puts("INS");
      break;
    case REC:
      //puts("REC");
      break;
    case MESS: {
      //puts("MESS");
      //-----(*)-----Zuskin-----16/12/2011-----
      //-----(+)-----Zuskin-----30/09/2011-----
      //prev_state = view_state;
      //---------------------------------------
      //---------------------------------------
      break;
    }
    case FILL:
      //puts("FILL");
      break;
  }
  view_state = st;
}

//======================================================================//
ReceiptState StateStore::GetReceiptState()
{
//puts("ReceiptState StateStore::GetReceiptState()");
  /*switch(receipt_state)
  {
    case FREE:
      puts("FREE");
      break;
    case SELL:
      puts("SELL");
      break;
    case RETURN:
      puts("RETURN");
      break;
    case COPY:
      puts("COPY");
      break;
  }*/
  return receipt_state;
}

//======================================================================//
SessionState StateStore::GetSessionState()
{
/*puts("SessionState StateStore::GetSessionState()");
  switch(session_state)
  {
    case CLOSED :
      puts("CLOSED");
      break;
    case OPENED:
      puts("OPENED");
      break;
    case ENDING:
      puts("ENDING");
      break;
   case BLOCKED:
      puts("BLOCKED");
      break;
  }*/
  return session_state;
}

//======================================================================//
ViewState StateStore::GetViewState()
{
/*puts("ViewState StateStore::GetViewState()");
  switch(view_state)
  {
    case GENERAL:
      puts("GENERAL");
      break;
    case INS_TEXT:
      puts("INS_TEXT");
      break;
    case INS:
      puts("INS");
      break;
    case REC:
      puts("REC");
      break;
    case MESS:
      puts("MESS");
      break;
    case FILL:
      puts("FILL");
      break;
  }*/
  return view_state;
}
//-----(+)-----Zuskin-----30/09/2011-----
ViewState StateStore::GetPrevState()
{
  return prev_state;
}
//---------------------------------------
