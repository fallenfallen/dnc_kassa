#ifndef DISCRULE_H
#define	DISCRULE_H

#include <string>
#include "Object.h"


using namespace std;

class DiscRuleInfo {
public:
    int code;
    int scheme;
    string short_name;
    string long_name;
    double value;
    int type;
    string bdate;
    string edate;
    string btime;
    string etime;
    int weekdays;
    double sumcheck;
    double accumulation;

    DiscRuleInfo();
};


class DiscRule : public Object {
public:
    DiscRule();

    void setInfo(const DiscRuleInfo& dsc_info);
    DiscRuleInfo getInfo();

    void copyInfo(Object * obj);
    Object* createCopyObject();

    int getOID() const;
    int getSchemeId() const;
    string getLongTitle() const;
    string getShortcut() const;
    double getValue() const;
    int getType() const;
    string getBDate() const;
    string getEDate() const;
    string getBTime() const;
    string getETime() const;
    int getWeekdays() const;
    double getSumCheck() const;
    double getAccumulation() const;
        
    void setCode(const int code);    
    void setSchemeId(const int schemeid);
    void setLongTitle(const string & dsc_lttl);
    void setShortcut(const string & dsc_scut);
    void setValue(const double val);
    void setType(const int type);
    void setBDate(const string & bdate);
    void setEDate(const string & edate);
    void setBTime(const string & btime);
    void setETime(const string & etime);
    void setWeekdays(const int wdays);
    void setSumCheck(const double sum);
    void setAccumulation(const double acc);
        
private:
    DiscRuleInfo info;
};

#endif	/* DISCSRULE_H */

