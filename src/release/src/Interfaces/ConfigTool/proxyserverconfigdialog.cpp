#include "proxyserverconfigdialog.h"
#include "databaseengine.h"
#include "translate.h"

#include <QCloseEvent>
#include <QFormLayout>
#include <QMessageBox>
#include <QPlastiqueStyle>

ProxyServerConfigDialog::ProxyServerConfigDialog() : Dialog()
{
    DatabaseLoader::init();
    initInterface();
    initConnections();

    progressBar = new ProgressBar;
    request = new NetworkRequest;
    connect(this, SIGNAL(proxyRequest(int, QString, int, QString, QString)),
            request, SLOT(proxyRequest(int, QString, int, QString, QString)));
    connect(request, SIGNAL(requestResult(int)), SLOT(requestResult(int)));

    setWindowTitle(conv("Настройка прокси-сервера"));
    resize(400, 250);
}

ProxyServerConfigDialog::~ProxyServerConfigDialog()
{
    delete request;
}

void ProxyServerConfigDialog::closeEvent(QCloseEvent * event)
{
    progressBar->close();
}

void ProxyServerConfigDialog::initInterface()
{
    grpProxyServer = new QGroupBox(conv("Использовать прокси-сервер"));
    grpProxyServer->setCheckable(true);
    bool used = DatabaseLoader::useProxy();
    grpProxyServer->setChecked(used);
    grpProxyServer->setStyle(new QPlastiqueStyle);
    {
        QFormLayout * fLayout = new QFormLayout;
        int type, port;
        QString host, user, password;
        DatabaseLoader::proxySettings(type, host, port, user, password);
        proxyMap.insert("SOCKS5", QNetworkProxy::Socks5Proxy);
        proxyMap.insert("HTTP", QNetworkProxy::HttpProxy);
        proxyMap.insert("HTTP Caching", QNetworkProxy::HttpCachingProxy);
        cmbProxyServerType = new QComboBox;
        cmbProxyServerType->addItems(proxyMap.keys());
        if (used)
            cmbProxyServerType->setCurrentIndex(cmbProxyServerType->findText(
                    proxyMap.key((QNetworkProxy::ProxyType)type)));
        cmbProxyServerType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        fLayout->addRow(conv("Тип сервера"), cmbProxyServerType);
        QHBoxLayout * hLayout = new QHBoxLayout;
        edtProxyServerHost = new QLineEdit(host);
        hLayout->addWidget(edtProxyServerHost);
        edtProxyServerPort = new QSpinBox;
        edtProxyServerPort->setRange(0, 65535);
        edtProxyServerPort->setValue(port);
        hLayout->addWidget(edtProxyServerPort);
        fLayout->addRow(conv("Адрес"), hLayout);
        edtProxyServerUser = new QLineEdit(user);
        fLayout->addRow(conv("Пользователь"), edtProxyServerUser);
        edtProxyServerPassword = new QLineEdit(password);
        fLayout->addRow(conv("Пароль"), edtProxyServerPassword);
        edtProxyServerPassword->setEchoMode(QLineEdit::Password);
        grpProxyServer->setLayout(fLayout);
    }
    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->addWidget(grpProxyServer);
    btnApply = new QPushButton(conv("Применить"));
    btnApply->setFixedWidth(100);
    btnExit = new QPushButton(conv("Отмена"));
    btnExit->setFixedWidth(100);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnApply);
    hLayout->addWidget(btnExit);

    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void ProxyServerConfigDialog::initConnections()
{
    connect(btnApply, SIGNAL(clicked()), SLOT(apply()));
    connect(btnExit, SIGNAL(clicked()), SLOT(close()));
}

void ProxyServerConfigDialog::updateProxyConfig()
{
    bool used = DatabaseLoader::useProxy();
    grpProxyServer->setChecked(used);
    int type, port;
    QString host, user, password;
    DatabaseLoader::proxySettings(type, host, port, user, password);
    if (used)
        cmbProxyServerType->setCurrentIndex(cmbProxyServerType->findText(
                proxyMap.key((QNetworkProxy::ProxyType)type)));
    edtProxyServerHost->setText(host);
    edtProxyServerPort->setValue(port);
    edtProxyServerUser->setText(user);
    edtProxyServerPassword->setText(password);
}

void ProxyServerConfigDialog::apply()
{
    bool used = grpProxyServer->isChecked();
    int type = proxyMap.value(cmbProxyServerType->currentText());
    QString host = edtProxyServerHost->text();
    int port = edtProxyServerPort->text().toInt();
    QString user = edtProxyServerUser->text();
    QString password = edtProxyServerPassword->text();

    QRegExp rexp("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                 "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                 "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                 "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    if (used && type != 0 && !host.isEmpty() && rexp.exactMatch(host) && port != 0)
    {
        progressBar->show();
        progressBar->start();
        emit proxyRequest(type, host, port, user, password);
    }
    else
        requestResult(!used);
}

void ProxyServerConfigDialog::requestResult(int result)
{
    QString text;

    progressBar->close();

    if (result)
    {
        bool used = grpProxyServer->isChecked();
        int type = proxyMap.value(cmbProxyServerType->currentText());
        QString host = edtProxyServerHost->text();
        int port = edtProxyServerPort->text().toInt();
        QString user = edtProxyServerUser->text();
        QString password = edtProxyServerPassword->text();

        bool result = DatabaseLoader::setProxySettings(used, type, host, port, user, password);
        emit changeProxyServerSettings(used);

        text = result ? conv("Записи успешно добавлены в базу данных.") :
               conv("Во время добавления данных в базу произошла ошибка.");
    }
    else
        text = conv("Ошибка настроек proxy-сервера");

    QMessageBox::information(this, conv("ДЭНСИ: Настройка модуля дисконтной системы"), text);
    emit information(new Information(text));
}
