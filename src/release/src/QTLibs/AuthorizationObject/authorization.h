#ifndef AUFORIZATION_CLASS
#define AUFORIZATION_CLASS

#include "accessrights.h"

#include <string>
#include <vector>

using namespace std;

class Authorization
{
  private :
    Authorization();
    
  public :
    static Authorization * GetInstance();

    const char * GetUserName() const;
    const char * GetUserShortcut() const;
    int GetUid() const;
    bool GetRight(int index) const;
    
    bool GetRights(int id);
    
    bool Init(int id);
    int Request(bool block_mode = true);
    
    bool RequestWithAlliedRights(int * index, int num_rights);
    bool RequestWithRight(int index);
    void SwapUser();
  
  public :
    static Authorization instance;
    
  private :
    AccessRights * Acc;
	 
    int cur_uid;
    int afr_uid;

    string cur_username;
    string cur_shortcut;
    string afr_username;
    
    vector<bool> rights;
    
};

#endif
