

#include "class_pgsql.h"

#include <qstring.h>

#ifndef MY_CLASS_PGSQL
#define MY_CLASS_PGSQL


class PgRef : public PGSQL 
{  
public :
    ~PgRef();
    PgRef(const char* str);
    
    int orderbysearch;
    QString checkOnApostrof(const char * s);
    //Update
  
    int UpdateTaxGroup(const char * longtitle, const char * shortcut, const char * code);
    int UpdateTaxRate(const char * longtitle,const char * shortcut, const char * value, const char * code);
    int UpdateTaxRateGroup(const char * base_flag, const char * order_id, const char * tax_rate_id, const char * tax_group_id);
//-----(*)-----Zuskin-----08/07/2011-----
//  int UpdateWare(const char* internal_id, const char * new_internal_id, const char * longtitle, const char * shortcut, const char * price, const char * min_price, const char * quantity, const char * sale_quantity, const char * flags, const char * best_before, const char * auto_scheme_id, const char * group_id, const char * tax_group_id);	    
    int UpdateWare(const char* internal_id, const char * new_internal_id, const char * longtitle, const char * shortcut, const char * price, const char * min_price, const char * quantity, const char * sale_quantity, const char * flags, const char * best_before, const char * auto_scheme_id, const char * group_id, const char * tax_group_id, const char * section);	    
//---------------------------------------
    int UpdateAutoScheme(const char * auto_scheme_id, const char * longtitle, const char * shortcut);  
    int UpdateAutoReduction(const char * auto_reduction_id, const char * auto_scheme_id, const char * longtitle, const char * shortcut, const char * value, const char * type, const char * begindate, const char * enddate, const char * begintime, const char * endtime, const char * weekdays, const char * quantity, const char * position_sum, const char * check_sum, const char * card_prefix_begin, const char * card_prefix_end);
    int UpdateWareGroup(const char * longtitle, const char * shortcut, const char * p_id, const char * code);
    int Update_Flags_Value( const char * code);
    
     //check on exists
    int CheckExistWareTable(int * col_ware);
    int CheckExistingWareWithCode(const char * str);
    int CheckExistingWareWithBarCode(const char * str);
    int CheckExistingWareWithName(const char * str);
    int CheckExistingBarCode(const char * str);    
    int GetCountWare(const char* internal_id);

   
    //GetNumFields
    int GetQuantityFieldsWare(int * col_ware);
    int GetQuantityFieldsTaxGroup(void);
    int GetQuantityFieldsTaxRate(void);
    int GetQuantityFieldsTaxRateGroup(void);
    int GetQuantityFieldsWareGroup(void);   
    int GetQuantityFieldsAutoScheme(void);
    int GetQuantityFieldsAutoReduction(void);
    int GetQuantityFieldsWareBarcode(void);  
    
    
    //GetQuantityStr
    int GetQuantityStrInWareGroup(void);
    int GetQuantityStrInWare(int * col_ware);
    int GetQuantityStrInFlags(void);
    int GetQuantityStrInWareWithAutoSchemeId(const char *code);
    int GetQuantityStrInWareWithTaxGroupId(const char *code);
    int GetQuantityStrInNULLWareGroup(void);
    int GetQuantityStrInNotNULLWareGroup(const char * s);
    int GetQuantityWaresInGroup(const char * code);
    int GetQuantityStrInTaxGroup(void);
    int GetQuantityStrInTaxRateByTaxGroupId(const char *code);
    int GetQuantityStrInTaxRate(void);
    int GetQuantityStrInTaxRateGroup(void);
    int GetQuantityStrInAutoSchemeGroup(void);
    int GetQuantityStrInAutoRed( void);
    int GetQuantityStrInAutoReductionByAutoSchemeId(const char *code);
    int GetQuantityStrInWareBarcodeById(const char *code);
    int GetQuantityStrInWareByTaxGroupId(const char *code);
    
    
    //max
    int FindNextOrderIdInTaxRateGroup(void);
    int FindNextIdFromTaxRateGroup(void);
    int FindNextTaxRateIdFromTaxRate(void);
    int FindNextInternalIdFromWare(void);
    int FindNextAutoRedIdFromAutoRed(void);
    int FindNextAutoSchemeIdFromAutoScheme(void);
    int FindNextWareGroupIdFromWare(void);
    int FindNextTaxGroupIdFromTaxGroup(void);
    
    
    //insert    
//-----(*)-----Zuskin-----08/07/2011-----    
//  int AddWareInTable(const char * internal_id, const char * longtitle, const char * shortcut, const char * price,const  char * min_price, const char * quantity, const char * sale_quantity,const char * flags, const char * best_before, const char * auto_scheme_id,const  char * group_id,const char * tax_group_id);
    int AddWareInTable(const char * internal_id, const char * longtitle, const char * shortcut, const char * price,const  char * min_price, const char * quantity, const char * sale_quantity,const char * flags, const char * best_before, const char * auto_scheme_id,const  char * group_id,const char * tax_group_id, const char * section);
//---------------------------------------
    int CreateBarCode(const char * id, const char * code, const char * coefficient, const char * flag);
    int CreateNewTaxGroup(const char * LongtitleStroka,const char * ShortCutStroka);
    int CreateNewGroupOfWare(const char * LongtitleStroka,const char * ShortCutStroka,const char * p_id);
    int Create_Auto_Red_Group_With_SchemeId(const char * auto_scheme_id, const char * name,const char * cutname);
    int Create_Auto_Reduction_Group( const char * name,const char * cutname);
    int Create_Tax_Rate( const char * code);
    int Create_Tax_Rate_Group( const char * code);
    int Create_Auto_Reduction( const char * code);
    int Create_Flags_Value( const char * code);
    int Create_Flags_Table(void);
    
