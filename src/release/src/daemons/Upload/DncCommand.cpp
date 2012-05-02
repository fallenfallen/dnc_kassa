/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "DncCommand.h"

#include "stringconvert.h"
#include "Decoder.h"
#include "WareSystem.h"
#include "FeaturesSet.h"
#include "UsefulFunctions.h"

DncCommand::DncCommand() {
}

DncCommand* DncCommand::findCommand(const string& command_name) {
    if ( command_name == "!!!ADDFEATURESETS" )
        return new DncCommandAddFeaturesSets;
    if ( command_name == "!!!ADDFEATURES" )
        return new DncCommandAddFeatures;
    if ( command_name == "!!!ADDFEATURESVALUES" )
        return new DncCommandAddFeaturesValues;
    if ( command_name == "!!!ADDWARES" )
        return new DncCommandAddWares(false);
    if ( command_name ==  "!!!REPLACEWARES" )
        return new DncCommandAddWares(true);
    if ( command_name == "!!!ADDWAREBARCODES" )
        return new DncCommandAddBarcodes;
    if ( command_name == "!!!ADDWAREADDITIONS" )
        return new DncCommandAddWareAdditions(false);
    if ( command_name == "!!!REPLACEWAREADDITIONS" )
        return new DncCommandAddWareAdditions(true);
    if ( command_name == "!!!DNCUPLOAD_END" )
        return new DncCommandEnd;
    if ( command_name == "!!!ADDWAREGROUPS" )
        return new DncCommandAddWareGroup;
    if ( command_name == "!!!ADDFIXDISCOUNTS" )
        return new DncCommandAddFixedDiscount;
    if ( command_name == "!!!ADDSALESRESTRICTIONBYTIME" )
        return new DncCommandAddSaleRestrictionByTime;
    if ( command_name == "!!!ADDPROFILES" )
        return new DncCommandAddProfiles;
    if ( command_name == "!!!ADDUSERS" )
        return new DncCommandAddUsers;
    //-----(+)-----Zuskin-----18/07/2011-----
    if ( command_name == "!!!ADDDISCSCHEMES" )
	return new DncCommandAddDiscSchemes;
    if ( command_name == "!!!ADDDISCCONDITIONS" )
	return new DncCommandAddDiscConditions;	
    //---------------------------------------
    //-----(+)-----Zuskin-----21/11/2011-----
    if ( command_name == "!!!ADDMANAGERS" )
	return new DncCommandAddManagers;
    //---------------------------------------
    //-----(+)-----Zuskin-----22/11/2011-----
    if ( command_name == "!!!ADDTAXGROUPS" )
	return new DncCommandAddTaxGroups;
    //---------------------------------------
    //-----(+)-----Zuskin-----11/01/2012-----
    if ( command_name == "!!!ADDTAXRATES" )
	return new DncCommandAddTaxRates;
    //---------------------------------------	
    
    if ( command_name == "!!!DELETEADITIONS" ) {
	DncCommandDeleteAdditions::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!DELETEFEATURES" ) {
	DncCommandDeleteFeatures::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!DELETEWARES" ) {
	DncCommandDeleteWares::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!CLEARWAREDATABASE" ) {
	DncCommandClearWareSystem::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!DELETEFIXDISCOUNTS" ) {
        DncCommandDeleteFixdiscount::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!DELETEPROFILES" ) {
        DncCommandDeleteProfiles::exec();
        return getDefaulCommand();
    }
    if ( command_name == "!!!DELETEUSERS" ) {
        DncCommandDeleteUsers::exec();
        return getDefaulCommand();
    }
    //-----(+)-----Zuskin-----19/07/2011-----
    if ( command_name == "!!!DELETEDISCSCHEMES" ) {
        DncCommandDeleteDiscSchemes::exec();
        return getDefaulCommand();
    }
    //---------------------------------------
    //-----(+)-----Zuskin-----21/11/2011-----
    if ( command_name == "!!!DELETEMANAGERS" ) {
        DncCommandDeleteManagers::exec();
	return getDefaulCommand();
    }	
    //-----(+)-----Zuskin-----12/01/2012-----
    if ( command_name == "!!!DELETETAXES" ) {
        DncCommandDeleteTaxes::exec();
	return getDefaulCommand();
    }	
    //---------------------------------------
    //cout << "Команда не поддерживается данным протоколом. Будет использована команда по умолчанию!!!" << endl;
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Команда не поддерживается данным протоколом. Будет использована команда по умолчанию!!!")) );
    logfile.PrintString(log_type, ERROR, "Command not found = <%s>\n", command_name.c_str());
	
    return getDefaulCommand();
}

