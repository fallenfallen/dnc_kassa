/* 
 * File:   DncCommand.h
 * Author: shonda
 *
 * Created on 30 Ноябрь 2010 г., 14:27
 */

#ifndef DNCCOMMAND_H
#define	DNCCOMMAND_H

#include <string>
#include <vector>
#include "WareSystem.h"
#include "Error.h"
#include "upload_const.h"
#include "debug_level_log.h"
#include <qtextcodec.h>


extern DebugLevelLog userlog;
extern DebugLevelLog logfile;

extern QTextCodec * WinCodec;
extern QTextCodec * SystemCodec;


using namespace std;

class DncCommand {
public:
    DncCommand();
    static DncCommand * findCommand(const string &command_name);
    static DncCommand * getDefaulCommand();

    int execute(const string & line);
    virtual bool noEnd();

protected:
    string name;
    vector<string> params;
    virtual void executeCommand() = 0;
    void addBarcode(Barcode * barcode);

    static void catchError(const Error & err);

private:
    void selectParam(const string &line);
};


class DncCommandEnd : public DncCommand {
public:
    bool noEnd();
    void executeCommand();
};


class DncCommandAddFeaturesSets : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddFeatures : public DncCommand {
    void executeCommand();
};

class DncCommandAddFeaturesValues : public DncCommand {
    void executeCommand();
};

class DncCommandAddWares : public DncCommand {
public:
    DncCommandAddWares(bool replace_quantity = true);
private:
    bool replace_quantity;
    void executeCommand();
    void addBarcodes();
};


class DncCommandAddBarcodes : public DncCommand {
    void executeCommand();
};


class DncCommandAddWareAdditions : public DncCommand {
public :
    DncCommandAddWareAdditions(bool replace_quantity);
private:
    bool replace_quantity;
    void executeCommand();
    void addAdditionBarcode();
    void addSelectedValue(WareAddition * addition);
};


class DncCommandAddWareGroup : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddFixedDiscount : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddSaleRestrictionByTime : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddProfiles : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddUsers : public DncCommand {
private:
    void executeCommand();
};

//-----(+)-----Zuskin-----18/07/2011-----
class DncCommandAddDiscSchemes : public DncCommand {
private:
    void executeCommand();
};

class DncCommandAddDiscConditions : public DncCommand {
private:
    void executeCommand();
};
//---------------------------------------
//-----(+)-----Zuskin-----21/11/2011-----
class DncCommandAddManagers : public DncCommand {
private:
    void executeCommand();
};
//---------------------------------------
//-----(+)-----Zuskin-----21/11/2011-----
class DncCommandAddTaxGroups : public DncCommand {
private:
    void executeCommand();
};
//---------------------------------------
//-----(+)-----Zuskin-----11/01/2012-----
class DncCommandAddTaxRates : public DncCommand {
private:
    void executeCommand();
};
//---------------------------------------

/**************************************************************************/

class DncCommandDeleteAdditions : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandDeleteWares : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandDeleteFeatures : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandClearWareSystem : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandDeleteFixdiscount : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandDeleteProfiles : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};


class DncCommandDeleteUsers : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};

//-----(+)-----Zuskin-----19/07/2011-----
class DncCommandDeleteDiscSchemes : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};
//---------------------------------------
//-----(+)-----Zuskin-----21/11/2011-----
class DncCommandDeleteManagers : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};
//---------------------------------------

//-----(+)-----Zuskin-----12/01/2012-----
class DncCommandDeleteTaxes : public DncCommand {
public:
    static void exec();
private:
    void executeCommand();
};
//---------------------------------------

/**************************************************************************/

void maskApostroff(string& str);
void replace164(string& str);

#endif	/* DNCCOMMAND_H */




