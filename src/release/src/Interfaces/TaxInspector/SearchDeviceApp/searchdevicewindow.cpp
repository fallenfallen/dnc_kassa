/*====================================================================================
# «ДЭНСИ:Налоговый инспектор» module of «ДЭНСИ: КАССА» complex is an additional utility, wich allows to use FM/ECRP functions.
# © OOO "DNC Trade" 2011, Authors: Romanow Alexey.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:Налоговый инспектор» программного комплекса «ДЭНСИ: КАССА» - дополнительная утилита, предоставляющая доступ к функциям работы с ФП/ЭКЛЗ. 
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Романов Алексей.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "searchdevicewindow.h"

SeachDeviceWindow::SeachDeviceWindow(QWidget * parent) : QWidget(parent)
{
    searchEngine   = new SearchDevice;
    searchProgress = new SearchProgress;

    init();

    connect(btnStartSearch, SIGNAL(clicked()), SLOT(startSearch()));
    connect(comboDeviceType, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(searchEngine, SIGNAL(setOperationDescription(QString)), progressBar, SLOT(setOperationDescription(QString)));
    connect(searchEngine, SIGNAL(setProgressValue(double)), progressBar, SLOT(setProgressValue(double)));
    connect(searchEngine, SIGNAL(setValueRange(double,double)), progressBar, SLOT(setValueRange(double,double)));
    connect(searchEngine, SIGNAL(addFoundDevice(QString,QString,QString,QString)), this, SLOT(addFoundDevice(QString,QString,QString,QString)));
    connect(this, SIGNAL(setValue(double)), progressBar, SLOT(setProgressValue(double)));
    connect(deviceTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellDoubleClicked(int, int)));
    connect(searchProgress, SIGNAL(finished()), this, SLOT(finished()));

    setWindowTitle(conv("Поиск устройств"));
}

SeachDeviceWindow::~SeachDeviceWindow()
{
    delete searchEngine;
    delete searchProgress;
    delete progressBar;

    itemList->clear();
}

void SeachDeviceWindow::show()
{
    setFixedSize(450, 500);
    setCenterWindow();
    setVisible(true);
}

void SeachDeviceWindow::init()
{
    QFormLayout * fLayout = new QFormLayout;

    comboDeviceType = new QComboBox(this);
    comboDeviceType->addItem(conv("Фискальный регистратор"));
    fLayout->addRow(conv("Тип устройства"), comboDeviceType);

    comboDeviceProtocol = new QComboBox(this);
    comboDeviceProtocol->addItem(conv("Неизвестно"));
    comboDeviceProtocol->addItems(searchEngine->protocolList(0));
    fLayout->addRow(conv("Протокол"), comboDeviceProtocol);

    comboDevicePort = new QComboBox(this);
    comboDevicePort->addItem(conv("Неизвестно"));
    comboDevicePort->addItems(searchEngine->portList());
    fLayout->addRow(conv("Порт"), comboDevicePort);

    comboDeviceBaudRate = new QComboBox(this);
    comboDeviceBaudRate->addItem(conv("Неизвестно"));
    comboDeviceBaudRate->addItem(conv("2400"));
    comboDeviceBaudRate->addItem(conv("4800"));
    comboDeviceBaudRate->addItem(conv("9600"));
    comboDeviceBaudRate->addItem(conv("19200"));
    comboDeviceBaudRate->addItem(conv("38400"));
    comboDeviceBaudRate->addItem(conv("57600"));
    comboDeviceBaudRate->addItem(conv("115200"));
    fLayout->addRow(conv("Скорость обмена данными"), comboDeviceBaudRate);

    groupPassword = new QGroupBox(conv("Изменить стандартные пароли"), this);
    groupPassword->setCheckable(true);
    {
        QFormLayout * fLayout = new QFormLayout;
        edtAccessPassword = new QLineEdit(conv("0000"), this);
        fLayout->addRow(conv("Пароль доступа"), edtAccessPassword);
        edtAdminPassword = new QLineEdit(conv("00000030"), this);
        fLayout->addRow(conv("Пароль администратора"), edtAdminPassword);
        groupPassword->setLayout(fLayout);
    }
    groupPassword->setChecked(false);
    fLayout->addRow(groupPassword);

    btnStartSearch = new QPushButton(conv("Поиск"), this);
    btnStartSearch->setFixedWidth(100);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnStartSearch);
    fLayout->addItem(hLayout);

    itemList = new QList<QTableWidgetItem *>;
    deviceTable = new QTableWidget(0, 4);
    QTableWidgetItem * header = new QTableWidgetItem(conv("Тип устройства"));
    deviceTable->setHorizontalHeaderItem(0, header);
    header = new QTableWidgetItem(conv("Порт"));
    deviceTable->setHorizontalHeaderItem(1, header);
    header = new QTableWidgetItem(conv("Протокол"));
    deviceTable->setHorizontalHeaderItem(2, header);
    header = new QTableWidgetItem(conv("Скорость"));
    deviceTable->setHorizontalHeaderItem(3, header);
    deviceTable->horizontalHeader()->setStretchLastSection(true);
    deviceTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    fLayout->addRow(deviceTable);

    progressBar = new SearchProgressBar;
    fLayout->addRow(progressBar);
    progressBar->hide();

    setLayout(fLayout);
}

void SeachDeviceWindow::setCenterWindow()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void SeachDeviceWindow::startSearch()
{
    for (int i = 0; i < deviceTable->rowCount(); ++i)
        deviceTable->removeRow(i);

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    progressBar->show();
    setEnabled(false);

    QString deviceName = comboDeviceType->currentText();
    QString itemPort = comboDevicePort->currentText();
    QString itemBaudRate = comboDeviceBaudRate->currentText();
    QString itemProtocol = comboDeviceProtocol->currentText();

    Password passwords;
    passwords.accessPassword = edtAccessPassword->text();
    passwords.password = edtAdminPassword->text();

    searchProgress->run(searchEngine, &SearchDevice::searchFRConnectionParam,
                        deviceName, itemPort, itemBaudRate, itemProtocol, passwords);

    QApplication::restoreOverrideCursor();
}

void SeachDeviceWindow::currentIndexChanged(int index)
{
    comboDeviceProtocol->clear();
    comboDeviceProtocol->addItem(conv("Неизвестно"));
    comboDeviceProtocol->addItems(searchEngine->protocolList(index));
}

void SeachDeviceWindow::finished()
{
    progressBar->hide();
    setEnabled(true);
}

void SeachDeviceWindow::setEnabled(bool enabled)
{
    comboDeviceType->setEnabled(enabled);
    comboDevicePort->setEnabled(enabled);
    comboDeviceBaudRate->setEnabled(enabled);
    comboDeviceProtocol->setEnabled(enabled);
    btnStartSearch->setEnabled(enabled);
    groupPassword->setEnabled(enabled);
}

void SeachDeviceWindow::addFoundDevice(const QString &deviceName, const QString &port,
                                const QString &protocol, const QString &baudRate)
{
    int row = deviceTable->rowCount();
    deviceTable->setRowCount(row + 1);
    QTableWidgetItem * item = new QTableWidgetItem(deviceName);
    item->setFlags(Qt::ItemIsEnabled);
    deviceTable->setItem(row, 0, item);
    itemList->append(item);

    item = new QTableWidgetItem(port);
    item->setFlags(Qt::ItemIsEnabled);
    deviceTable->setItem(row, 1, item);
    itemList->append(item);

    item = new QTableWidgetItem(protocol);
    item->setFlags(Qt::ItemIsEnabled);
    deviceTable->setItem(row, 2, item);
    itemList->append(item);

    item = new QTableWidgetItem(baudRate);
    item->setFlags(Qt::ItemIsEnabled);
    deviceTable->setItem(row, 3, item);
    itemList->append(item);

    deviceTable->resizeColumnsToContents();
}

void SeachDeviceWindow::cellDoubleClicked(int row, int column)
{
    QString deviceName = deviceTable->item(row, 0)->text();
    QString devicePort = deviceTable->item(row, 1)->text();
    QString deviceProtocol = deviceTable->item(row, 2)->text();
    QString deviceBaudRate = deviceTable->item(row, 3)->text();
    QString accessPassword = edtAccessPassword->text();
    QString password = edtAdminPassword->text();

    ConnectOptions * options = searchEngine->getConnectOptions(deviceName, devicePort, deviceProtocol,
                                                               deviceBaudRate, password, accessPassword);

    emit deviceSelected(options);
}