DncCommand * DncCommand::getDefaulCommand() {
    return new DncCommandAddWares;
}

bool DncCommand::noEnd() {
    return true;
}

int DncCommand::execute(const string& line) {
    
    selectParam(line);
    try {
        executeCommand();
    } catch (const Error & err) {
	catchError(err);
        return -1;
    }

    return 0;
}

void DncCommand::selectParam(const string& line) {
    params.clear();
    string utf_line = fromWinFile(line);
    params = parseStringWithSeparator(utf_line, ";");    
}

void DncCommand::addBarcode(Barcode * barcode) {
    BarcodeCatalog * catalog = WareSystem::getInstance()->getBarcodeCatalog();
    catalog->addBarcode(barcode);
}

void DncCommand::catchError(const Error & err) {
    //cout << "Ошибка " << err.getErrorCode() << " " << err.getErrorUserText() << endl;
    //cout << err.getErrorLogText() << endl;
    
    string user_err_text = "Ошибка " + toString<int>(err.getErrorCode()) + " " + err.getErrorUserText();
    string log_err_text = user_err_text + " " + err.getErrorLogText();
    
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode(user_err_text.c_str())) );
    logfile.PrintString(log_type, ERROR, log_err_text.c_str());    
    
}

/*************************************************************/

bool DncCommandEnd::noEnd() {
    return false;
}
void DncCommandEnd::executeCommand() { }

/*************************************************************/

void DncCommandAddFeaturesSets::executeCommand() {

    //cout << "load features set" << endl;

        if ( params.size() < 2 )
            throw Error(221, "Строка не соответствует протоколу", "FeatureSet not added");

        FeaturesCatalog * fcatalog = WareSystem::getInstance()->getFeaturesCatalog();
        FeaturesSet fset(params[1]);
        fset.setCode(params[0]);
        fcatalog->addFeaturesSet(fset);

    //cout << "fs load ---  ok" << endl;
}

/*************************************************************/

void DncCommandAddFeatures::executeCommand() {
    //cout << "load feature " << endl;
    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "Feature not added");

    FeaturesCatalog * fcatalog = WareSystem::getInstance()->getFeaturesCatalog();

    Feature feature(params[1]);
    feature.setCode(params[0]);
    string fset_code = params[2];
    fcatalog->addFeatureInSet(feature, fset_code);

    //cout << "  f load ----- ok" << endl;
}

/*************************************************************/

void DncCommandAddFeaturesValues::executeCommand() {
    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "FeatureValue not added");

    FeaturesCatalog * fcatalog = WareSystem::getInstance()->getFeaturesCatalog();
    string f_code = params[2];
    Feature * feature_by_code = fcatalog->getFeatureByCode(f_code);
    FeatureValue value(params[1]);
    value.setCode(params[0]);
    feature_by_code->addPossibleValue(value);
}

/*************************************************************/
DncCommandAddWares::DncCommandAddWares(bool replace_quantity) {
    this->replace_quantity = replace_quantity;
}

