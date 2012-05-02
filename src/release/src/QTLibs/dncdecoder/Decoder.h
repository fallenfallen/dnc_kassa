/* 
 * File:   Decoder.h
 * Author: shonda
 *
 * Created on 25 Январь 2010 г., 13:31
 */

#ifndef _DECODER_H
#define	_DECODER_H


#include <qstring.h>
#include <qtextcodec.h>
#include <string>

using namespace std;

QString toForm(const char* str);
QString toForm(const string & str);
QString toForm(const QString & str);

QString toForm(int value);
QString toForm(double value, int precision = 2);

QString fromForm(const QString & str);

string toWinFile(const char* str);
string toWinFile(const string & str);

string fromWinFile(const string &str);

#endif	/* _DECODER_H */

