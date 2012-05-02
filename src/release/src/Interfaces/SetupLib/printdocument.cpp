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

#include "printdocument.h"

#include "const_conf.h"
#include "const.h"
#include "setup_doc_pgsql.h"
#include <qtextcodec.h>
#include <stdio.h>
#include <iostream>


/**********************PrintDocument class***************************/
PrintDocument::PrintDocument()  
{  
    name = QString("");    
    path = QString("");  
}

PrintDocument::PrintDocument(QString doc_name, QString doc_path) { name = doc_name; path = doc_path;  }

PrintDocument::~PrintDocument()   {  ;  }

QString PrintDocument::GetName()  {  return name;  }
void PrintDocument::SetName(QString doc_name){    name = doc_name;  }

QString PrintDocument::GetPath()  {  return path; }
void PrintDocument::SetPath(QString doc_path){    path = doc_path;   }

void PrintDocument::Edit(QString doc_name, QString doc_path) { name = doc_name; path = doc_path; }

/********************end PrintDocument class*****************************/


/********************Documents class*****************************/

/*------------------------------------------------*/
DocumentList::DocumentList()  
{  
    table = NULL; 
}
/*------------------------------------------------*/
DocumentList::DocumentList(QTable * t) 
{ 
    table = t;  
}
/*------------------------------------------------*/
DocumentList::~DocumentList()  
{  
    documents.clear(); 
    if ( table != NULL )   table->setNumRows(0);
}
/*------------------------------------------------*/
PrintDocument* DocumentList::GetCurrentDocument()  
{  
    if (table == NULL) return NULL;
    if ( (table->currentRow() < 0) || (table->currentRow() > documents.size()) ) return NULL;
    return &documents.at(table->currentRow());  
}
/*------------------------------------------------*/
PrintDocument DocumentList::GetDocumentAt(int index)  
{ 
    return documents.at(index);  
}
/*------------------------------------------------*/
void DocumentList::AddDocument(QString name, QString path) 
{
    if (table == NULL) return;
    PrintDocument pd(name, path);
    documents.push_back(pd);		
    table->setNumRows(documents.size());
    table->setText(documents.size() - 1, 0, name);
    table->setText(documents.size() - 1, 1, path);
}
/*------------------------------------------------*/
void DocumentList::AddDocument(PrintDocument pd) 
{ 
    if (table == NULL) return;
    documents.push_back(pd); 
    table->setNumRows(documents.size());
    table->setText(documents.size() - 1, 0, pd.GetName());
    table->setText(documents.size() - 1, 1, pd.GetPath());
}
/*------------------------------------------------*/
void DocumentList::EditDocument(QString name, QString path) 
{  
    if (table == NULL) return;
    if ( (table->currentRow() < 0) || (table->currentRow() > documents.size()) ) return;
    GetCurrentDocument()->Edit(name, path);  
    table->setText(table->currentRow(), 0, name);
    table->setText(table->currentRow(), 1, path);
}
/*------------------------------------------------*/	
void DocumentList::EditDocument( PrintDocument pd ) 
{
    if (table == NULL) return;
    if ( (table->currentRow() < 0) || (table->currentRow() > documents.size()) ) return;
    *GetCurrentDocument() = pd; 
    table->setText(table->currentRow(), 0, pd.GetName());
    table->setText(table->currentRow(), 1, pd.GetPath());    
}
/*------------------------------------------------*/
void DocumentList::RemoveDocument()  
{
    if (table == NULL) return;
    if ( (table->currentRow() < 0) || (table->currentRow() > documents.size()) ) return;
    documents.erase(documents.begin() + table->currentRow());
    table->removeRow(table->currentRow());   
}
/*------------------------------------------------*/	
int DocumentList::Save(QString scheme_name)
{
    //cout << "scheme_name = " << scheme_name << endl;
    
    SetupDocument doc_db(LINCASH_CONF_PATH);
    if ( doc_db.er_con != 0 ) return -1;
    
    QTextCodec* codec_db =  QTextCodec::codecForName(DBCODEC);
    QTextCodec * codec_sys =  QTextCodec::codecForName(SYSCODEC);
    
    char * scheme_name_str = (char*) calloc(2*scheme_name.length() + 1, sizeof(char));
    scheme_name = codec_db->fromUnicode(codec_sys->toUnicode(scheme_name));
    strcpy(scheme_name_str, scheme_name);
    
    int sum_err;
    //printf("scheme_name_str = %s \n", scheme_name_str);
    sum_err = doc_db.DeleteDocuments(scheme_name_str);
    //printf("sum_err -= %d \n", sum_err);
    if ( sum_err != 0 ) {
	free(scheme_name_str);
	return -1;
    }
    
    QString tmp1, tmp2;
    sum_err = 0;
    
    //printf("document_size = %d \n", documents.size());
    
    for ( vector<PrintDocument>::iterator it = documents.begin(); it < documents.end(); it++ ){ 
	//cout << "name = " << (*it).GetName() << " path = " << (*it).GetPath();
	char * name_str = (char*) calloc(2*(*it).GetName().length() + 1, sizeof(char));
	char * path_str = (char*) calloc(2*(*it).GetPath().length() + 1, sizeof(char) );
	tmp1 = codec_db->fromUnicode((*it).GetName());
	tmp2 = codec_db->fromUnicode((*it).GetPath());
	strcpy(name_str, tmp1);
	strcpy(path_str, tmp2);
	
	sum_err += doc_db.AddDocument(scheme_name_str, name_str, path_str);
	
	//printf("sum_err -= %d \n", sum_err);
	
	free(path_str);
	free(name_str);
    }
    
    free(scheme_name_str);
    
    if ( sum_err != 0 ) return -1;
    else return 0;
}

/*------------------------------------------------*/

int DocumentList::Load(QString scheme_name)
{
    documents.clear(); 
    if ( table != NULL )   table->setNumRows(0);
    
    SetupDocument doc_db(LINCASH_CONF_PATH);
    if ( doc_db.er_con != 0 ) return -1;
    
    QTextCodec* codec_db =  QTextCodec::codecForName(DBCODEC);
    QTextCodec * codec_sys =  QTextCodec::codecForName(SYSCODEC);
    
    //printf("scheme_name.length() = %d \n", scheme_name.length());
    char * scheme_name_str = (char*) calloc(2*scheme_name.length() + 1, sizeof(char));
    scheme_name = codec_db->fromUnicode(scheme_name);
    strcpy(scheme_name_str, scheme_name);
    //printf("scheme_name_str = %d \n", strlen(scheme_name_str));
    
    TDocument * docs;
    int count_doc;
    
    if ( doc_db.GetDocuments(scheme_name_str, docs, count_doc) != 0 ) {
	free(scheme_name_str);
	return -1;
    }
    
    //cout << "count = " << count_doc << endl;
    
    
    for ( int i = 0; i < count_doc; i++ )   {
	//cout << "name = " << docs[i].name << " path = " << docs[i].path << endl;
	AddDocument(codec_db->toUnicode(docs[i].name), codec_db->toUnicode(docs[i].path));
    }
    
    if ( count_doc > 0 )  free(docs);    
    free(scheme_name_str);
    
    return 0;
}
/*------------------------------------------------*/

/********************end DocumentList class*****************************/

