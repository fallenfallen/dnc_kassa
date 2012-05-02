#include "class_pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#ifndef SETUP_SAMPLEBARCODE_CLASS
#define SETUP_SAMPLEBARCODE_CLASS

struct TSampleBarcode {
    int id;
    char prefix[3];
    char sample[28];
    char multiplier[22];
};

class SetupSamplebarcode : public PGSQL
{
  public:
    SetupSamplebarcode();
    SetupSamplebarcode(const char * conf_path);
    SetupSamplebarcode(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path);
    ~SetupSamplebarcode();
   
    int GetSamples(const char* scheme_name, TSampleBarcode* sambar, int sam_count);
    int GetCountSamples(const char* scheme_name, int& sam_count);
    int AddNewSample(const char* scheme_name, TSampleBarcode* sambar);
    int SaveInScheme(const char* scheme_name, TSampleBarcode* sambar, int sam_count);
   
  private:
    int query_Insert(const char* scheme_name, char prefix[3], char sample[28], char multiplier[22] );
    int query_Update(TSampleBarcode* dis);
    int query_Delete(TSampleBarcode* dis);
    int query_DeleteSamples(const char*scheme_name);
};

#endif














