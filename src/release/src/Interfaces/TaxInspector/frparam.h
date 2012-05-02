#ifndef FRPARAM_H
#define FRPARAM_H

struct FRParam
{
    char serialNumber[80];
    char ROMVersion[80];
    char description[80];
    int day;
    int mounth;
    int year;
    int hour;
    int minute;
    int second;
    bool fiscal;
    bool sessionOpened;
    int protocol;
    bool license;
    int mode;
    int advancedMode;
};

#endif // FRPARAM_H
