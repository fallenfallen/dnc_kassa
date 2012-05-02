/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/


#include "update_db.h"
#include <qtextcodec.h>

QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
QTextCodec * DbCodec = QTextCodec::codecForName(DBCODEC);


DB::DB(const char * conf_path) : PGSQL(conf_path)  
{ 
    if (er_con != 0) throw("Error connect to Db. Check configure file");
}

DB::DB() : PGSQL() 
{
    if (er_con != 0) throw("Error connect to Db. Check configure file");
}

int DB::TestTable(string table_name)
{
    string str;
    str += "SELECT relid FROM pg_statio_user_tables WHERE relname = '";
    str += table_name;
    str += "'";
    
    int state = doSQL(myconnection, str.c_str());   
    
    if ( state != 2 ) throw("table test ---------  sql request error");
    
    int n = PQntuples(result);
    
    PQclear(result);
    
    if ( n == 0 ) {
	return 1;
    }
    
    return 0;
}

void DB::AddDocumentTable()
{
    string str;
    str += "CREATE TABLE Documents( document_id serial, scheme_id  integer, document_name varchar(100),   document_path varchar(500),   CONSTRAINT	DocumentId PRIMARY KEY(document_id), CONSTRAINT SchemeId FOREIGN KEY(scheme_id) REFERENCES schemes(scheme_id)  );";
    str += "INSERT INTO Documents(scheme_id, document_name, document_path) SELECT actsh.activescheme_id, ";
    str += DbCodec->fromUnicode(DefCodec->toUnicode("'Товарный чек', '/usr/share/dnc/print_doc/товарный_чек.html'"));
    str += " FROM activescheme actsh ";
    
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddDocumentTable ------------ sql request error");
    
}

int DB::TestSetupParam(string param_name, string table_name)
{
    string str;
    if (table_name == "parameters") str += "SELECT * FROM " + table_name + " WHERE param_name = '" + param_name + "'";
    else str += "SELECT * FROM " + table_name + " WHERE stringparam_name = '" + param_name + "'";
    
    int state = doSQL(myconnection, str.c_str());
    
    if ( state != 2 ) throw("TestSetupParam  ---------  sql request error");  
    
    int n =  PQntuples(result);
    
    PQclear(result);
    
    if ( n ==  0 ) {
	return 1;
    }
    return 0;
}

void DB::AddSetupParam(string id, string name, string type, string default_value, string table_name)
{
    string str;
    if (table_name == "parameters")    str += "SELECT * FROM " + table_name + " WHERE param_id = " + id;
    else  str += "SELECT * FROM " + table_name + " WHERE stringparam_id = " + id;
    
    int state  = doSQL(myconnection, str.c_str());
    if ( state != 2 ) throw("AddSetupParam  ---------  sql request error"); 
    
    PQclear(result);
    
    if ( PQntuples(result) > 0  ) {	
	// удалить его 
	if (table_name == "parameters" ) {
	    str = "DELETE FROM " + table_name + "values WHERE param_id = " + id + ";";
	    str += "DELETE FROM " + table_name + " WHERE param_id = " + id;
	    
	} else {
	    str = "DELETE FROM " + table_name + "values WHERE stringparam_id = " + id + ";";
	    str += "DELETE FROM " + table_name + " WHERE stringparam_id = " + id;
	}
	if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSetupParam  ---------  sql request error");
    }
    
    if (table_name == "parameters") {
	
	str = "INSERT INTO parameters (param_id, param_name, param_type, default_value) VALUES (" + id + ",'" + name + "','" + type + "', '" + default_value + "');";
	str += "INSERT INTO parametersValues (param_id, scheme_id, value) SELECT p.param_id, s.scheme_id, p.default_value FROM parameters p, schemes s WHERE param_id = " + id;
	
    }   else {
	str = "INSERT INTO stringparameters (stringparam_id, stringparam_name, default_value) VALUES (" + id + ",'" + name + "', '" + default_value + "');";
	str += "INSERT INTO stringparametersValues (stringparam_id, scheme_id, value) SELECT p.stringparam_id, s.scheme_id, p.default_value FROM stringparameters p, schemes s WHERE stringparam_id = " + id;
    }
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSetupParam  ---------  sql request error");
    
    printf("%s parameter was added \n", name.c_str());
}


