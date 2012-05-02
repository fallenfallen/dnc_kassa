#ifndef PARENT_CLASS
#define PARENT_CLASS
#include "class_pgsql.h"
#endif


#ifndef CLASS_ACCESS_RIGHTS
#define CLASS_ACCESS_RIGHTS

class AccessRights : public PGSQL
{
  public :
    AccessRights();
    AccessRights(const char * conf_path);
    AccessRights(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path);
    ~AccessRights();
    
    /*Access Rights Authorization Section*/
    static const int count_rg = 30;
    int Get_Users_Info_with_Rights(char **& user, char **& profile, int *&ident, int * right_index , int num_allied_rights);
    int Authorize_by_Password(int uid, const char *pass);
    int Authorize_by_Barcode(const char *barcode, int &uid);
    int Authorize_by_Cardcode(const char *cardcode, int &uid);
    
    /*Access Rights Registration Section*/
    int Get_User_Rights(int uid, char *&user, bool *rights);
    
    /*Access Rights Setup Section*/

    int Get_Profiles_Info(char **& profile, int *&ident);
    int Get_Users_Info(char **& user, char **& profile, int *&ident);
    int Get_Profile(bool *& profile, char*& name, int id);
    int Get_Profile_Id_by_Name(char *prof_name);
    int Get_User_Id_by_Name(char *user_name);

    int Update_Profile_Info(int id, char *prof_name, bool *rights);
    int Update_User_Info(int id, char *user_name, char *shortcut, int profile, const char* bar_code, const char* card_code, const char* upassword);
    int Add_New_Profile(int &id, char *prof_name, bool *rights);
    int Add_New_User(int &user_id, char *user_name, char *shortcut, int profile_id,  char *bar_code,  char *card_code, char *upassword);
    int Delete_Profile(int id);
    int Delete_User(int id);
    //int Get_User(int id, int *profile_id, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass);
    int Get_User(int id, char* &pr_name, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass);

    /*Access Check Section*/ 
    int Check_Barcode(int user_id, char* barcode);
    int Check_Cardcode(int user_id, char* cardcode);	
    
    int GetUsers(int &count, char ** &users_shortcut);
    
    int GetSellerName(const char * seller_num, char* &seller_name);
    int GetUserName(const char * seller_num, char* &seller_name);
    
  private :
    /*Access Rights Authorization Section*/
    const char * Get_Right(int i);
    int Query_User_with_Password(const char * user_id, const char *password);
    int Query_User_by_Barcode(const char *barcode);
    int Query_User_by_Cardcode(const char *cardcode);
    int Query_Users_with_Rights(char **rights, int num_rights);
    
    /*Access Rights Registration Section*/
    int Query_User_Rights(char *user_id);

    /*Access Rights Setup Section*/
    /*int Insert_Admin_Profile();
    int Insert_Admin_User();*/
    int CheckInsertAdminUser();
    int InsertAdminUser();
    int Get_Admin_Profile_Info();
    int Get_Admin_User_Info();

    int Query_Users();
    int Query_Profiles();
    int Query_Profile(char * id);
    int Query_Profile_Id_by_Name(char * name);
    int Query_User_Id_by_Name(char * name);
    int Query_Max_Profile_Id();
    int Query_Max_User_Id();
    int Update_Profile(const char *id, const char *prof_name, bool *rights);
    int Update_User(const char *uid_str, const char *user_name, const char *shortcut, const char *pid_str, const char *bar_code, const char *card_code, const char *upassword);
    int Insert_Profile(const char *id, const char *prof_name, bool *rights);
    int Insert_User(const char *uid, const char *pid, const char *name, const char *shortcut, const char *barcode, const char *cardcode, const char *userpass);
    int Delete_Profile(char * id);
    int Delete_User(char *id);
    int Query_Number_Users_with_Profile(char *pr_id);
    int Query_User(char * id);

    /*Access Check Section*/
    int Query_Check_Barcode(char* barcode); 
    int Query_Check_Ware_Barcode(char* barcode); 
    int Query_Check_Cardcode(char* cardcode); 
};

#endif
