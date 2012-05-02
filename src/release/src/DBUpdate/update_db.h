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

#ifndef MYDB
#define MyDB

#include <string>
#include <iostream>
#include "class_pgsql.h"
#include <libpq-fe.h>
#include "const.h"


using namespace std;

class DB : public PGSQL {
public:
    DB(const char * conf_path);
    DB();
    
    
    int TestTable(string table_name);
    int TestColumn(string column_name, string table_name);
    int TestConstraint(string constraint_name);
    
    void AddDocumentTable();
    
    int TestSetupParam(string param_name, string table_name = "parameters");
    void AddSetupParam(string id, string name, string type, string default_value, string table_name = "parameters");
    
    void AddRight(string right_name);
    
    void AddTransColumn(string column_name, string column_type);
    
    void DropConstraint(string table_name, string constraint_name);
    
    void DropColumn(string table_name, string column_name);
    
    void RenameColumn(string table_name, string column_name, string new_column_name);
    
    void AddColumn(string table_name, string column_name, string column_type);
    
    void IntColumnToVarchar(string table_name, string column_name);
    
    void ChangeOwner(string table_name, string user);
    
    int TestTransactionType(string type_num);
    
    void AddTransactionType(string type_num, string type_name);

    //-----(+)-----Zuskin-----11/07/2011-----
    void CreateTransViews(void);
    void AddFeatureSetsTable(void);
    void AddFeaturesTable(void);
    void AddFeaturesValuesTable(void);
    void AddFeaturesInSetsTable(void); 
    void AddWareConstraint(void);
    void AddWareAdditionsTable(void); 
    void AddWAFVTable(void);
    void AddWareFSConstraint(void);
    void AddVisualSearchViews(void);	
    void AddWareGroupConstraint(void);
    void AddWareGroupIdConstraint(void);
    void AddSectionsTable(void);
    void AddSaleTimeLimitTable(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----20/02/2012-----
    void AddSaleTimeLimitView(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----18/10/2011-----
    void DropTransViews(void);
    void AlterRoundSumColumnType(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----21/10/2011-----
    void AlterUseProxyColumnType(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----27/10/2011-----
    int TestIndex(string index_name);
    void CreateIndex(string index_name, string table_name, string column_name);
    int TestTransViews();
    //---------------------------------------
    //-----(+)-----Zuskin-----16/11/2011-----
    void AddManagersTable(void);    
    //---------------------------------------
};



#endif