int DB::TestColumn(string column_name, string table_name)
{
    string str;
    str += " SELECT a.attname , t.relname FROM pg_attribute a, pg_statio_user_tables t WHERE a.attrelid = t.relid AND a.attname = '" + column_name + "' AND t.relname = '" + table_name + "'";
    int state = doSQL(myconnection, str.c_str());
    
    if (state != 2) throw("TestRightParam  ---------  sql request error");
    
    int n = PQntuples(result);
    
    PQclear(result);
    
    if ( n == 0 ) {
	return 1;
    }
    return 0; 
}

void DB::AddRight(string right_name)
{
    string str;
    str += "ALTER TABLE profiles  ADD COLUMN "+ right_name + " boolean ;";
    str += " UPDATE profiles SET " + right_name + " = 't';";
    str += "ALTER TABLE profiles ALTER COLUMN " + right_name + " SET NOT NULL";
    
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddRight  ---------  sql request error");
    printf("right %s -  was added \n", right_name.c_str());
}


void DB::AddTransColumn(string column_name, string column_type)
{
    string str;
    str += "ALTER TABLE transaction ADD COLUMN " + column_name + " " + column_type;
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddTransactionColumn  ---------  sql request error");
    
    printf("Column %s was added into transaction\n", column_name.c_str());
}

int DB::TestConstraint(string constraint_name)
{
    string str;
    str += "SELECT conname FROM pg_constraint WHERE conname = '" + constraint_name + "'";
    
    int state = doSQL(myconnection, str.c_str());
    
    if (state < 0) throw("TestConstraint  ---------  sql request error");
    if ( PQntuples(result) == 0 ) {
	PQclear(result);
	return 1;
    }
    PQclear(result);
    return 0; 
}	

void DB::DropConstraint(string table_name, string constraint_name)
{
    string str;
    str += "ALTER TABLE " + table_name + " DROP CONSTRAINT " + constraint_name;
    
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("DropConstraint  ---------  sql request error");
    
    printf("Constraint %s was dropped\n", constraint_name.c_str());
}



void DB::RenameColumn(string table_name, string column_name, string new_column_name) {
    string str;
    str += "ALTER TABLE " + table_name + " RENAME COLUMN " + column_name + " TO " + new_column_name;
    
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("RenameColumn  ---------  sql request error");
    
    printf("Column %s was renamed to %s \n", column_name.c_str(), new_column_name.c_str());
}
    
void DB::AddColumn(string table_name, string column_name, string column_type) {
    string str;
    str += "ALTER TABLE " + table_name + " ADD COLUMN " + column_name + " " + column_type;
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddColumn  ---------  sql request error");   
    printf("Column %s was added\n", column_name.c_str());
}
    
void DB::DropColumn(string table_name, string column_name) {
    string str;
    str += "ALTER TABLE " + table_name + " DROP COLUMN " + column_name;
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("DropColumn  ---------  sql request error");   
    printf("Column %s was dropped\n", column_name.c_str());
}

void DB::IntColumnToVarchar(string table_name, string column_name) {
    string str;
    try {
	AddColumn(table_name, "test_column", "varchar(20)");
	str += "UPDATE " + table_name + " SET test_column = CAST(" + column_name + " AS VARCHAR)" ;
	if ( doSQL(myconnection, str.c_str()) < 0 ) throw("ConvertColumn  ---------  sql operation error");
	printf("TestColumn was updated\n");
	DropColumn(table_name, column_name);
	RenameColumn(table_name, "test_column", column_name);
	printf("Column %s was converted to varchar \n", column_name.c_str());
    } catch(...) {
	throw("ConvertColumn  ---------  sql operation error");
    }   
}

void DB::ChangeOwner(string table_name, string user) {
    string str;
    str += "ALTER TABLE " + table_name + " OWNER TO " + user;
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" Change Owner ---------  sql request error");   
    printf("Table %s change owner %s was changed\n", table_name.c_str(), user.c_str());
}

