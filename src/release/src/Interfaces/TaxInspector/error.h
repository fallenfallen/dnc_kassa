#ifndef ERROR_H
#define ERROR_H

#include <QObject>

class Model;

class Error : public QObject
{
    Q_OBJECT

public:
    Error(Model * model);
    ~Error();

    int analizeError(int res, const QString &funcName = QString());

signals:
    void requestContinuePrint(int * request);

private:
    Model * model;

    bool errorOutOfPaper(int result);
    bool errorCancelCheck(int result);

    int repeatBreakOperation();

};

#endif // ERROR_H
