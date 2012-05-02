#include "dbconfigdialog.h"
#include "databaseengine.h"
#include "translate.h"
#include "settings.h"

#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QPlastiqueStyle>

DBConfigDialog::DBConfigDialog() : Dialog()
{
    initInterface();
    initConnections();

    setWindowTitle(conv("Настройка подключения к базе данных"));
    resize(400, 250);
}

void DBConfigDialog::initInterface()
{
    QGroupBox * group = new QGroupBox(conv("Параметры"));
    group->setStyle(new QPlastiqueStyle);
    {
        int port;
        QString name, host;
        QString user, password;

        if (!Settings::readConfig(name, host, port, user, password))
            emit information(new Exception(conv("Ошибка чтения настроек подключения к базе"), 103));

        QFormLayout * fLayout =  new QFormLayout;
        edtDatabaseName = new QLineEdit(name);
        fLayout->addRow(conv("Название"), edtDatabaseName);
        edtDatabaseHost = new QLineEdit(host);
        QHBoxLayout * hLayout = new QHBoxLayout;
        hLayout->addWidget(edtDatabaseHost);
        hLayout->addWidget(new QLabel(conv("Порт")));
        edtDatabasePort = new QSpinBox;
        edtDatabasePort->setRange(1, 65535);
        edtDatabasePort->setValue(port);
        hLayout->addWidget(edtDatabasePort);
        fLayout->addRow(conv("Адрес"), hLayout);
        edtDatabaseUser = new QLineEdit(user);
        fLayout->addRow(conv("Пользователь"), edtDatabaseUser);
        edtDatabasePassword = new QLineEdit(password);
        edtDatabasePassword->setEchoMode(QLineEdit::Password);
        fLayout->addRow(conv("Пароль"), edtDatabasePassword);
        hLayout = new QHBoxLayout;
        chkConnectionState = new QCheckBox;
        chkConnectionState->setFixedSize(QSize(20, 20));
        chkConnectionState->setStyleSheet("QCheckBox::indicator:unchecked { image: url(:/images/unchecked) }"
                                          "QCheckBox::indicator:checked { image: url(:/images/checked) }");

        lblConnectionState = new BlinkLabel(conv("Не подключен"));
        lblConnectionState->setFixedWidth(120);
        lblConnectionState->setMargin(10);
        lblConnectionState->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        btnCheckConnection = new QPushButton(conv("Проверка"));
        btnCheckConnection->setFixedWidth(100);
        hLayout->addStretch();
        hLayout->addWidget(chkConnectionState);
        hLayout->addWidget(lblConnectionState);
        hLayout->addWidget(btnCheckConnection);
        fLayout->addRow(hLayout);
        group->setLayout(fLayout);
    }

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->addWidget(group);

    btnApply = new QPushButton(conv("Применить"));
    btnApply->setFixedWidth(100);
    btnExit = new QPushButton(conv("Выход"));
    btnExit->setFixedWidth(100);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnApply);
    hLayout->addWidget(btnExit);

    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void DBConfigDialog::initConnections()
{
    connect(btnApply, SIGNAL(clicked()), SLOT(apply()));
    connect(btnExit, SIGNAL(clicked()), SLOT(close()));
    connect(btnCheckConnection, SIGNAL(clicked()), SLOT(checkConnection()));
}

void DBConfigDialog::apply()
{
    QString name = edtDatabaseName->text();
    QString host = edtDatabaseHost->text();
    int port = edtDatabasePort->value();
    QString user = edtDatabaseUser->text();
    QString password = edtDatabasePassword->text();

    if (!Settings::writeConfig(name, host, port, user, password))
        emit information(new Exception(conv("Ошибка записи настроек подключения"), 105));

    if (checkDatabaseConfig(name, host, port, user, password))
    {
        lblConnectionState->stop();
        close();
        emit databaseConnected();
    }
    else
    {
        lblConnectionState->setText(conv("Не подключен"));
        chkConnectionState->setChecked(false);
        lblConnectionState->stop();
        lblConnectionState->start(1000, 8);
    }
}

void DBConfigDialog::checkConnection()
{
    QString name = edtDatabaseName->text();
    QString host = edtDatabaseHost->text();
    int port = edtDatabasePort->value();
    QString user = edtDatabaseUser->text();
    QString password = edtDatabasePassword->text();
    if (checkDatabaseConfig(name, host, port, user, password))
    {
        chkConnectionState->setChecked(true);
        lblConnectionState->stop();
        lblConnectionState->setText(conv("Подключен"));
    }
    else
    {
        lblConnectionState->setText(conv("Не подключен"));
        chkConnectionState->setChecked(false);
        lblConnectionState->stop();
        lblConnectionState->start(1000, 8);
    }

}

bool DBConfigDialog::checkDatabaseConfig(const QString &name, const QString &host, int port,
                                         const QString &user, QString &password)
{
    return DatabaseAdapter::connect(name, host, port, user, password);
}
