#include "class_pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#ifndef SETUP_DOC_CLASS
#define SETUP_DOC_CLASS

struct TDocument {
    int id;
    char name[200];
    char path[1000];
};

class SetupDocument : public PGSQL
{
  public:
    SetupDocument();
    SetupDocument(const char * conf_path);
    SetupDocument(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path);
    ~SetupDocument();
   
    int GetDocuments(const char* scheme_name, TDocument*& docs, int & count);
    int GetActiveDocuments(TDocument*& docs, int & count);
    int AddDocument(const char* scheme_name, const char* doc_name, const char* path);
    int DeleteDocuments(const char*scheme_name);
};

#endif














