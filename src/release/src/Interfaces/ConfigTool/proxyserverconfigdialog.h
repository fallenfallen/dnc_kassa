#ifndef PROXYSERVERCONFIGDIALOG_H
#define PROXYSERVERCONFIGDIALOG_H

#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QNetworkProxy>

#include "dialog.h"
#include "exception.h"
#include "networkrequest.h"

class ProxyServerConfigDialog : public Dialog
{
    Q_OBJECT

public:
    ProxyServerConfigDialog();
    ~ProxyServerConfigDialog();

    void updateProxyConfig();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void apply();
    void requestResult(int result);

signals:
    void information(Information * information);
    void changeProxyServerSettings(bool use);
    void proxyRequest(int type, const QString &host, int port,
                      const QString &user, const QString &password);

private:
    void initInterface();
    void initConnections();
    void information();

    ProgressBar * progressBar;
    NetworkRequest * request;
    QMap<QString, QNetworkProxy::ProxyType> proxyMap;
    QGroupBox * grpProxyServer;
    QComboBox * cmbProxyServerType;
    QLineEdit * edtProxyServerHost;
    QSpinBox * edtProxyServerPort;
    QLineEdit * edtProxyServerUser;
    QLineEdit * edtProxyServerPassword;
    QPushButton * btnApply;
    QPushButton * btnExit;

};

#endif // PROXYSERVERCONFIGDIALOG_H