void DncCommandAddWares::executeCommand() {
    int param_count = params.size();
    if ( param_count < 8 )
        throw Error(221, "Строка не соответствует протоколу", "Ware not added");

    WareCatalog * catalog = WareSystem::getInstance()->getWareCatalog();
    WareInfo ware;
    ware.internal_code = params[0];
    //-----(*)-----Zuskin-----14/07/2011-----
    //ware.title = params[3];
    //ware.text = params[4];
    ware.title = params[3];
    replace164(ware.title);
    maskApostroff(ware.title);
    ware.text = params[4];
    replace164(ware.text);
    maskApostroff(ware.text);
    ware.article = params[5];
    replace164(ware.article);
    //---------------------------------------
    ware.price = fromString<double>(params[6]);
    ware.quantity =  fromString<double>(params[7]);
    if (param_count > 8)
        ware.flags.setFlags(params[8]);
    if ( param_count > 9 )
        ware.min_price = fromString<double>(params[9]);
    if ( param_count > 10 ) {
        if ( params[10] == "0" )
            ware.expiration = "";
        else
            ware.expiration = params[10];
    }

    if ( param_count > 11 ) {
        if ( !params[11].empty()  ) {
            if (params[11] != "0") {
                WareGroup * parent_group = catalog->getWareGroupByCode(params[11]);
                ware.group_id = parent_group->getOID();
            }
        }
    }

    if (param_count > 13) {
	if ( params[13] == "0" ) {
	    ware.features_set_code = "";
	} else {
	    ware.features_set_code = params[13];
	}
    }

    if (param_count > 14)
        ware.section_num = fromString<int>(params[14]);

    if (param_count > 15)
        //---(*)---Zuskin---20/02/2012---
        //ware.sale_limit_by_time = fromString<int>(params[15]) == 1;
        ware.sale_limit_by_time = fromString<int>(params[15]);
        //-------------------------------

    catalog->addWare(ware, replace_quantity);
    addBarcodes();
}

void DncCommandAddWares::addBarcodes() {
    Barcode barcode;
    barcode.setWareInternalCode(params[0]);
    barcode.setCoefficient(fromString<double>(params[2]));

    string barcodes_str = params[1];
    vector<string> barcodes = parseStringWithSeparator(barcodes_str, ",");

    for (int i = 0; i < barcodes.size(); i++) {
        barcode.setText(barcodes[i]);
        barcode.setMainBarcode( i== 0 );
        addBarcode(&barcode);
    }

}

/*************************************************************/

void DncCommandAddBarcodes::executeCommand() {
    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "Barcode not added");

    Barcode barcode;
    barcode.setText(params[0]);
    barcode.setWareInternalCode(params[1]);
    barcode.setCoefficient(fromString<double>(params[3]));
    barcode.setAdditionCode(params[2]);

    addBarcode(&barcode);
}

/*************************************************************/

DncCommandAddWareAdditions::DncCommandAddWareAdditions(bool replace_quantity) {
    this->replace_quantity = replace_quantity;
}

void DncCommandAddWareAdditions::executeCommand() {
    if ( params.size() < 8 )
        throw Error(221, "Строка не соответствует протоколу", "WareAddition not added");

    WareCatalog * wcatalog = WareSystem::getInstance()->getWareCatalog();
    string w_code = params[1];
    Ware * ware = wcatalog->getWareByCode(w_code);
    
    WareAdditionInfo addition_info;

    addition_info.code = params[0];
    addition_info.article = params[2];
    //-----(+)-----Zuskin-----15/07/2011-----
    replace164(addition_info.article);
    //--------------------------------------- 
        
    addition_info.price = fromString<double>(params[5]);
    addition_info.quantity = fromString<double>(params[6]);

    WareAddition ware_addition;
    ware_addition.replaceInfo(addition_info);

    ware->addAddition(&ware_addition, replace_quantity);
    
    WareAddition * added_addition = ware->getAdditionByCode(ware_addition.getCode());

    addAdditionBarcode();

    addSelectedValue(added_addition);
}
void DncCommandAddWareAdditions::addAdditionBarcode() {
    Barcode barcode;
    barcode.setText(params[3]);
    barcode.setCoefficient(fromString<double>(params[4]));
    barcode.setAdditionCode(params[0]);
    barcode.setMainBarcode(true);
    addBarcode(&barcode);
}

void DncCommandAddWareAdditions::addSelectedValue(WareAddition* addition) {
    string selected_value_string = params[7];

    vector<string> values = parseStringWithSeparator(selected_value_string, ",");

    for (int i = 0; i < values.size(); i++) {
        //int vcode = fromString<int>(values[i]);
        //if (vcode > 0)
        //    addition->setSelectedValueByCode(toString<int>(vcode));
        if (values[i].length() > 0) {
    	    addition->setSelectedValueByCode(values[i]);
        }
    }

}

/*************************************************************/

