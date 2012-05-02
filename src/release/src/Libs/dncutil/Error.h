/* 
 * File:   Error.h
 * Author: shonda
 *
 * Created on 24 Август 2010 г., 15:16
 */

#ifndef ERROR_H
#define	ERROR_H

#include <string>

using namespace std;

const int ERROR_OBJECT_NOT_FOUND = 209;

enum ErrorType {MSG_INFO, MSG_ERROR, MSG_WARNING, MSG_FATAL};

class Error {
public:
    Error(const string & err_text, ErrorType err_type = MSG_INFO);
    Error(int err_code, const string & err_user_text, const string & err_log_text, ErrorType err_type = MSG_ERROR);
    Error(int err_code, const string & err_text, ErrorType err_type = MSG_ERROR);

    string  getErrorLogText() const;
    string getErrorUserText() const;
    ErrorType getErrorType() const;
    int getErrorCode() const;
    
private:
    string user_text;
    string log_text;
    ErrorType type;
    int code;
};

#endif	/* ERROR_H */

