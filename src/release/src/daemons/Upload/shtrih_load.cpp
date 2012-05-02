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

#include "shtrih_load.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "const_conf.h"
#include "const.h"


QTextCodec * DbCodec = QTextCodec::codecForName(DBCODEC);
QTextCodec * CP1251Codec = QTextCodec::codecForName("CP1251");

/******************************* ShtrihLoad ***********************************/

ShtrihLoad::ShtrihLoad()
{
    command_ware_clear = false;
    command_fix_clear = false;
    command_add_quantity = false;
    err_count = 0;
    pg_upload = NULL;
}


ShtrihLoad::~ShtrihLoad()
{
    // удалить объект подключения к базе
    if ( pg_upload != NULL ) delete pg_upload;
}


int ShtrihLoad::load(const char* file_load_path) 
{
    
    //проверить подклчение к базе
    pg_upload = new PgUpload(LINCASH_CONF_PATH);
    //er_con = pg_upload->er_con;
    
    if ( pg_upload->er_con != 0 ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Отсутствует подключение к базе данных \r\n")) );
	logfile.PrintString(log_type, ERROR, "Data base connect error \n");
	return -1;
    }
    
    string line;
    ifstream loadfile(file_load_path);
    
    if ( ! loadfile.is_open() )  {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось открыть файл загрузки \r\n")) );
	logfile.PrintString(log_type, ERROR, "Couldn't open data file %s \n", file_load_path);
	return -1;
    }
    
    string tmp_file_path;
    tmp_file_path += file_load_path;
    tmp_file_path += "~";
    
    ofstream tmpfile(tmp_file_path.c_str());
    
    if ( ! tmpfile.is_open() ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось перезаписать файл \r\n")) );
	logfile.PrintString(log_type, INFO, "Couldn't rewrite data file \n");
	
	loadfile.close();
	return -1;
    }
    
    
    getline(loadfile,line);
    //cout << " line =  " << line << endl;
    
    string file_first_string = "##@@&&";
    string file_readed_string = "@";
    string file_not_readed_string = "#";        
    
    if ( (line.length() <= 0) || (line.find(file_first_string) != 0) ) {
	userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Формат файла не соответствует протоколу \r\n")) );
	logfile.PrintString(log_type, INFO, "File format isn't right \n");
	return -1;
    }
    
    if ( line.length() > file_first_string.length() ) {
	testCommand(line.substr(file_first_string.length()));
    }
    
    tmpfile << line << endl;
    
    if ( !loadfile.eof() ) {
	getline (loadfile,line);	
	//cout << "line = " << line << endl;
	if ( line.find(file_readed_string) == 0 ) {
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Файл уже был обработан \r\n")) );
	    logfile.PrintString(log_type, INFO, "File of load was processed earlier \n");
	    tmpfile.close();
	    return 1;
	}
	if ( line.find(file_not_readed_string) != 0 ) {
	    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Формат файла не соответствует протоколу \r\n")) );
	    logfile.PrintString(log_type, INFO, "File format isn't right \n");
	    tmpfile.close();
	    return -1;
	}	
    }
    
    tmpfile << file_readed_string << endl;
    
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Запущен процесс загрузки товаров \r\n")) );
    logfile.PrintString(log_type, INFO, "Ware load start\n");
    userlog.SetMode(false);
    logfile.SetMode(false);
    
    
    int i = 3;
    while (! loadfile.eof() )    {
	getline (loadfile,line);
	//tmpfile << line << "\r\n";
	//tmpfile << line;
	tmpfile << line << endl;
	//cout << "i = " << i << " " << line << endl;
	if ( (line.find("$$$") != 0) && (line.length() > 0) ) {
	    //puts("commands push back");
	    commands.push_back(Command(line, i));
	}
	if (line.find("$$$") == 0) testCommand(line);
	i++;
    }
    loadfile.close();
    tmpfile.close();
    
    executeCommand();
    
    remove(file_load_path);
    rename(tmp_file_path.c_str(), file_load_path);
    remove(tmp_file_path.c_str());
    chmod(file_load_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); 
    
    
    userlog.SetMode(true);
    logfile.SetMode(true);	
    userlog.PrintString(user_log_type, INFO,WinCodec->fromUnicode(SystemCodec->toUnicode( "Загрузка товаров успешно завершена \r\n")) );
    logfile.PrintString(log_type, INFO, "Ware load successfully complete \n");
    userlog.SetMode(false);
    logfile.SetMode(false);		      
    userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Всего ошибок : %d\r\n")), err_count  );
    logfile.PrintString(log_type, INFO, "Errors : %d \n", err_count );    
    userlog.SetMode(true);	
    logfile.SetMode(true);
    
    return 0;
}



