#ifndef SYSTEMCONFIGDIALOG_H
#define SYSTEMCONFIGDIALOG_H

#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>

#include "dialog.h"
#include "exception.h"
#include "blinklabel.h"
#include "serverclient.h"
#include "networkrequest.h"
#include "proxyserverconfigdialog.h"

class SystemConfigDialog : public Dialog
{
    Q_OBJECT

public:
    SystemConfigDialog();

private slots:
    void applyServerChanges();
    void applyDiscountSystemChanges();
    void systemProxyServer();
    void changeProxyServerSettings(bool use);
    void checkConnection();
    void connectionResult(bool connected);
    void restartServer();
    void restartResult(int error, const QString &description);

signals:
    void information(Information * information);
    void closeProgressDialog();

protected:
    void closeEvent(QCloseEvent * event);

private:
    void initInterface();
    void initConnections();
    bool addServerSettingsToDatabase();
    bool addDiscountSystemSettingsToDatabase();

    ServerClient * client;
    ProgressBar * progressBar;
    ProxyServerConfigDialog * proxyServerDialog;
    QTabWidget * tab;
    QLabel * lblServerAddress;
    QLineEdit * edtServerHost;
    QSpinBox * edtServerPort;
    QLineEdit * edtServerAttemptCount;
    QLineEdit * edtServerConnectionTimeout;
    QCheckBox * chkConnectionState;
    BlinkLabel * lblConnectionState;
    QPushButton * btnCheckConnection;
    QPushButton * btnRestartServer;
    QPushButton * btnApplyServerChanges;
    QLineEdit * edtMemberID;
    QLineEdit * edtSystemURL;
    QLineEdit * edtSystemIPv4Host;
    QLineEdit * edtSystemIPv6Host;
    QSpinBox * edtSystemPort;
    QLineEdit * edtSystemRequestTimeout;
    QLineEdit * edtSystemRetryCount;
    QLineEdit * edtSystemAttemptTimeout;
    QLineEdit * edtDiscountMobileSertify;
    QLabel * lblUsedProxyServer;
    QPushButton * btnSystemProxyServer;
    QPushButton * btnApplyDiscountSystemChanges;

};

#endif // SYSTEMCONFIGDIALOG_H
