#ifndef DISCSCHEME_H
#define	DISCSCHEME_H

#include <string>
#include "Object.h"


using namespace std;

class DiscSchemeInfo {
public:
    int code;
    string short_name;
    string long_name;
    //-----(+)-----Zuskin-----14/12/2011-----
    bool is_default;
    //---------------------------------------
    DiscSchemeInfo();
};


class DiscScheme : public Object {
public:
    DiscScheme();

    void setInfo(const DiscSchemeInfo& dsc_info);
    DiscSchemeInfo getInfo();

    void copyInfo(Object * obj);
    Object* createCopyObject();

    int getOID() const;
    string getLongTitle() const;
    string getShortcut() const;
    //-----(+)-----Zuskin-----14/12/2011-----
    bool getIsDefault() const;
    //---------------------------------------
    void setCode(const int code);
    void setLongTitle(const string & dsc_lttl);
    void setShortcut(const string & dsc_scut);
    //-----(+)-----Zuskin-----14/12/2011-----
    void setIsDefault(const bool & dsc_isdfl);
    //---------------------------------------
    
private:
    DiscSchemeInfo info;
};

#endif	/* DISCSCHEME_H */