void ShtrihLoad::testCommand(string str)
{
    string clear_command_symbol = "$$$CLR";
    string no_tov_clear_command_symbol = "{NO_TOV}";
    string clear_fix_clear_command_symbol = "{FIX_S}";
    
    string add_quantity_command = "$$$ADD";
    string replace_quantity_command = "$$$RPL";
    
    int pos = str.find(add_quantity_command);
    if (pos != string::npos ) command_add_quantity = true; // replace = false;
    
    pos = str.find(clear_command_symbol);
    if ( pos == string::npos ) return;
    
    command_ware_clear = true;
    int tmp_pos = str.find( no_tov_clear_command_symbol, pos );
    if ( tmp_pos != string::npos ) command_ware_clear = false;
    
    tmp_pos = str.find(clear_fix_clear_command_symbol, pos);
    if ( tmp_pos != string::npos ) command_fix_clear = true;
}



void ShtrihLoad::executeCommand()
{
    if ( command_fix_clear ) {
	int res = pg_upload->DeleteFromTable(2);
	if ( res < 0 ) {
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось очистить список фиксированных скидок \r\n")));
	    logfile.PrintString(log_type, ERROR, "Couldn't clear fix discount list \n" ); 
	    return;
	}
    }
    if ( command_ware_clear ) {
	int res = pg_upload->DeleteAllWaresGroups();
	if ( res < 0 ) {
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не удалось очистить справочник товаров \r\n")));
	    logfile.PrintString(log_type, ERROR, "Couldn't clear ware table \n" ); 
	    return;
	}	
    }
    
    //cout << "Commands.size() = " << commands.size() << endl;
    
    for(vector<Command>::iterator it = commands.begin(); it < commands.end(); it++ ) {
	if ( (*it).execute(pg_upload, command_add_quantity) != 0 ) err_count++;
    }
	
}


/******************************* Gommand ***********************************/

Command::Command(string line, int string_num)
{
    line_str = line; 
    str_num = string_num; 
    //type = EMPTY;
}

void Command::print()
{
    cout << str_num << " " <<  line_str << endl;
}

void Command::process(string line)
{
    //cout << "process string - " << line << endl;
    parameters.clear();
    string param;
    int pos = 0;
    int pos_s = 0;
        
    pos = line.find(";", pos);
    while (pos != -1) {
	param = line.substr(pos_s, pos - pos_s);	
	param = DbCodec->fromUnicode(CP1251Codec->toUnicode(QString(param)));
	//cout << "param = " << param << endl;
	parameters.push_back(param);
	pos++;
	pos_s = pos;
	pos = line.find(";", pos);
    }
    
    if ( pos_s < line.length() ) {
	//cout << "pos_s = " << pos_s << " line.length() = " <<  line.length() << " line.length() - pos_s = " << (line.length()-pos_s) << endl;
	param = line.substr(pos_s, line.length() - pos_s - 1);
	//cout << " last param = " << param << endl;
	param = DbCodec->fromUnicode(CP1251Codec->toUnicode(QString(param)));
	parameters.push_back(param);
    }    
}

