#include "systemconfigdialog.h"
#include "databaseengine.h"
#include "translate.h"

#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>
#include <QMessageBox>

SystemConfigDialog::SystemConfigDialog() : Dialog()
{
    DatabaseLoader::init();

    client = new ServerClient;
    progressBar = new ProgressBar;
    proxyServerDialog = new ProxyServerConfigDialog;
    initInterface();
    initConnections();

    setWindowTitle(conv("ДЭНСИ: Настройка модуля дисконтной системы"));
    resize(580, 500);
}

void SystemConfigDialog::initInterface()
{
    tab = new QTabWidget;
    QHBoxLayout * hLayout = new QHBoxLayout;
    QFrame * frame = new QFrame;
    {
        QFormLayout * fLayout = new QFormLayout;
        fLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        QHBoxLayout * hLayout = new QHBoxLayout;
        lblServerAddress = new QLabel(conv("Текущий адрес сервера"));
        lblServerAddress->setFixedWidth(261);
        hLayout->addWidget(lblServerAddress);
        QString host = DatabaseLoader::serverHost();
        edtServerHost = new QLineEdit(host);
        hLayout->addWidget(edtServerHost);
        int port = DatabaseLoader::serverPort();
        edtServerPort = new QSpinBox;
        edtServerPort->setRange(0, 65535);
        edtServerPort->setValue(port);
        hLayout->addWidget(edtServerPort);
        fLayout->addRow(hLayout);
        int attemptCount = DatabaseLoader::serverAttemptCount();
        edtServerAttemptCount = new QLineEdit(QString::number(attemptCount));
        edtServerAttemptCount->setValidator(new QIntValidator(0, 100, edtServerAttemptCount));
        fLayout->addRow(conv("Количество попыток запуска сервера"), edtServerAttemptCount);
        int timeout = DatabaseLoader::serverConnectionTimeout();
        edtServerConnectionTimeout = new QLineEdit(QString::number(timeout));
        edtServerConnectionTimeout->setValidator(new QIntValidator(0, 3600, edtServerConnectionTimeout));
        fLayout->addRow(conv("Таймаут подключения (секунды)"), edtServerConnectionTimeout);

        QVBoxLayout * vLayout = new QVBoxLayout;
        vLayout->addLayout(fLayout);
        hLayout = new QHBoxLayout;
        chkConnectionState = new QCheckBox;
        chkConnectionState->setFixedSize(QSize(20, 20));
        chkConnectionState->setStyleSheet("QCheckBox::indicator:unchecked { image: url(:/images/unchecked) }"
                                          "QCheckBox::indicator:checked { image: url(:/images/checked) }");
        hLayout->addStretch();
        hLayout->addWidget(chkConnectionState);
        lblConnectionState = new BlinkLabel(conv("Не подключен"));
        lblConnectionState->setFixedWidth(120);
        lblConnectionState->setMargin(10);
        lblConnectionState->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        hLayout->addWidget(lblConnectionState);
        btnCheckConnection = new QPushButton(conv("Проверка связи"));
        btnCheckConnection->setFixedWidth(180);
        hLayout->addWidget(btnCheckConnection);
        btnRestartServer = new QPushButton(conv("Перезагрузка сервера"));
        btnRestartServer->setFixedWidth(180);
        btnRestartServer->setEnabled(false);
        hLayout->addWidget(btnRestartServer);
        vLayout->addLayout(hLayout);
        vLayout->addStretch();
        btnApplyServerChanges = new QPushButton(conv("Сохранить изменения"));
        btnApplyServerChanges->setFixedWidth(200);
        hLayout = new QHBoxLayout;
        hLayout->addStretch();
        hLayout->addWidget(btnApplyServerChanges);
        vLayout->addLayout(hLayout);
        frame->setLayout(vLayout);
    }
    tab->addTab(frame, conv("Настройки запуска сервера"));

    frame = new QFrame;
    {
        QFormLayout * fLayout = new QFormLayout;
        fLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        QVBoxLayout * vLayout = new QVBoxLayout;
        int memberID = DatabaseLoader::memberID();
        edtMemberID = new QLineEdit(QString::number(memberID));
        edtMemberID->setValidator(new QIntValidator(edtMemberID));
        fLayout->addRow(conv("Идентификатор Участника"), edtMemberID);
        QString URL = DatabaseLoader::systemURL();
        edtSystemURL = new QLineEdit(URL);
        fLayout->addRow(conv("Адрес сервера"), edtSystemURL);
        int port = DatabaseLoader::systemPort();
        edtSystemPort = new QSpinBox;
        edtSystemPort->setRange(0, 65535);
        edtSystemPort->setValue(port);
        edtSystemPort->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        fLayout->addRow(conv("Порт сервера"), edtSystemPort);
        QString IPv4Host = DatabaseLoader::systemIPv4Host();
        edtSystemIPv4Host = new QLineEdit(IPv4Host);
        fLayout->addRow(conv("Адрес сервера по протоколу IPv4"), edtSystemIPv4Host);
        QString IPv6Host = DatabaseLoader::systemIPv6Host();
        edtSystemIPv6Host = new QLineEdit(IPv6Host);
        fLayout->addRow(conv("Адрес сервера по протоколу IPv6"), edtSystemIPv6Host);
        int requestTimeout = DatabaseLoader::systemRequestTimeout();
        edtSystemRequestTimeout = new QLineEdit(QString::number(requestTimeout));
        edtSystemRequestTimeout->setValidator(new QIntValidator(0, 3600, edtSystemRequestTimeout));
        fLayout->addRow(conv("Время ожидания ответа (секунды)"), edtSystemRequestTimeout);
        int retryCount = DatabaseLoader::systemRetryCount();
        edtSystemRetryCount = new QLineEdit(QString::number(retryCount));
        edtSystemRetryCount->setValidator(new QIntValidator(0, 100, edtSystemRetryCount));
        fLayout->addRow(conv("Количество попыток установить связь"), edtSystemRetryCount);
        int attemptTimeout = DatabaseLoader::systemAttemptTimeout();
        edtSystemAttemptTimeout = new QLineEdit(QString::number(attemptTimeout));
        edtSystemAttemptTimeout->setValidator(new QIntValidator(0, 100, edtSystemAttemptTimeout));
        fLayout->addRow(conv("Интервал между попытками (секунды)"), edtSystemAttemptTimeout);
        QString sertify = DatabaseLoader::discountMobileSertify();
        edtDiscountMobileSertify = new QLineEdit(sertify);
        fLayout->addRow(conv("Сертификат Discount Mobile"), edtDiscountMobileSertify);
        QHBoxLayout * hLayout = new QHBoxLayout;
        bool usedProxy = DatabaseLoader::useProxy();
        lblUsedProxyServer = new QLabel(usedProxy ? conv("Используется прокси-сервер") :
                                        conv("Прокси-сервер не используется"));
        btnSystemProxyServer = new QPushButton(conv("Настройка прокси"));
        btnSystemProxyServer->setFixedWidth(150);
        hLayout->addStretch();
        hLayout->addWidget(lblUsedProxyServer);
        hLayout->addWidget(btnSystemProxyServer);
        vLayout->addLayout(fLayout);
        vLayout->addLayout(hLayout);
        btnApplyDiscountSystemChanges = new QPushButton(conv("Сохранить изменения"));
        btnApplyDiscountSystemChanges->setFixedWidth(200);
        vLayout->addStretch();
        hLayout = new QHBoxLayout;
        hLayout->addStretch();
        hLayout->addWidget(btnApplyDiscountSystemChanges);
        vLayout->addLayout(hLayout);
        frame->setLayout(vLayout);
    }
    tab->addTab(frame, conv("Подключение к серверу дисконтной системы"));
    hLayout->addWidget(tab);
    setLayout(hLayout);
}