void DncCommandAddWareGroup::executeCommand() {
    //cout << "DncCommandAddWareGroup::executeCommand()" << endl;
     if ( params.size() < 4 )
        throw Error(221, "Строка не соответствует протоколу", "WareGroup not added");

    WareCatalog * wcatalog = WareSystem::getInstance()->getWareCatalog();

    WareGroup * group = new WareGroup();
    group->setCode(params[0]);
    group->setTitle(params[1]);
    group->setText(params[2]);

    if ( !params[3].empty() ) {
        if ( params[3] != "0" ) {
	    //cout << params[3] << endl;
            WareGroup * parent = wcatalog->getWareGroupByCode(params[3]);
            group->setParent(parent);
        }
    }

    //cout << group->getParentId() << endl;
    wcatalog->addGroup(group);

    delete group;

//    cout << "DncCommandAddWareGroup::executeCommand() ------- ok" << endl;
}

/*************************************************************/

void DncCommandAddFixedDiscount::executeCommand() {
    
    if ( params.size() < 7 )
        throw Error(221, "Строка не соответствует протоколу", "FixedDiscount not added");
    
    
    
    /*for (int i = 0; i < params.size(); i++) {
	printf("%s |", params[i].c_str());
    }
    puts(""); */    
    
    DiscountCatalog * dcatalog = WareSystem::getInstance()->getDiscountCatalog();

    FixedDiscountInfo fdiscountinfo;

    fdiscountinfo.code = params[0];
    fdiscountinfo.title = params[1];
    fdiscountinfo.text = params[2];
    fdiscountinfo.card_code = params[3];
    fdiscountinfo.type = (FixedDiscountType)fromString<int>(params[4]);
    fdiscountinfo.rate = fromString<double>(params[5]);
    fdiscountinfo.range = (DiscountRange)fromString<int>(params[6]);

    //-----(*)-----Zuskin-----30/06/2011-----
    //if ( params.size() > 6 )
    //    if ( !params[6].empty() )
    //        fdiscountinfo.barcode = params[6];
    //if ( params.size() > 7 ) {
    //    fdiscountinfo.visible = params[7] == "1" ? true : false;
    //}
    
    if ( params.size() > 7 )
        if ( !params[7].empty() )
            fdiscountinfo.barcode = params[7];

    if ( params.size() > 8 ) {
        fdiscountinfo.visible = params[8] == "1" ? true : false;
    }
    //----------------------------------------
    //-----(+)-----Zuskin-----19/07/2011-----
    if ( params.size() > 9 ) {
	fdiscountinfo.summ = fromString<double>(params[9]);
    }
    
    if ( params.size() > 10 ) {
	fdiscountinfo.scheme_id = fromString<int>(params[10]);
    }
    
    if ( params.size() > 11 ) {
	fdiscountinfo.maxbonuspercent = fromString<double>(params[11]);
    }
    
    //---------------------------------------
    dcatalog->addFixedDiscount(fdiscountinfo);
    
}

/*************************************************************/

void DncCommandAddSaleRestrictionByTime::executeCommand() {
    if ( params.size() < 2 )
        throw Error(221, "Строка не соответствует протоколу", "RestrictionByTime not added");

    WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
    
    //---(*)---Zuskin---20/02/2012---
    //ware_catalog->updateSaleRestrictionByTime(params[0], params[1]);
    if (params.size() > 2) {
	ware_catalog->updateSaleRestrictionByTime(params[0], params[1], fromString<int>(params[2]));
    }
    else {
	ware_catalog->updateSaleRestrictionByTime(params[0], params[1], 1);
    }
    //-------------------------------
}

/*************************************************************/

void DncCommandAddProfiles::executeCommand() {
    if ( params.size() < 30 )
        throw Error(221, "Строка не соответствует протоколу", "Profile not added");

    UserProfileInfo p_info;

    p_info.profile_code = fromString<int>(params[0]);
    p_info.profile_name = params[1];
    
    int param_num_start = 2;
    for ( int param_num = param_num_start; param_num < RIGHTS_COUNT + param_num_start; param_num++ ) {
        p_info.access_rigths[param_num - param_num_start] = params[param_num] == "1";
    }

    AuthorizationSystem * as = WareSystem::getInstance()->getAuthorizationSystem();
    as->addProfile(p_info);
    
}

/*************************************************************/

