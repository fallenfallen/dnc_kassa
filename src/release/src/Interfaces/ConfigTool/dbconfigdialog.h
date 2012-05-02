#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QNetworkProxy>

#include "dialog.h"
#include "exception.h"
#include "blinklabel.h"

class DBConfigDialog : public Dialog
{
    Q_OBJECT

public:
    DBConfigDialog();

private slots:
    void apply();
    void checkConnection();

signals:
    void databaseConnected();
    void information(Information * information);

private:
    void initInterface();
    void initConnections();
    bool checkDatabaseConfig(const QString &name, const QString &host, int port,
                             const QString &user, QString &password);

    QLineEdit * edtDatabaseHost;
    QSpinBox * edtDatabasePort;
    QLineEdit * edtDatabaseName;
    QLineEdit * edtDatabaseUser;
    QLineEdit * edtDatabasePassword;
    BlinkLabel * lblConnectionState;
    QCheckBox * chkConnectionState;
    QPushButton * btnCheckConnection;
    QPushButton * btnApply;
    QPushButton * btnExit;

};

#endif // DBCONFIGDIALOG_H
