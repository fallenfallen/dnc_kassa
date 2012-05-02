/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include <string>
#include "update_db.h"
#include "const_conf.h"

int main( int argc, char ** argv )
{
    bool internal_id_to_varchar = false;
    
    try {
	
	DB db(LINCASH_CONF_PATH);
	
	if (db.TestTable(string("documents")) == 1) {
	    puts("table test -------- table absent ");
	    db.AddDocumentTable();
	    puts("document table -------- created ");
	}
	
	if (db.TestSetupParam(string("LearnMode")) == 1) 
	    db.AddSetupParam("0", "LearnMode", "bool", "0");    
	
	if (db.TestSetupParam(string("WriteTransaction")) == 1)
	    db.AddSetupParam("1","WriteTransaction","bool", "1");
	
	if (db.TestSetupParam(string("LimitClientSum")) == 1)
	    db.AddSetupParam("109","LimitClientSum","bool", "1");
	
	if (db.TestSetupParam(string("MaxMoney")) == 1)
	    db.AddSetupParam("110","MaxMoney","int", "5000");
	
	if (db.TestSetupParam(string("PrintOneReg")) == 1)
	    db.AddSetupParam("111","PrintOneReg","bool", "0");
	
	if (db.TestSetupParam(string("ShowAbsentGoods")) == 1)
	    db.AddSetupParam("112","ShowAbsentGoods","bool", "0");
	
	if (db.TestColumn(string("handle_unload"), string("profiles")) == 1) 
	    db.AddRight("handle_unload");
	
	if (db.TestColumn(string("handle_upload"), string("profiles")) == 1) 
	    db.AddRight("handle_upload");
	
	if (db.TestSetupParam("PicturePath", "stringParameters") == 1 )
	    db.AddSetupParam("506", "PicturePath", "", "/usr/share/dnc/movie/k.GIF", "stringParameters");
	
	if (db.TestSetupParam("ReturnCountInc") == 1)
	    db.AddSetupParam("113", "ReturnCountInc", "bool", "0");
	    
	if (db.TestSetupParam("ReportPSReconsiliation") == 1)
	    db.AddSetupParam("114", "ReportPSReconsiliation", "bool", "0");
	
	if (db.TestSetupParam("VSManualInput") == 1)
	    db.AddSetupParam("115", "VSManualInput", "bool", "0");
	
	if (db.TestColumn("ps_reconsiliation", string("profiles")) == 1)
	    db.AddRight("ps_reconsiliation");
	
	if (db.TestConstraint(string("barcode_unique")) == 0)
	    db.DropConstraint(string("ware_barcode"), string("barcode_unique"));
	
	if (db.TestSetupParam("ReportWare") == 1)
	    db.AddSetupParam("116", "ReportWare", "bool", "0");
	
	if (db.TestSetupParam("RoundCheckSum") == 1)
	    db.AddSetupParam("117", "RoundCheckSum", "bool", "0");	
	
	if (db.TestSetupParam("RoundPower") == 1)
	    db.AddSetupParam("118", "RoundPower", "double", "0.5");
	
	if (db.TestSetupParam("RoundType") == 1)
	    db.AddSetupParam("119", "RoundType", "int", "0");
	
	if (db.TestColumn(string("ware_id"), string("ware")) == 1) {
	    db.ChangeOwner("ware", db.Get_User());
	    db.AddColumn("ware", "ware_id", "serial");
	    db.IntColumnToVarchar("ware", "internal_id");
	    db.ChangeOwner("ware_barcode",db.Get_User());
	    db.IntColumnToVarchar("ware_barcode","internal_id");
	    internal_id_to_varchar = true;
	}
	//-----(+)-----Zuskin-----11/07/2011-----
	if (db.TestTable(string("features_sets")) == 1) {
	    puts("table features_sets test -------- table absent ");
	    db.AddFeatureSetsTable();
	    puts("features_sets table -------- created ");
	}
	if (db.TestTable(string("features")) == 1) {
	    puts("table features test -------- table absent ");
	    db.AddFeaturesTable();
	    puts("features table -------- created ");
	}
	if (db.TestTable(string("features_values")) == 1) {
	    puts("table features_values test -------- table absent ");
	    db.AddFeaturesValuesTable();
	    puts("features_values table -------- created ");
	}
	if (db.TestTable(string("features_in_sets")) == 1) {
	    puts("table features_in_sets test -------- table absent ");
	    db.AddFeaturesInSetsTable();
	    puts("features_in_sets table -------- created ");
	}
	if (db.TestConstraint(string("ware_pk")) == 0) {
	    db.DropConstraint(string("ware"), string("ware_pk"));
	}
	if (db.TestConstraint(string("ware_id_pk")) == 1) {
	    db.AddWareConstraint();
	}
	
	if (db.TestTable(string("ware_additions")) == 1) {
	    puts("table ware_additions test -------- table absent ");
	    db.AddWareAdditionsTable();
	    puts("ware_additions table -------- created ");
	}
	if (db.TestTable(string("ware_additions_features_values")) == 1) {
	    puts("table ware_additions_features_values test -------- table absent ");
	    db.AddWAFVTable();
	    puts("ware_additions_features_values table -------- created ");
	}
	if (db.TestColumn(string("features_set_id"), string("ware")) == 1) {
	    db.ChangeOwner("ware", db.Get_User());
	    db.AddColumn("ware", "features_set_id", "integer");
	}
	if (db.TestConstraint(string("features_set_id_fk")) == 1) {
	    db.AddWareFSConstraint();
	}

	if (db.TestColumn(string("addition_code"), string("ware_barcode")) == 1) {
	    db.ChangeOwner("ware_barcode", db.Get_User());
	    db.AddColumn("ware_barcode", "addition_code", "varchar(20)");
	}

	if (db.TestColumn(string("article"), string("ware")) == 1) {
	    db.ChangeOwner("ware", db.Get_User());
	    db.AddColumn("ware", "article", "varchar(20)");
	}
	
	db.AddVisualSearchViews();

	if (db.TestColumn(string("group_code"), string("ware_group")) == 1) {
	    db.ChangeOwner("ware_group", db.Get_User());
	    db.AddColumn("ware_group", "group_code", "varchar(20)");
	}
	
	if (db.TestConstraint(string("ware_group_fk")) == 0) {
	    db.DropConstraint(string("ware_group"), string("ware_group_fk"));
	    db.AddWareGroupConstraint();
	}

	if (db.TestConstraint(string("ware_group_id_fk")) == 0) {
	    db.DropConstraint(string("ware"), string("ware_group_id_fk"));
	    db.AddWareGroupIdConstraint();
	}
	
	if (db.TestColumn(string("section"), string("ware")) == 1) {
	    db.ChangeOwner("ware", db.Get_User());
	    db.AddColumn("ware", "section", "integer");
	}

	if (db.TestTable(string("sections")) == 1) {
	    puts("table sections test -------- table absent ");
	    db.AddSectionsTable();
	    puts("sections table -------- created ");
	}
	//-----(+)-----Zuskin-----21/10/2011-----
	if (db.TestColumn(string("section_cash"), string("sections")) == 1) {
	    db.ChangeOwner("sections", db.Get_User());
	    db.AddColumn("sections", "section_cash", "numeric(10,2)");
	}
	if (db.TestColumn(string("auto_encashment"), string("sections")) == 1) {
	    db.ChangeOwner("sections", db.Get_User());
	    db.AddColumn("sections", "auto_encashment", "boolean");
	}
	//---------------------------------------
	if (db.TestTable(string("sale_time_limit")) == 1) {
	    puts("table sale_time_limit -------- table absent ");
	    db.AddSaleTimeLimitTable();
	    puts("sale_time_limit table -------- created ");
	}
	if (db.TestColumn(string("sale_limit_by_time"), string("ware")) == 1) {
	    db.ChangeOwner("ware", db.Get_User());
	    db.AddColumn("ware", "sale_limit_by_time", "integer");
	}
	//---------------------------------------
	//-----(+)-----Zuskin-----11/07/2011-----
	if (db.TestColumn(string("summ"), string("fix_reduction")) == 1) {
	    db.ChangeOwner("fix_reduction", db.Get_User());
	    db.AddColumn("fix_reduction", "summ", "numeric(10,2)");
	}
	
	if (db.TestColumn(string("maxbonuspercent"), string("fix_reduction")) == 1) {
	    db.ChangeOwner("fix_reduction", db.Get_User());
	    db.AddColumn("fix_reduction", "maxbonuspercent", "numeric(10,2)");
	}
	
	if (db.TestColumn(string("accumulation"), string("auto_reduction")) == 1) {
	    db.ChangeOwner("auto_reduction", db.Get_User());
	    db.AddColumn("auto_reduction", "accumulation", "numeric(15,2)");
	}
	//---------------------------------------
		
	//-----(+)-----Zuskin-----27/10/2011-----
	if (db.TestIndex(string("longtitle_idx")) == 1) {
	    db.CreateIndex(string("longtitle_idx"), string("ware"), string("longtitle")); 
	}
	//---------------------------------------	
	//-----(+)-----Zuskin-----16/11/2011-----
	if (db.TestTable(string("managers")) == 1) {
	    puts("table managers test -------- table doesn't exist ");
	    db.AddManagersTable();
	    puts("managers table -------- created ");
	}
	//---------------------------------------
	//-----(+)-----Zuskin-----13/12/2011-----
	if (db.TestColumn(string("is_default"), string("auto_scheme")) == 1) {
	    db.ChangeOwner("auto_scheme", db.Get_User());
	    db.AddColumn("auto_scheme", "is_default", "boolean default(false)");
	}
	//-----(+)-----Zuskin-----20/02/2012-----
	if (db.TestColumn(string("id"), string("sale_time_limit")) == 1) {
	    db.ChangeOwner("sale_time_limit", db.Get_User());
	    db.AddColumn("sale_time_limit", "id", "integer NOT NULL default(1)");
	}
	//---------------------------------------
	//---(+)---Zuskin---20/02/2012---
	db.AddSaleTimeLimitView();
	//-------------------------------
    } catch(const char* str) {
	puts(str);
    }
    
    try {
	DB trans_db(TRANSACTION_CONF_PATH);
	
	if (trans_db.TestColumn(string("barcode"), string("transaction")) == 1) 
	    trans_db.AddTransColumn(string("barcode"), string("varchar(20)"));
	if ( internal_id_to_varchar ) {
	   trans_db.IntColumnToVarchar("transaction", "code"); 
	}
	if (trans_db.TestColumn(string("round_sum"), string("transaction")) == 1) 
	    trans_db.AddTransColumn(string("round_sum"), string("numeric(15,3)"));
	//-----(+)-----Zuskin-----11/07/2011-----
	if (trans_db.TestTransactionType(string("12")) == 1)
	    trans_db.AddTransactionType(string("12"), string("Сторно"));
	if (trans_db.TestTransactionType(string("56")) == 1)
	    trans_db.AddTransactionType(string("56"), string("Отмена чека"));
	//trans_db.CreateTransViews();
	//---------------------------------------	
	//-----(+)-----Zuskin-----11/10/2011-----
	if (trans_db.TestTransactionType(string("57")) == 1)
	    trans_db.AddTransactionType(string("57"), string("Чек отложен"));
	if (trans_db.TestTransactionType(string("58")) == 1)
	    trans_db.AddTransactionType(string("58"), string("Отложенный чек восстановлен"));    
	//---------------------------------------	
	//-----(+)-----Zuskin-----18/10/2011-----
	if (trans_db.TestTransViews() == 0) {
	    trans_db.DropTransViews();
	}    
	trans_db.AlterRoundSumColumnType();
	trans_db.CreateTransViews();
	//---------------------------------------
	//-----(+)-----Zuskin-----16/11/2011-----
	if (trans_db.TestColumn(string("manager"), string("transaction")) == 1) 
	    trans_db.AddTransColumn(string("manager"), string("integer"));
	//---------------------------------------
	
    } catch(const char* str) {
	puts(str);
    }
    //-----(+)-----Zuskin-----21/10/2011-----
    try {
	DB eds_db("/etc/dancy/dnc_eds.conf");
	eds_db.AlterUseProxyColumnType();
    } catch(const char* str) {
	puts(str);
    }
    
    puts("End of Update");
}