    //delete
    int DeleteWareGroup(const char * str);
    int DeleteAutoRedWithAutoRedId(const char * str);
    int DeleteAutoGroupWithAutoSchemeId(const char * str);
    int DeleteWare(const char * str);
    int DeleteFromTaxRateGroupById(const char * str);
    int DeleteTaxGroupWhereTaxGroupId(const char * str);
    int DeleteTaxRate(const char * str);
    int DeleteTaxRateGroupByTaxGroupId(const char * str);
    int DeleteTaxRateGroupByTaxRateId(const char * str);
    int DeleteTaxRateByLongtitle(const char * str);
    int DeleteBarcodeOfWare(const char * str);
    int DeleteAllWare(void);
    int DeleteAllWareGroup(void);
    int DeleteAllWareBarcode(void);
    
    
    int Get_Flags_Info(char * values); 
    
    //FUNC WITH SEARCH 
    
    //поисковые функции
    int Query_Cnt_by_IntCode(const char *pattern);
    int Query_Cnt_by_BarCode(const char *pattern);
    int Query_Cnt_by_ShCut(const char *pattern);
    
    int Get_Num_Search_Info(const char *pattern, int pattern_type);
    int Get_List_By_Pattern(const char *pattern, int pattern_type);
    //--------------------------------
    
    int LoadInfo(const char *str, char **mas);
    void Get_Group_Info(char *name,  char *group_id, int i, int n);
    void Get_Prod_Info(char *longtitle,  char *shortcut, char *price,  char *quantity,  char *bar_code,  char *internal_code, int i, int n);
    int Get_Group_Info(const char *group_id);
    int Get_Group_Info(const char *group_id, const char * name_group, int limit);
	 
    int Get_Goods_Info(const char *group_id);
    int Get_Goods_Info(const char *group_id, const char * id_ware,int limit);
	 
	 
    int Get_Parent_Info(char *group_id,  char *name,  char *pgroup);
    int Query_Group(const char *gr);
    int Query_Group(const char *gr, const char * name_begin_gr, int limit);
	 
    int QuantityOfGroup(const char * str,char ** mas,char ** mas1);
    int Get_Tax_Rate_Info( char *name, char *shortname,char * value, const char *group_id);
    int Get_Base_Flag_Info(char * base_flag, char * order_id, const char *group_id);
    int Get_Tax_Rate_Group_Info(char * id,char * base_flag,  char * tax_group_id, const char * tax_rate_id, const char * order_id);
    int Get_All_Tax_Rate_Group_Info( char * id, char * base_flag,  char * tax_group_id, const char * tax_rate_id, char * order_id);
    int Get_All_Auto_Red_Info( char * auto_reduction_id, char * auto_scheme_id, char * longtitle, char * shortcut, char * value, char * type, char * begindate, char * enddate, char * begintime, char * endtime, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end); 
    int LoadAutoRedInfoWithAutoSchemeId(const char *str, char **mas);
    int Get_Id_By_Group_Longtitle(const char * code, char ** mas);
    
    //FOR INSERT GROUP LIST
    int GetNameWareGroupByParentId(const char * str, char ** mas);
    int GetNameWareGroupByParentId(const char * str, char ** mas, int i, int j);
    
