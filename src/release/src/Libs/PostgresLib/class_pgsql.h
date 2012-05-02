
#include <libpq-fe.h>

#include "debug_level_log.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL LOGFILE
#endif

#ifndef	 PGSQL_ONSCREAN
#define PGSQL_ONSCREAN NODEBUG
#endif

#ifndef CLASS_PGSQL
#define CLASS_PGSQL

#define DEFAULT_LOG_PATH "/tmp/dancy/postgres_log/pgsql.log"

class PGSQL
{
  public :
    PGSQL();
    //-----(*)-----Zuskin-----20/12/2011-----
    //PGSQL(const char * conf_path);
    PGSQL(const char * conf_path, bool try_home = true);
    //---------------------------------------
    PGSQL(const char *dbname, const char *user, const char *password, const char *host, const char *hostaddr, const char *port, const char* log_path);
    ~PGSQL();

    int Init_Connection();
    void Close_Connection();
 //  int doSQL(PGconn * conn, char * command);
    
    void Set_Log_Path(const char *);
    void Set_Db(const char *);
    void Set_User(const char *);
    void Set_Password(const char *);
    void Set_Host(const char *);
    void Set_Hostaddr(const char *);
    void Set_Port(const char *);
    
        
    char * Get_Db();
    char * Get_User();
    char * Get_Password();
    char * Get_Host();
    char * Get_Hostaddr();
    char * Get_Port();  
    
    int Set_Datestyle();
    int BeginTransaction();
    int RollbackTransaction();
    int CommitTransaction();
    
  protected :
    int doSQL(PGconn * conn, const char * command);
    int ReConnect();
    PGconn * Get_Connection();
    

  private :
    void pause(int Nsec);

  public :
    int er_con;
    static const DEBUG_LEVEL_ENUM log_type = LOGFILE;
    
  protected :
    PGconn * myconnection;
    PGresult * result;
    int state;
    DebugLevelLog * log;
    //DebugLevelLog * log;
    //DebugLevelLog pgsql_onscreanlog;

  private : 
    char * logpath;
    char * db;
    char * us;
    char * passwd;
    char * hostname;
    char * hostaddrname;
    char * portnum;
    
    char * confpath;

//    void PG_Stop();
//    void Write_Log_File(char *str);
//    void Write_Error_Log_File(char *str);
};
#endif


