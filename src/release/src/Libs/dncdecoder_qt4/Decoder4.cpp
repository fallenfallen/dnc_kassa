/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "Decoder4.h"
#include <QTextCodec>
#include <iostream>
#include <bits/basic_string.h>

const char * SYSTEM_CODEC = "UTF-8";
const char * WIN_CODEC = "CP1251";

const QTextCodec * system_codec = QTextCodec::codecForName(SYSTEM_CODEC);
const QTextCodec * win_codec = QTextCodec::codecForName(WIN_CODEC);

QString toForm(const char * str)  {
    return  system_codec->toUnicode(str);
}

QString toForm(const string & str) {
    return system_codec->toUnicode(str.c_str());
}

QString toForm(const QString & str) {
    return  system_codec->toUnicode(str.toAscii());
}

QString toForm(int value) {
    return QString::number(value);
}

QString toForm(double value, int precision) {
    return QString::number(value, 'f', precision);
}

QString fromForm(const QString& str)  {
    return system_codec->fromUnicode(str);
}


string toWinFile(const char* str) {
    QString result = win_codec->fromUnicode(system_codec->toUnicode(str));
    return result.toStdString();
}

string toWinFile(const string & str) {
    return toWinFile(str.c_str());
}

string fromWinFile(const string &str) {
    
    QString result = system_codec->fromUnicode(win_codec->toUnicode(str.c_str()));
    
    return result.toStdString();
}