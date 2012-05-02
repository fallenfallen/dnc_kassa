/* 
 * File:   MapperContainer.h
 * Author: shonda
 *
 * Created on 23 Июнь 2010 г., 14:34
 */

#ifndef _MAPPERCONTAINER_H
#define	_MAPPERCONTAINER_H

#include <string>
#include <map>

#include "AbstractMapper.h"

using namespace std;

class MapperContainer {
public:
    static void addMapper(const string& class_name, AbstractMapper * mapper);
    static map<string, AbstractMapper*> getAllMappers();

private:
    static map <string, AbstractMapper*> mappers;
};

#endif	/* _MAPPERCONTAINER_H */