int Command::correctWareGroup()
{
    int error = 0;
    
    //cout << "parameters.size() = " << parameters.size() << endl;
    //cout << "parameters[16] = " << parameters[16] << endl;
    
    if ( parameters.size() < 17 ) error = -1;
    else if ( parameters[16] != "0" && parameters[16] != "1" ) error = -1;
    
    if ( parameters[0].empty() ) error = -2;
    if ( atoi(parameters[0].c_str()) <= 0 ) error = -2;
    
    if ( error == -1 ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Строка не соответсвует протоколу. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Incorrect string format for ware or group : %d \n", str_num ); 
	return -1;
    }
    
    if ( error == -2 ) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не верный идентификатор для товара или группы. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Incorrect id for group or ware %d \n", str_num );    
	return -1;
    }
    
    return 0;
}

int Command::correctBarcode()
{
    if (parameters.size() < 2) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Строка не соответствует протоколу. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Incorrect string format for barcode : %d \n", str_num ); 
	return -1;
    }
    
    if (parameters[0].empty()) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Не верный идентификатор товара для штрих-кода. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Incorrect ware id for barcode %d \n", str_num );    
	return -1;
    }
    return 0;
}

string Command::convertDouble(double value) {
    std::ostringstream o;
    if (!(o << value)) return "";
    return o.str();
}

void testByAppostroff(string& str) {
    
    int pos = str.find("'", 0);
    
    while ( pos != string::npos ) {
	str.insert(pos, "'");
	pos++;
	pos = str.find("'", pos + 1);
    }
}

int Command::wareLoad(PgUpload * db, bool add_quantity)
{
    //puts("ware load");
    string code = parameters[0];
    string barcode = parameters[1];
    
    string longtitle = parameters[2];
    testByAppostroff(longtitle);
    longtitle = "'" + longtitle + "'";
    
    string shortcut = parameters[3];
    testByAppostroff(shortcut);
    shortcut = "'" + shortcut + "'";
    
    string price = "0";
    if (!parameters[4].empty()) price = parameters[4];
    string quantity = "0";
    if ( !parameters[5].empty() ) quantity = parameters[5];
    string auto_scheme_id = "null";
    if ( !parameters[6].empty() ) auto_scheme_id = parameters[6];
    string flags = "438";
    if ( parameters[7] != "0" ) flags = "439";
    string min_price = "0";
    if ( !parameters[9].empty() ) {
	double price_double = atof(price.c_str());
	//cout << "price_double" << price_double << endl;
	double min_price_double =  price_double - price_double*(atof(parameters[9].c_str())/100);
	//cout << "parameters = " << atof(parameters[9].c_str()) << endl;
	//cout << "min_price_double = " << min_price_double << endl;
	if ( min_price_double > 0 ) min_price = convertDouble(min_price_double);
	//cout << "min price itogo = " << min_price << endl;
    }
    string best_before = "null";
    string coefficient = "1";
    string group_code = "null";
    if (!parameters[15].empty() && parameters[15] != "0" ) group_code = parameters[15];
    string tax_group_id = "null";
        
    //cout << "code = " << code << "barcode = " << barcode << "longtitle = " << longtitle << endl;
    //cout << "price = " << price << "quantity = " << quantity << "group_code = " << group_code << endl;
    int res = 0;
    if ( add_quantity ) {
	res = db->AddWare(code.c_str(), barcode.c_str(), longtitle.c_str(), shortcut.c_str(), price.c_str(), quantity.c_str(), auto_scheme_id.c_str(), flags.c_str(), min_price.c_str(), best_before.c_str(), coefficient.c_str(), group_code.c_str(), tax_group_id.c_str());
    } else {
	res = db->ReplaceWare(code.c_str(), barcode.c_str(), longtitle.c_str(), shortcut.c_str(), price.c_str(), quantity.c_str(), auto_scheme_id.c_str(), flags.c_str(), min_price.c_str(), best_before.c_str(), coefficient.c_str(), group_code.c_str(), tax_group_id.c_str(), false);
    }
    
    if (res < 0) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка при добавлении товара. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Ware add into base error string - %d \n", str_num );    
	return -1;
    }
    
    //puts("ware loaded");
    return 0;
}

