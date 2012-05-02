/* 
 * File:   ReaderConfig.h
 * Author: shonda
 *
 * Created on 15 Январь 2010 г., 17:39
 */

#ifndef _CONFIGREADER_H
#define	_CONFIGREADER_H

#include <map>
#include <string>

using namespace std;

class ConfigReader {

public:
    void loadFromFile(const char* path);
    void saveToFile(const char * path);

    string getParamValue(const char* param_name);
    void setParamValue(const char * param_name, const char * param_value);
    
private:
    map<string, string> parameters;
};

#endif	/* _CONFIGREADER_H */

