#ifndef STATE_STORE_CLASS
#define STATE_STORE_CLASS

enum ReceiptState {FREE, SELL, RETURN, COPY};
// FREE - нет открытого чека
// SELL - открыт чек продажи
// RETURN - открыт чек возврата
// COPY - открыта копия чека

enum SessionState {CLOSED, OPENED, ENDING, BLOCKED};
// CLOSED - кассовая смена закрыта
// OPENED - кассовая смена открыта
// ENDING - кассовая смена открыта, но находится в режиме предупреждения о скором окончании времени смены
// BLOCKED - время кассовой смены истекло

enum ViewState {GENERAL, INS_TEXT, INS, REC, MESS, FILL};
// GENERAL - многофункциональные поля свободны специализированного ввода
// INS_TEXT - активированна функция промитога, сумма чека отображенна в поле воода
// INS - активированна функция промитога, поле ввода очищенно
// REC - начисленна скидка на чек, список доступных операция ограничен 
// MESS - в поле воода отображенно сообщение для пользователя
// FILL - произведена оплата чека, визуализация в режиме отображения информации о закрытом чеке

class StateStore
{
  private :
    StateStore();
    ~StateStore();

  public :
    static StateStore instance;
    static StateStore * GetInstance();
    
    void SetReceiptState(ReceiptState st);
    void SetSessionState(SessionState st);
    void SetViewState(ViewState st);
    
    ReceiptState GetReceiptState();
    SessionState GetSessionState();
    ViewState GetViewState();
    //-----(+)-----Zuskin-----30/09/2011-----
    ViewState GetPrevState();
    //---------------------------------------
  private :
    ReceiptState receipt_state;
    SessionState session_state;
    ViewState view_state;
    //-----(+)-----Zuskin-----30/09/2011-----
    ViewState prev_state;
    //---------------------------------------
};

#endif