void DncCommandAddUsers::executeCommand() {
    if ( params.size() < 7 )
        throw Error(221, "Строка не соответствует протоколу", "User not added");

    AuthorizationSystem * as = WareSystem::getInstance()->getAuthorizationSystem();
    UserProfile * profile = as->getProfileById(fromString<int>(params[6]));
    
    UserInfo user_info;

    user_info.code = fromString<int>(params[0]);
    user_info.short_name = params[1];
    user_info.long_name = params[2];
    user_info.barcode = params[3];
    user_info.card_code = params[4];
    user_info.password = params[5];
    user_info.profile_id =profile->getOID();
    
    as->addUser(user_info);
    
}

//-----(+)-----Zuskin-----18/07/2011-----
void DncCommandAddDiscSchemes::executeCommand() {

    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "Discount scheme not added");

/*    for (int i = 0; i < params.size(); i++) {
	printf("%s |", params[i].c_str());
    }
    puts(""); */

    DiscountCatalog * dcatalog = WareSystem::getInstance()->getDiscountCatalog();
    DiscSchemeInfo disc_scheme;
    
    disc_scheme.code = fromString<int>(params[0]);
    disc_scheme.short_name = params[1];
    disc_scheme.long_name = params[2];
    //-----(+)-----Zuskin-----14/12/2011-----
    if ( params.size() > 3 ) {
	disc_scheme.is_default = fromString<bool>(params[3]);
    }
    if (disc_scheme.is_default == true) {
	dcatalog->clearDefaultDiscScheme();
    }
    //---------------------------------------
    
    dcatalog->addDiscScheme(disc_scheme);
    
    
}    

void DncCommandAddDiscConditions::executeCommand() {

    if ( params.size() < 12 )
        throw Error(221, "Строка не соответствует протоколу", "DiscountCondition not added");
        
/*    for (int i = 0; i < params.size(); i++) {
	printf("%s |", params[i].c_str());
    }
    puts(""); */

    DiscountCatalog * dcatalog = WareSystem::getInstance()->getDiscountCatalog();
    DiscRuleInfo disc_rule;
    
    disc_rule.code = fromString<int>(params[0]);
    disc_rule.scheme = fromString<int>(params[1]);
    disc_rule.short_name = params[2];
    disc_rule.long_name = params[3];
    disc_rule.value = fromString<double>(params[4]);
    disc_rule.type = fromString<int>(params[5]);
    disc_rule.bdate = convertDate(params[6]); 
    disc_rule.edate = convertDate(params[7]);
    disc_rule.btime = params[8];
    disc_rule.etime = params[9]; 
    
    int wd = convertStringOfZeros(params[10]); 
//    if (wd == -1) wd = 127;
    disc_rule.weekdays = wd; 
    disc_rule.sumcheck = fromString<double>(params[11]);
    if ( params.size() > 12 ) {
	disc_rule.accumulation = fromString<double>(params[12]);
    }
    
    dcatalog->addDiscRule(disc_rule);
    
}    
//---------------------------------------

//-----(+)-----Zuskin-----21/11/2011-----
void DncCommandAddManagers::executeCommand() {

    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "Manager wasn't added");

//    for (int i = 0; i < params.size(); i++) {
//	printf("%s |", params[i].c_str());
//    }
//    puts(""); 

    ManagerCatalog * mcatalog = WareSystem::getInstance()->getManagerCatalog();
    ManagerInfo manager;
    
    manager.code = fromString<int>(params[0]);
    manager.short_name = params[1];
    manager.long_name = params[2];
    if ( params.size() > 3 ) {
	manager.barcode = params[3]; 
    }
    if ( params.size() > 4 ) {
	manager.cardcode = params[4];
    }
    mcatalog->addManager(manager);
    
}    
//--------------------------------------

//-----(+)-----Zuskin-----22/11/2011-----
void DncCommandAddTaxGroups::executeCommand() {

    //printf("void DncCommandAddTaxGroups::executeCommand() {\n");
    if ( params.size() < 3 )
        throw Error(221, "Строка не соответствует протоколу", "Tax group wasn't been added");

    //for (int i = 0; i < params.size(); i++) {
    //	printf("%s |", params[i].c_str());
    //}
    //puts(""); 

    TaxesCatalog * tcatalog = WareSystem::getInstance()->getTaxesCatalog();
    TaxGroupInfo taxgroup;
    
    taxgroup.code = fromString<int>(params[0]);
    taxgroup.short_name = params[1];
    taxgroup.long_name = params[2];
    tcatalog->addTaxGroup(taxgroup);
    //printf("DncCommandAddTaxGroups::executeCommand() end\n");
    
}    
//--------------------------------------

