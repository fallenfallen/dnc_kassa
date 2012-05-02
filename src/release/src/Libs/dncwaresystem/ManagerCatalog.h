#ifndef MANAGERCATALOG_H
#define	MANAGERCATALOG_H


#include <string>
#include "ObjectCatalog.h"
#include "Manager.h"

using  namespace std;

class ManagerCatalog {
public:
    
    void addManager(const ManagerInfo& manager_info);
    void clearCatalog();
private:
    ObjectCatalog manager_container;
};

#endif	/* MANAGERCATALOG_H */

