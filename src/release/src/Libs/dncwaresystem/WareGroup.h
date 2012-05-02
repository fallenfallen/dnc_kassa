/* 
 * File:   WareGroup.h
 * Author: shonda
 *
 * Created on 11 Январь 2011 г., 16:13
 */

#ifndef WAREGROUP_H
#define	WAREGROUP_H

#include "Object.h"
#include <string>

using namespace std;

class WareGroup : public Object {
public:
    WareGroup();

    string getTitle() const;
    string getText() const;
    string getCode() const;

    int getParentId() const;
    WareGroup* getParentGroup();

    void setTitle(const string & gtitle);
    void setText(const string & gtext);
    void setCode(const string & gcode);
    //---(+)---Zuskin---20/02/2012---
    void setGroupId(int gid);
    int getGroupId() const;
    //-------------------------------

    void setParent(WareGroup* parent_group);
    void assignParent(int parent_group_id);

    void copyInfo(Object * obj);
    Object* createCopyObject();

    void clear();
    
private:
    string title;
    string text;
    string code;
    //---(+)---Zuskin---20/02/2012---
    int group_id;
    //-------------------------------
    WareGroup* parent;
};

#endif	/* WAREGROUP_H */