//-----(+)-----Zuskin-----11/01/2012-----
void DncCommandAddTaxRates::executeCommand() {

    //printf("void DncCommandAddTaxRates::executeCommand() {\n");
    if ( params.size() < 7 )
        throw Error(221, "Строка не соответствует протоколу", "Tax rate wasn't been added");

    TaxesCatalog * tcatalog = WareSystem::getInstance()->getTaxesCatalog();
    TaxRateInfo taxrate;
    TaxRateGrInfo taxrategr;
    
    taxrate.code = fromString<int>(params[0]);
    taxrate.short_name = params[1];
    taxrate.long_name = params[2];
    //taxrate.type = fromString<int>(params[3]);
    taxrate.value = fromString<double>(params[4]);
    
    taxrategr.code = taxrate.code;
    taxrategr.rate_id = taxrate.code;
    taxrategr.group_id = fromString<int>(params[5]);
    taxrategr.switchbase = params[6] == "1" ? true : false;
        
    tcatalog->addTaxRate(taxrate);
    tcatalog->addTaxRateGr(taxrategr);
    //printf("DncCommandAddTaxRates::executeCommand() end\n");
    
}    
//--------------------------------------


/*************************************************************/

void DncCommandDeleteAdditions::exec() {
    try {
	WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
	ware_catalog->dropAllAdditions();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandDeleteWares::exec() {
    try {
	WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
	ware_catalog->clearCatalog();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandDeleteFeatures::exec() {
    try {
	FeaturesCatalog * f_catalog = WareSystem::getInstance()->getFeaturesCatalog();
	f_catalog->clearCatalog();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandClearWareSystem::exec() {
    try {
	WareSystem::getInstance()->clearWareSystem();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandDeleteFixdiscount::exec() {
    try {
        DiscountCatalog * dcatalog = WareSystem::getInstance()->getDiscountCatalog();
        dcatalog->clearCatalog();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandDeleteUsers::exec() {
    try {
        AuthorizationSystem * as = WareSystem::getInstance()->getAuthorizationSystem();
        as->clearUsers();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/

void DncCommandDeleteProfiles::exec() {
    try {
        AuthorizationSystem * as = WareSystem::getInstance()->getAuthorizationSystem();
        as->clearAuthorizationSystem();
    } catch (const Error & err) {
	catchError(err);
    }
}

/*************************************************************/
//-----(+)-----Zuskin-----19/07/2011-----

void DncCommandDeleteDiscSchemes::exec() {
    try {
	DiscountCatalog * dcatalog = WareSystem::getInstance()->getDiscountCatalog();
        dcatalog->clearDiscSchemes();
    } catch (const Error & err) {
	catchError(err);
    }
}

//---------------------------------------

//-----(+)-----Zuskin-----21/11/2011-----

void DncCommandDeleteManagers::exec() {
    try {
	ManagerCatalog * mcatalog = WareSystem::getInstance()->getManagerCatalog();
        mcatalog->clearCatalog();
    } catch (const Error & err) {
	catchError(err);
    }
}

//---------------------------------------

//-----(+)-----Zuskin-----12/01/2012-----

void DncCommandDeleteTaxes::exec() {
    try {
	TaxesCatalog * tcatalog = WareSystem::getInstance()->getTaxesCatalog();
        tcatalog->clearCatalog();
    } catch (const Error & err) {
	catchError(err);
    }
}

//---------------------------------------


//-----(+)-----Zuskin-----14/07/2011-----
void maskApostroff(string& str) {
    
    int pos = str.find("'", 0);
    
    while ( pos != string::npos ) {
	str.insert(pos, "'");
	pos++;
	pos = str.find("'", pos + 1);
    }
    
    pos = str.find("\\", 0);
        
    while ( pos != string::npos ) {
	str.insert(pos, "\\");
	pos++;
	pos = str.find("\\", pos + 1);
    }
}

void replace164(string& str) {
    string tmpl = "¤";

    int pos = str.find(tmpl, 0);
    while ( pos != string::npos ) {
	str.replace(pos, tmpl.length(), ";");
	pos++;
	pos = str.find(tmpl, pos + 1);
    }
}
//----------------------------------------