/* 
 * File:   ResultSet.h
 * Author: shonda
 *
 * Created on 21 Январь 2010 г., 13:52
 */

#ifndef _ResultSet_H
#define	_ResultSet_H


#include <string>
#include <libpq-fe.h>
#include <vector>
#include <map>

using namespace std;

class ResultRecord;
class ResultSet;

class ResultSet {
public:
    ResultSet();
    ResultSet(PGresult * result);
    ~ResultSet();
    int getCountRecord() const;
    void clear();
    void setResult(PGresult * pg_result);

    string getValue(int i, int j) const;
    string getValue(int i, const string &field_name) const;

    ResultRecord getRecord(int i);

private:
    
    PGresult * records;
};


class ResultRecord {
public:
    ResultRecord(int num, ResultSet * rs);
    
    string getValue(int j) const;
    string getValue(const string &field_name) const;
    
private:
    int record_num;
    ResultSet * result_set;
};

#endif	/* _ResultSet_H */