void SystemConfigDialog::initConnections()
{
    connect(btnApplyServerChanges, SIGNAL(clicked()), SLOT(applyServerChanges()));
    connect(btnApplyDiscountSystemChanges, SIGNAL(clicked()), SLOT(applyDiscountSystemChanges()));
    connect(btnRestartServer, SIGNAL(clicked()), SLOT(restartServer()));
    connect(btnCheckConnection, SIGNAL(clicked()), SLOT(checkConnection()));
    connect(btnSystemProxyServer, SIGNAL(clicked()), SLOT(systemProxyServer()));
    connect(proxyServerDialog, SIGNAL(information(Information *)), SIGNAL(information(Information *)));
    connect(proxyServerDialog, SIGNAL(changeProxyServerSettings(bool)), SLOT(changeProxyServerSettings(bool)));
    connect(client, SIGNAL(information(Information *)), SIGNAL(information(Information *)));
    connect(client, SIGNAL(connectionResult(bool)), SLOT(connectionResult(bool)));
    connect(client, SIGNAL(restartResult(int, QString)), progressBar, SLOT(close()));
    connect(client, SIGNAL(restartResult(int, QString)), SLOT(restartResult(int, QString)));
    connect(client, SIGNAL(parseError()), progressBar, SLOT(close()));
}

void SystemConfigDialog::closeEvent(QCloseEvent * event)
{
    progressBar->close();
}

