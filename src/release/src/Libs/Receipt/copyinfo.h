#ifndef COPYINFO
#define COPYINFO

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class CopyInfo
{
  private:
    string n_check;
    string n_document;
    string n_cash;
    string n_cashier;
    string datetime;
    
    double checksum;
    double nalsum;
    double credsum;
    double contsum;
    
    double clientsum;

    int type;

  public:
    CopyInfo();
    ~CopyInfo();

    int GetLengthCheckNum();
    int GetLengthDocumentNum();
    int GetLengthCashNum();
    int GetLengthCashierNum();
    int GetLengthDateTime();

    const char * GetCheckNum();
    const char * GetDocumentNum();
    const char * GetCashNum();
    const char * GetCashierNum();
    const char * GetDateTime();
    
    int GetReceiptType();
    
    double GetNalSum();
    double GetCredSum();
    double GetContSum();
    double GetClientSum();
    double GetCheckSum();
    
    void SetNalSum(double s);
    void SetCredSum(double s);
    void SetContSum(double s);
    void SetClientSum(double s);
    void SetCheckSum(double s);
    
    void SaveInfo(const char *ncheck, const char *ndoc, const char *ncash, const char *ncashier, const char *dt, int type_id);
    void Clear_CopyInfo();
};


#endif