int DB::TestTransactionType(string type_num)
{
    string str;
    str += "SELECT * FROM transaction_type WHERE tr_type = ";
    str += type_num;
    
    int state = doSQL(myconnection, str.c_str());   
    
    if ( state != 2 ) throw("Transaction type test ---------  sql request error");
    
    int n = PQntuples(result);
    
    PQclear(result);	    
    
    if ( n == 0 ) {
	return 1;
    }
    return 0;
}

void DB::AddTransactionType(string type_num, string type_name) {    
    string str;
    str += "INSERT INTO transaction_type (tr_type, name) VALUES (";
    str += type_num;
    str += ", '";
    str += DbCodec->fromUnicode(DefCodec->toUnicode(type_name.c_str()));
    str += "')";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" AddTypeTransaction ---------  sql request error");   
    printf("Transaction type - %s %s - was added\n", type_num.c_str(), type_name.c_str());   
}

//-----(+)-----Zuskin-----11/07/2011-----
void DB::CreateTransViews() {    
    string str;
    str = "CREATE OR REPLACE VIEW transaction_by_last_session AS SELECT * FROM transaction tr, (SELECT CASE WHEN MAX(n_transaction) IS NULL THEN 0 ELSE MAX(n_transaction) END AS max FROM transaction WHERE tr_type = 61) s WHERE tr.n_transaction > s.max";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" CreateTransView ---------  sql request error");   
    str = "CREATE OR REPLACE VIEW transaction_with_closed_check_by_last_session AS SELECT * FROM transaction_by_last_session tr WHERE tr.n_check IN (SELECT trcc.n_check FROM transaction_by_last_session trcc WHERE trcc.tr_type = 55 AND trcc.tr_date_time = tr.tr_date_time);"; 
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" CreateTransView ---------  sql request error");   
    printf("Transaction views was added\n"); 
}

//-----(+)-----Zuskin-----18/10/2011-----
void DB::DropTransViews() {    
    string str;
    str = "DROP VIEW transaction_with_closed_check_by_last_session"; 
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" DropTransView ---------  sql request error");   
    str = "DROP VIEW transaction_by_last_session";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" DropTransView ---------  sql request error");   
    printf("Transaction views was dropped...\n"); 
}

void DB::AlterRoundSumColumnType() {    
    string str;
    str = "ALTER TABLE transaction ALTER COLUMN round_sum TYPE NUMERIC(15,3)";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" AlterRoundSumColumnType ---------  sql request error");   
}


//---------------------------------------

void DB::AddFeatureSetsTable()
{
    string str;
    str += "CREATE TABLE features_sets ( set_id serial , set_name varchar(100) , code varchar(20), CONSTRAINT set_id_pk PRIMARY KEY (set_id));";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddFeatureSetsTable ------------ sql request error");
}

void DB::AddFeaturesTable()
{
    string str;
    str += "CREATE TABLE features ( feature_id serial , feature_name varchar(100) , code varchar(20), CONSTRAINT feature_id_pk PRIMARY KEY (feature_id));";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddFeaturesTable ------------ sql request error");
}

void DB::AddFeaturesValuesTable()
{
    string str;
    str += "CREATE TABLE features_values ( value_id serial , value_value varchar(100) , code varchar(20), feature_id integer, CONSTRAINT value_id_pk PRIMARY KEY (value_id), CONSTRAINT feature_id_fk FOREIGN KEY (feature_id) REFERENCES features (feature_id) ON DELETE CASCADE);";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddFeaturesValuesTable ------------ sql request error");
}

