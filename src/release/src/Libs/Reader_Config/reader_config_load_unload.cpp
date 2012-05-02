/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#ifndef READ_CONF_LOAD_UNLOAD
#define READ_CONF_LOAD_UNLOAD


#include "reader_config_load_unload.h"

const int COUNT_PARAM = 7;

const string TEXT_PARAM[COUNT_PARAM] = {"[UPLOAD_FLAG]",
                        		"[UPLOAD_FILE]",
    		                        "[UNLOAD_FLAG]",
                		        "[UNLOAD_FILE]",
				        "[N_PC]",
					"[HU_MARK_TRANS]",
					"[PROTOCOL]" };

/*************************************************************************/
ReaderConf::ReaderConf() {
   //printf("create reader config config_path = %s \n", RC_LOADUNLOAD_PATH);
   
   ifstream conf;
   conf.open(RC_LOADUNLOAD_PATH, ifstream::in);
   
   int i = 0;
   string tmp;
   while (conf.good()) {
     getline(conf, tmp);
     //cout << tmp << endl;
     TakeParam(tmp);
     i++;
   }
   conf.close();
}
/*************************************************************************/
ReaderConf::ReaderConf(const char* aconfig_path) {
   //printf("create reader config config_path = %s \n", aconfig_path);

   ifstream conf;
   conf.open(aconfig_path, ifstream::in);
	
   string tmp;
   while (conf.good()) {
     getline(conf, tmp);
     
     //cout << tmp << endl;
     TakeParam(tmp);
   }
   conf.close();
     
   /*for( map<string, string>::iterator it = addresses.begin(); it != addresses.end(); it++) {
     cout << "addresses " << (*it).first << " = " << (*it).second << endl;
   }*/
       
   //puts("reader config was created");

}
/*************************************************************************/
void ReaderConf::TakeParam(string str){
     
	  //printf("TakeParam(%s) \n", str.c_str());
     int i = 0;
          
     if ( str.empty() ) return;


     while( (i < COUNT_PARAM) && ((int) str.find(TEXT_PARAM[i]) != 0) ) i++;

     if ( i < COUNT_PARAM ) {
          
          //cout << "str = <" << str << ">" << endl;
          int text_len = TEXT_PARAM[i].length();
          
          str = str.substr(text_len, str.length()-text_len);          
			 
			 size_t pos = str.find("[");
			 if ( pos != -1 ) {
			 	 TakeParam(str.substr(pos-1, str.length() - pos));
			 	 str = str.substr(0, pos); 
			 }
			 
			           
          //cout << "str = <" << str << ">" << endl;
          
          pos = str.find("/");
          
          if (pos != -1) str = str.substr( pos, str.length() - pos );
          
	       //cout << "" << "" << endl;
          
          //cout << "<" << str << ">" << endl;
          
          addresses[TEXT_PARAM[i]] =  str;
     } 

}
/*************************************************************************/
ReaderConf::~ReaderConf() {   
     //puts("destroy reader conf load unload :(");
}
/*************************************************************************/
int ReaderConf::getLen(const char* str) {
     string s = str;
     return addresses[str].length();
}
/*************************************************************************/
int ReaderConf::GetUploadFlagPath( char* upload_flag ) {
     //puts("GetUploadFilesPath");
     //cout << "upload_falg = " << addresses[TEXT_PARAM[0]] << endl;
     //cout << "upload_file = " << addresses[TEXT_PARAM[1]] << endl;
     if (addresses.find(TEXT_PARAM[0]) != addresses.end()) strcpy(upload_flag, addresses[TEXT_PARAM[0]].c_str());
     //puts("end of get files up");
     return 0;   }
/*************************************************************************/
int ReaderConf::GetUploadDataPath( char* upload_file ) {
     //puts("GetUploadFilesPath");
     //cout << "upload_falg = " << addresses[TEXT_PARAM[0]] << endl;
     //cout << "upload_file = " << addresses[TEXT_PARAM[1]] << endl;
     if (addresses.find(TEXT_PARAM[1]) != addresses.end()) strcpy(upload_file, addresses[TEXT_PARAM[1]].c_str());
     //puts("end of get files up");
     return 0;   }
/*************************************************************************/
int ReaderConf::GetUnloadFlagPath( char* unload_flag ) {
     //puts("GetUnloadFilesPath");
     if (addresses.find(TEXT_PARAM[2]) != addresses.end()) strcpy(unload_flag, addresses[TEXT_PARAM[2]].c_str());
     //puts("end of un get files");
     return 0;  
}
/*************************************************************************/
int ReaderConf::GetUnloadDataPath( char* unload_file ) {
     //puts("GetUnloadFilesPath");
     if (addresses.find(TEXT_PARAM[3]) != addresses.end()) strcpy(unload_file, addresses[TEXT_PARAM[3]].c_str());
     //puts("end of un get files");
     return 0;  
}
/*************************************************************************/
int ReaderConf::GetNPC(char* n_pc ) {
     //puts("GetUnloadFilesPath");
     if (addresses.find(TEXT_PARAM[4]) != addresses.end()) strcpy(n_pc, addresses[TEXT_PARAM[4]].c_str());
     //puts("end of un get files");
     return 0;  
}
/*************************************************************************/     
int ReaderConf::GetProtocol(char * protocol) {
	  if (addresses.find(TEXT_PARAM[6]) != addresses.end()) strcpy(protocol, addresses[TEXT_PARAM[6]].c_str());
     //puts("end of un get files");
     return 0;  
}     
/*************************************************************************/
int ReaderConf::GetHUParam(char* mark_trans)
{
     if (addresses.find(TEXT_PARAM[5]) != addresses.end()) strcpy(mark_trans, addresses[TEXT_PARAM[5]].c_str());
}
/*************************************************************************/


#endif













