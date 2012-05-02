
#ifndef LOAD_FILE_H
#define LOAD_FILE_H


#include <qtextcodec.h>
#include "upload_const.h"
#include "debug_level_log.h"
#include "read_parameters.h"
#include "upload_pgsql.h"
#include "error_strings_log.h"
#include "reader_config.h"



//перевод каретки
#define CR 13
//конец строки
#define LF 10 

#define MAX_STR_LEN 500
#define MAX_PARAM_LEN 2000

#define DEF_COMMAND "$$$REPLACEQUANTITY"

//extern DebugLevelLog pgsql_onscreanlog;
extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;

class LoadFile
{
  public :
    int er_con;
    LoadFile();
    LoadFile(const char *path);
    ~LoadFile();

  int Load();

  private :
    //1 level
     int CheckHeader();
     int ChangeLoadFeature();
	
	//2 level
    int ExecuteCommand();

	//3 level
    int QuantityCommands();
    int AddBarcodes();
    int AddUsers();
    int AddFixReductions();
    int AddAutoReductionSchemes();
    int AddAutoReductions();
    int AddTaxRates();
    int AddTaxGroups();
    int AddTaxRatesGroups();		
    int DeleteCommands();
    int DeleteAllBarcodes();
    int DeleteAllWaresGroups();
    int DeleteWaresGroupsByCode();

	//4 level
    void WareQuantityCommands();
    void GroupQuantityCommands();
    void AddBarcodesCommands();
    int GetBarcodeDataParam();
    int GetWareDataParam();
    int GetGroupDataParam();
    int GetUserDataParam();
    int GetFixReductionDataParam();
    int GetAutoReductionSchemeDataParam();
    int GetAutoReductionDataParam();
    int GetTaxRateDataParam();
    int GetTaxGroupDataParam();
    int GetTaxRateGroupDataParam();

    //4 level
    int OpenFile();
    int GetNextString();
    int GetNextParam(char *head, bool fl);
    int GetGroupFlag(bool *wg_flag);

  private :
    FILE *fd;
    char * file_path;

    char * cur_str;
    char * prev_comm;
    int cur_command;
		
    char * cur_param;
    BarcodeParameters * barcode_params;		
    WareParameters * ware_params;
    GroupParameters * group_params;
    UserParameters * user_params;
    FixReductionParameters * fixreduction_params;
    AutoSchemeParameters * autoscheme_params;
    AutoReductionParameters * autoreduction_params;
    TaxRateParameters * taxrate_params;
    TaxGroupParameters * taxgroup_params;
    TaxRateGroupParameters * taxrategroup_params;

    PgUpload * pg_upload;
		
    ErStrLog * er_log;
		
    int str_number; //отладочная переменная, номер последней прочитанной строки из файла
    int param_number; //отладочная переменная, номер последнего прочитанного параметра в строке
};

#endif