void DB::AddFeaturesInSetsTable()
{
    string str;
    str += "CREATE TABLE features_in_sets (fis_id serial, set_id integer, feature_id integer, CONSTRAINT fis_id_pk PRIMARY KEY (fis_id), CONSTRAINT fis_f_id_fk FOREIGN KEY (feature_id) REFERENCES features (feature_id) ON DELETE CASCADE, CONSTRAINT fis_s_id_fk FOREIGN KEY (set_id) REFERENCES features_sets (set_id) ON DELETE CASCADE);";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddFeaturesInSetsTable (create table)------------ sql request error");
    str = "CREATE VIEW features_in_set_view AS SELECT fis.fis_id, f.feature_id, f.feature_name, f.code, fis.set_id FROM features f LEFT JOIN features_in_sets fis ON fis.feature_id = f.feature_id;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddFeaturesInSetsTable (create view)------------ sql request error");
}

void DB::AddWareConstraint()
{
    string str;
    str += "ALTER TABLE ware ADD CONSTRAINT ware_id_pk PRIMARY KEY (ware_id);";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWareConstraint ------------ sql request error");
}

void DB::AddWareAdditionsTable()
{
    string str;
    str += "CREATE TABLE ware_additions ( addition_id serial, addition_article varchar(100), addition_price numeric(8,2), addition_quantity numeric(8,3), addition_code varchar(20), ware_id integer, CONSTRAINT addition_id_pk PRIMARY KEY (addition_id), CONSTRAINT additions_w_id_fk FOREIGN KEY (ware_id) REFERENCES ware (ware_id) ON DELETE CASCADE);";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWareAdditionsTable (create table)------------ sql request error");
}

void DB::AddWAFVTable()
{
    string str;
    str += "CREATE TABLE ware_additions_features_values ( features_value_id serial, addition_id integer, value_id integer, CONSTRAINT wafv_features_value_id_pk PRIMARY KEY (features_value_id), CONSTRAINT wafv_addition_id_fk FOREIGN KEY (addition_id) REFERENCES ware_additions (addition_id) ON DELETE CASCADE, CONSTRAINT wafv_value_id_fk FOREIGN KEY (value_id) REFERENCES features_values(value_id) ON DELETE CASCADE);";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWAFVTable (create table)------------ sql request error");
    str = "CREATE OR REPLACE VIEW wa_fv_view AS SELECT fv.features_value_id, fv.addition_id, v.feature_id, fv.value_id, v.value_value, v.code, a.addition_code FROM ware_additions_features_values fv, features_values v, ware_additions a WHERE fv.value_id = v.value_id AND fv.addition_id = a.addition_id;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWAFVTable (create view)------------ sql request error");
}

void DB::AddWareFSConstraint()
{
    string str;
    str += "ALTER TABLE ware ADD CONSTRAINT features_set_id_fk FOREIGN KEY (features_set_id) REFERENCES features_sets(set_id) ON DELETE SET null;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWareFSConstraint ------------ sql request error");
}

void DB::AddVisualSearchViews()
{
    string str;
    str += "CREATE OR REPLACE VIEW additions_set_of_value AS select distinct fv.addition_id, ' (' || array_to_string(array(SELECT arr.value_value FROM wa_fv_view arr WHERE arr.addition_id = fv.addition_id), ', ' ) || ') ' as set_of_value FROM wa_fv_view fv;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddVisualSearchViews ------------ sql request error");
    str = "CREATE OR REPLACE VIEW visual_search_view AS SELECT w.ware_id, w.internal_id, w.longtitle, w.shortcut, NULL::unknown AS addition_code, w.price, w.quantity, wb.barcode FROM ware w LEFT JOIN ware_barcode wb ON wb.internal_id::text = w.internal_id::text UNION SELECT w.ware_id, w.internal_id, w.longtitle || sov.set_of_value as longtitle, w.shortcut || sov.set_of_value as shortcut, a.addition_code, a.addition_price AS price, a.addition_quantity AS quantity, ab.barcode FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode ab ON ab.addition_code = a.addition_code LEFT JOIN additions_set_of_value sov ON sov.addition_id = a.addition_id";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddVisualSearchViews ------------ sql request error");
}

void DB::AddWareGroupConstraint()
{
    string str;
    str += "ALTER TABLE ware_group ADD CONSTRAINT ware_group_fk FOREIGN KEY(parent_group_id) REFERENCES Ware_Group(group_id) ON DELETE SET null;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWareGroupConstraint ------------ sql request error");
}

