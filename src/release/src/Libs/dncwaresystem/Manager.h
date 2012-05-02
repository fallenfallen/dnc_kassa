#ifndef MANAGER_H
#define	MANAGER_H

#include <string>
#include "Object.h"

using namespace std;

class ManagerInfo {
public:
    int code; // = manager_id
    string long_name;
    string short_name;
    string barcode;
    string cardcode;
    ManagerInfo();
};


class Manager : public Object{
public:
    Manager();

    ManagerInfo getInfo();
    void setInfo(const ManagerInfo & manager_info);
    void copyInfo(Object * obj);
    
private:
    ManagerInfo info;
};

#endif	/* MANAGER_H */