int Command::groupLoad(PgUpload * db)
{
    //puts("group load");
    string code = parameters[0];
    
    string longtitle = parameters[2];
    testByAppostroff(longtitle);
    longtitle = "'" + longtitle + "'";
    
    string shortcut = parameters[3];
    testByAppostroff(shortcut);
    shortcut = "'" + shortcut + "'";
    
    string group_code = "null";
    if (!parameters[15].empty() && parameters[15] != "0" ) group_code = parameters[15];
    
    int res = db->ReplaceGroup(code.c_str(), longtitle.c_str(), shortcut.c_str(), group_code.c_str());
    
    if (res < 0) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка при добавлении группы. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Group add into base error string - %d \n", str_num );    
	return -1;
    }
    
    //puts("group loaded");
    return 0;
}

int Command::barcodeLoad(PgUpload * db)
{
   // puts("barcode load");
    string code = parameters[0];
    string barcode = parameters[1];
    string coeff = "1";
    if ( parameters.size() > 9 ) coeff = parameters[9];
    
    int res = db->Add_Barcodes(code.c_str(), barcode.c_str(), coeff.c_str());
    
    if (res < 0) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка при добавлении штрих-кода. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Barcode add into base error string - %d \n", str_num );    
	return -1;
    }
    
    //puts("barcode loaded");
    return 0;
}


int Command::correctFixDiscount()
{
    if (parameters.size() < 9) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Строка не соответствует протоколу. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Incorrect string format for barcode : %d \n", str_num ); 
	return -1;
    }
    return 0;
}

int Command::fixdiscountLoad(PgUpload * db)
{
    //puts("fixdiscount load");
    string code = "0";
    if ( !parameters[0].empty() ) code = parameters[0];
    string title = parameters[1];
    testByAppostroff(title);
    title = "'" + title + "'";
    string type = "0";
    if (parameters[2] == "2") type = "2";
    if (parameters[3] == "2") { if ( type == "0" )  type = "1"; else type = "3"; }
    string value = "0";
    if (!parameters[4].empty()) value = parameters[4];
    string shortcut = parameters[5];
    testByAppostroff(shortcut);
    shortcut = "'" + shortcut + "'";
    string card = "'" + parameters[6] + "'";
    string area = "'f'";
    if (parameters[7] == "1") area = "'t'";
    string show = "'t'";
    if ( parameters.size() > 9 ) if ( parameters[9] == "1" ) show = "'f'";
    
    
    int res = db->AddFixReduction(code.c_str(), title.c_str(), shortcut.c_str(), type.c_str(), value.c_str(), area.c_str(), card.c_str(), "''", show.c_str());
    
    if (res < 0) {
	userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка при добавлении фиксированной скидки. строка - %d \r\n")), str_num );
	logfile.PrintString(log_type, ERROR, "Fix reduction add into base error. string - %d \n", str_num );    
	return -1;
    }
    
    //puts("fix_discount loaded");
    return 0;    
}

int Command::execute(PgUpload *db, bool add_quantity)
{        
    if ( line_str[0] >= '0' && line_str[0] <= '9' ) {
	// ware or group
	//cout << "ware or group " << endl;
	//cout << "line_str = " << line_str << endl;
	
	process(line_str);	
	
	if ( correctWareGroup() != 0 ) return -1;
	//cout << " if 1 ware load, else group load == " << parameters[16] << endl;
	if ( parameters[16] == "1" ) {
	    return wareLoad(db, add_quantity);
	} else {
	    return groupLoad(db);
	}
	
	return 0;
    }
    
    if (line_str[0] == '#') {
	// barcode
	process(line_str.substr(1));
	if ( correctBarcode() != 0 ) return -1;
	return barcodeLoad(db);
    }
    
    if (line_str[0] == '^') {
	//fix_reduction
	process(line_str.substr(1));
	if ( correctFixDiscount() != 0 ) return -1;
	return fixdiscountLoad(db);
    }
    
    userlog.PrintString(user_log_type, WARN, WinCodec->fromUnicode(SystemCodec->toUnicode("Неизвестная команда строка %d \r\n")), str_num );
    logfile.PrintString(log_type, WARN, "Unknow command : %d %s \n", str_num, line_str.c_str() ); 
    return 0;
}