void SystemConfigDialog::applyDiscountSystemChanges()
{
    bool result = addDiscountSystemSettingsToDatabase();
    QString text = result ? conv("Записи успешно добавлены в базу данных.") :
                   conv("Во время добавления данных в базу произошла ошибка.");
    QMessageBox::information(this, conv("ДЭНСИ: Настройка модуля дисконтной системы"), text);

    emit information(new Information(text));
}

bool SystemConfigDialog::addServerSettingsToDatabase()
{
    bool result = true;
    QString host = edtServerHost->text();
    result &= DatabaseLoader::setServerHost(host);
    int port = edtServerPort->text().toInt();
    result &= DatabaseLoader::setServerPort(port);
    int attemptCount = edtServerAttemptCount->text().toInt();
    result &= DatabaseLoader::setServerAttemptCount(attemptCount);
    int timeout = edtServerConnectionTimeout->text().toInt();
    result &= DatabaseLoader::setServerConnectionTimeout(timeout);

    return result;
}

bool SystemConfigDialog::addDiscountSystemSettingsToDatabase()
{
    bool result = true;
    int member = edtMemberID->text().toInt();
    result &= DatabaseLoader::setMemberID(member);

    QString url = edtSystemURL->text();
//    Почему? При подлюченном сервере программа вываливается при попытке знести url в базу
    result &= DatabaseLoader::setURL(url);

    QString IPv4Host = edtSystemIPv4Host->text();
    result &= DatabaseLoader::setSystemIPv4Host(IPv4Host);
    QString IPv6Host = edtSystemIPv6Host->text();
    result &= DatabaseLoader::setSystemIPv6Host(IPv6Host);
    int port = edtSystemPort->text().toInt();
    result &= DatabaseLoader::setSystemPort(port);
    int requestTimeout = edtSystemRequestTimeout->text().toInt();
    result &= DatabaseLoader::setSystemRequestTimeout(requestTimeout);
    int retryCount = edtSystemRetryCount->text().toInt();
    result &= DatabaseLoader::setSystemRetryCount(retryCount);
    int attemptTimeout = edtSystemAttemptTimeout->text().toInt();
    result &= DatabaseLoader::setSystemAttemptTimeout(attemptTimeout);
    QString sertify = edtDiscountMobileSertify->text();
    result &= DatabaseLoader::setDiscountMobileSertify(sertify);

    return result;
}

void SystemConfigDialog::applyServerChanges()
{
    bool result = addServerSettingsToDatabase();
    QString text = result ? conv("Записи успешно добавлены в базу данных.") :
                   conv("Во время добавления данных в базу произошла ошибка.");
    QMessageBox::information(this, conv("ДЭНСИ: Настройка модуля дисконтной системы"), text);

    emit information(new Information(text));
}

void SystemConfigDialog::systemProxyServer()
{
    proxyServerDialog->updateProxyConfig();
    proxyServerDialog->show();
}

void SystemConfigDialog::changeProxyServerSettings(bool use)
{
    lblUsedProxyServer->setText(use ? conv("Используется прокси-сервер") :
                                conv("Прокси-сервер не используется"));
}

void SystemConfigDialog::checkConnection()
{
    QString host = edtServerHost->text();
    int port = edtServerPort->text().toInt();
    client->checkServerConnection(host, port);
}

void SystemConfigDialog::connectionResult(bool connected)
{
    int port = edtServerPort->text().toInt();
    QString host = edtServerHost->text();

    btnRestartServer->setEnabled(connected);
    btnCheckConnection->setEnabled(!connected);
    chkConnectionState->setChecked(connected);
    lblConnectionState->stop();
    if (connected)
    {
        emit information(new Information(conv("Сервер, работающий на адресе %1:%2, подключен.").arg(host).arg(port)));
        lblServerAddress->setText(conv("Новый адрес сервера"));
        lblConnectionState->setText(conv("Подключен"));
    }
    else
    {
        progressBar->close();
        lblServerAddress->setText(conv("Текущий адрес сервера"));
        lblConnectionState->setText(conv("Не подключен"));
        lblConnectionState->start(1000, 8);
    }
}

void SystemConfigDialog::restartServer()
{
    if (addServerSettingsToDatabase() &&
        addDiscountSystemSettingsToDatabase())
    {
        progressBar->show();
        progressBar->start();
        client->restartServer();
    }
}

void SystemConfigDialog::restartResult(int error, const QString &description)
{
    QString text;
    if (error == 0 && description.isEmpty())
        text = conv("Сервер успешно перезагружен");
    else
        text = conv("Во время перезагрузки произошла ошибка %1: %2").arg(error).arg(description);

    QMessageBox::information(this, conv("ДЭНСИ: Настройка модуля дисконтной системы"), text);

    emit information(new Information(text));
}