void DB::AddWareGroupIdConstraint()
{
    string str;
    str += "ALTER TABLE ware ADD CONSTRAINT ware_group_id_fk FOREIGN KEY(group_id) REFERENCES Ware_Group(group_id) ON DELETE SET null;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddWareGroupIdConstraint ------------ sql request error");
}

void DB::AddSectionsTable()
{
    string str;
    str += "CREATE TABLE sections (section_id serial, section_number integer, head_text varchar(200), bottom_text varchar(200), section_name varchar(20), fiscal_section  boolean, section_cash numeric(10,2), auto_encashment bool, CONSTRAINT section_pk PRIMARY KEY(section_id));";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSectionsTable ------------ sql request error");
}

void DB::AddSaleTimeLimitTable()
{
    string str;
    str += "CREATE TABLE sale_time_limit (start_time time without time zone, count_time time without time zone, id integer NOT NULL default(1));";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSaleTimeLimitTable (create table) ------------ sql request error");
    str = "CREATE OR REPLACE VIEW sale_limit_by_time_view AS SELECT CAST(now()-start_time AS TIME) > count_time AS sale_access_by_time FROM sale_time_limit;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSaleTimeLimitTable (create view) ------------ sql request error");
}
//---------------------------------------
//-----(+)-----Zuskin-----20/02/2012-----
void DB::AddSaleTimeLimitView()
{
    string str;
    str = "DROP VIEW sale_limit_by_time_view";
    doSQL(myconnection, str.c_str());
    str = "CREATE OR REPLACE VIEW sale_limit_by_time_view AS SELECT CAST(now()-start_time AS TIME) > count_time AS sale_access_by_time, id AS id FROM sale_time_limit;";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddSaleTimeLimitView ------------ sql request error");
}
//---------------------------------------

//-----(+)-----Zuskin-----21/10/2011-----
void DB::AlterUseProxyColumnType() {    
    string str;
    str = "ALTER TABLE discount_mobile_config ALTER COLUMN use_proxy TYPE integer";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw(" AlterUseProxyColumnType ---------  sql request error");   
}
//---------------------------------------
//-----(+)-----Zuskin-----27/10/2011-----
int DB::TestIndex(string index_name) {
    string str;
    str += "SELECT indexname FROM pg_indexes WHERE indexname = '";
    str += index_name;
    str += "'";
    
    int state = doSQL(myconnection, str.c_str());   
    
    if ( state != 2 ) throw("index test ---------  sql request error");
    
    int n = PQntuples(result);
    
    PQclear(result);
    
    if ( n == 0 ) {
	return 1;
    }
    
    return 0;
}

void DB::CreateIndex(string index_name, string table_name, string column_name)
{
    string str;
    str += "CREATE INDEX ";
    str += index_name;
    str += " ON ";
    str += table_name;
    str += " (";
    str += column_name;
    str += ")";
    if ( doSQL(myconnection, str.c_str()) < 0 ) {
	throw("CreateIndex ------------ sql request error");
    }
    else {
	printf("Index %s created\n", index_name.c_str());
    }
}

int DB::TestTransViews() {
    string str = "SELECT viewname FROM pg_views WHERE viewname = 'transaction_by_last_session'";
    
    int state = doSQL(myconnection, str.c_str());   
    
    if ( state != 2 ) throw("Transactions views test ---------  sql request error");
    
    int n = PQntuples(result);
    
    PQclear(result);
    
    if ( n == 0 ) {
	return 1;
    }
    
    return 0;

}

//---------------------------------------

//-----(+)-----Zuskin-----16/11/2011-----
void DB::AddManagersTable()
{
    string str;
    str += "CREATE TABLE managers (manager_id integer, manager_name varchar(100), name_to_print varchar(100), barcode varchar(20), cardcode varchar(100), CONSTRAINT managers_pk PRIMARY KEY (manager_id));";
    if ( doSQL(myconnection, str.c_str()) < 0 ) throw("AddManagersTable ------------ sql request error");
}
//---------------------------------------