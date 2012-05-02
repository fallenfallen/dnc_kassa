#include <malloc.h>
#include <math.h>
#include <string.h>
#include <qtextcodec.h>
#include "upload_const.h"
#include "debug_level_log.h"
#include "addon_func.h"
#include "encoding.h"
#include "const.h"
#include "error_strings_log.h"

#ifndef READ_PARAMETERS
#define READ_PARAMETERS

extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

//extern DebugLevelLog pgsql_onscreanlog;

#define FLAG_NUM 9

bool IsInteger(char * str);

/*---------------------------------------------------------------------------------------------------------------*/
class BarcodeParameters
{
public :
    BarcodeParameters();
    ~BarcodeParameters();
    
    void AddBarcodeParameters(int param_num, char * param);
    
private : 
    void FreeBarcodeParameters();
public :
   char * barcode; 
   char * internal_id;
   char * coefficient;
   QTextCodec * DbCodec;
   QTextCodec * CP1251Codec;
   bool error_param;
};
/*---------------------------------------------------------------------------------------------------------------*/
class WareParameters
{
  public :
    WareParameters();
   ~WareParameters();

    void AddWareParameter(int param_num, char * param);
	
  private :
    void FreeWareParameters();

    void SaveBarcode(char * param);
    void SaveDoubleValue(int num, char * param);
    void SaveDateTimeValue(char *& datetimevalue, char * param);
    void SaveFlags(char * param);
	
  public :
    char * internal_code;        // 1
    char * bar_code;              // 2
    char * longtitle;                // 3
    char * shortcut;               // 4
    char * price;                    // 5
    char * quantity;               // 6
    char * auto_scheme_id;  // 7
    char * flags;                    // 8
    char * min_price;            // 9
    char * best_before;        // 10
    char * coefficient;          // 14
    char * group_code;        // 16
    char* tax_group_id;       //  23
		
    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class GroupParameters
{
  public :
    GroupParameters();
  ~GroupParameters();

    void AddGroupParameter(int param_num, char * param);

  private :
    void FreeGroupParameters();

  public :
    char * code;                     // 1
    char * longtitle;                // 3
    char * shortcut;               // 4
    char * group_code;        // 16
		
    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class UserParameters
{
  public :
    UserParameters();
   ~UserParameters();

    void AddUserParameter(int param_num, char * param);

  private :
    void FreeUserParameters();

  public :
    char * code;
    char * longtitle;
    char * shortcut;
    char * profile_code;
    char * password;
    char * card_code;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class FixReductionParameters
{
  public :
    FixReductionParameters();
	~FixReductionParameters();

    void AddFixReductionParameter(int param_num, char *param);

  private :
    void FreeFixReductionParameters();

  public :
    char * code;
    char * longtitle;
    char * shortcut;
    char * card_prefix;
    char * type;
    char * value;
    //char * begin_date;
    //char * end_date;
    char * area;
    char* barcode;
    char * show;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class AutoSchemeParameters
{
  public :
    AutoSchemeParameters();
	~AutoSchemeParameters();

    void AddAutoSchemeParameter(int param_num, char * param);

  private :
    void FreeAutoSchemeParameters();

  public :
    char * code;
    char * longtitle;
    char	* shortcut;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class AutoReductionParameters
{
  public :
	  AutoReductionParameters();
	~AutoReductionParameters();

    void AddAutoReductionParameter(int param_num, char * param);

  private :
    void FreeAutoReductionParameters();
    void SaveWeekdays(int param_num, char * param);

  public :
    char * scheme_code;
    char * code;
    char * longtitle;
    char	* shortcut;
    char * type;
    char * value;
    char * begin_date;
    char * end_date;
    char * begin_time;
    char * end_time;
    char * weekdays;
    char * quantity;
    char * position_sum;
    char * check_sum;
    char * card_prefix_begin;
    char * card_prefix_end;

    bool error_param;

  private :
    int day_beg;
    int day_end;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class TaxRateParameters
{
  public :
    TaxRateParameters();
	~TaxRateParameters();

    void AddTaxRateParameter(int param_num, char * param);

  private :
    void FreeTaxRateParameters();

  public :
    char * code;
    char * longtitle;
    char * shortcut;
    char * value;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class TaxGroupParameters
{
  public :
    TaxGroupParameters();
	~TaxGroupParameters();

    void AddTaxGroupParameter(int param_num, char * param);

  private :
    void FreeTaxGroupParameters();

  public :
    char * code;
    char * longtitle;
    char * shortcut;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
class TaxRateGroupParameters
{
  public :
    TaxRateGroupParameters();
	~TaxRateGroupParameters();

    void AddTaxRateGroupParameter(int param_num, char * param);

  private :
    void FreeTaxRateGroupParameters();

  public :
    char * code;
    char * group_code;
    char * rate_code;
    char * base_flag;

    bool error_param;
    QTextCodec * DbCodec;
    QTextCodec * CP1251Codec;
};
/*---------------------------------------------------------------------------------------------------------------*/
#endif