     //===========GET   SMTH   BY LONGTITLE =====================//
     int GetAutoValueByName(const char * code, char * value);
     int GetTaxRateIdByName(const char * code, char * value);
     int  GetAutoRedIdByName(const char * code, char * value);
    
     //GET ID BY LONGTITLE
     int GetTaxRateId(const char * longtitle, char * value);
     int GetGroupId(const char * longtitle, char * value);
     int  GetGroupId(const char * longtitle, char * value, const char * parentgroupid);
     int GetAutoSchemeId(const char * longtitle, char * value);
     int GetGroupIdInWares(const char * longtitle, char * value);
     int GetGroupIdById(const char * internal_id, char * value);
     int GetTaxGroupId(const char * str, char * value);
    
     //GET SHORTCUT BY LONGTITLE
     int GetAutoSchemeShortCutByLongtitle(const char * str, char * value);
     int GetTaxGroupShortCutByLongtitle(const char * str, char * value);
     int GetWareGroupShortCutByLongtitle(const char * str, char * value);
     
    //===================================================//
    //GET NAMES TO INSERT INTO LISTS
    int GetNameAutoScheme(char **mas);
    int GetNameGroup(char **mas);
    int GetNameTaxGroup(char **mas);
    int GetAutoRedScheme(char **mas);
    int GetFixRedScheme(char **mas);
   
    //GET WARES INTERNAL_ID 
    int LoadWaresByTaxGroupId(const char * code, char **mas);
    int LoadBarcodeById(const char * code, char **mas, char ** mas_coeff);
    int LoadWaresByAutoSchemeId(const char * code, char **mas);
    int LoadWaresByTaxRateId(const char * code, char **mas);
    
    //GET ORDER  TAX RATE 
    int  GetOrderIdByTaxRateIdAndTaxGroupId(const char *  tax_rate_id,const char * tax_group_id, char * value);
    
    //GET  AUTO RED ID BY AUTO SCHEME ID
    int GetAutoRedSchemeWithAutoId(const char * str, char **mas);
    
    //GET LONGTITLE BY ID
   int  GetLongtitleByGroupId(const char * group_id, char * value);
  
  
   int  GetParentIdByGroupId(const char * group_id, char * value);
   int  GetNameWareGroupById(const char * str, char * value);
   int  GetNameAutoSchemeById(const char * str, char * value);
   int  GetNameTaxGroupById(const char * str, char * value);
    
/*   //WHERE CREATE GROUP IF IN BASE THE SAME LONGTITLE
    int  CompareLongtitleWithText(const char * str);
    int  CompareTaxLongtitleWithText(const char * str);
    */
    

    int Get_TaxRateByGroup(char ** massiv, const char * code);
    int Get_TaxRateIdByGroup(char ** massiv, const char * code);
    int Get_AutoReductionByGroup(char **mas, const char * code);
    int Get_Tax_Rate_InfoById(char ** mas,const char * code);
    int Get_IdFromTaxRateGroupByTaxGroupId(char ** mas,const char * code);
    int GetInternalIdWithBarCode(const char * str, char *id);
    
 private :	
  int Get_WareGroup_By_ParentId(const char * code);	
  int Query_by_Bar_Code_Pattern(const char *pattern);
  int Query_by_Internal_Code_Pattern(const char *pattern);
  int Query_by_Shortcut_Pattern(const char *pattern);
  int Get_WareGroup(const char * code);
  int Add_Ware(const char * code);
  int Get_TaxGroup(const char * code);
  int Get_AutoRed(const char * code);
  int Get_Ware_Info(const char * code);
  int Get_Longtitle_From_Ware_Group(void);
  int  Query_Goods(const char *gr);
  int  Query_Goods(const char *gr, const char * id_ware, int limit);
  
  int Query_Parent_Group(const char *gr); 
  int GetExistAutoSchemeId(char ** massiv); 
  int GetExistGroupId(char ** massiv);
  int GetExistTaxGroupId(char ** massiv);
  int Get_AutoRedWithAutoId(const char * code);
  int Get_Auto_Reduction_Info(const char * code);
  int Get_Tax_Rate_Info(const char * code);
  int FindAutoSchemeShortCutByLongtitle (const char * code); 
  int Get_AutoScheme(const char * code); 
  const char * GetTaxLongtitleById(const char * code);
  int Get_Longtitle_From_Tax_Group(void);
};

#endif
