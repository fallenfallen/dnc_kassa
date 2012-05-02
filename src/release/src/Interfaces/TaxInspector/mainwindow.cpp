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

#include "mainwindow.h"

MainWindow::MainWindow(const QString &name, QWidget * parent) : QWidget(parent)
{
    init();

    frmodel = new FRModel;
    eklsmodel = new EKLSModel;
    calendar = new Calendar;
    searchWindow = new SeachDeviceWindow;
    cancelFlag = false;

    connect(frmodel, SIGNAL(signalUpdateTable(const FRParam *)), this, SLOT(slotFillTable(const FRParam *)));
    connect(frmodel, SIGNAL(signalFail(QString)), this, SLOT(slotFail(QString)));
    connect(eklsmodel, SIGNAL(signalFail(QString)), this, SLOT(slotFail(QString)));
    connect(btnCopyDate, SIGNAL(clicked()), this, SLOT(slotCopyDate()));
    connect(btnSetDate, SIGNAL(clicked()), this, SLOT(slotSetDate()));
    connect(comboFRReportType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFRCurrentTypeChanged(int)));
    connect(comboEKLSReportType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotEKLSCurrentTypeChanged(int)));
    connect(btnSetSerialNumber, SIGNAL(clicked()), this, SLOT(slotSetSerialNumber()));
    connect(btnFiscalization, SIGNAL(clicked()), this, SLOT(slotFiscalization()));
    connect(btnFRMakeReport, SIGNAL(clicked()), this, SLOT(slotFRMakeReport()));
    connect(btnTestSale, SIGNAL(clicked()), this, SLOT(slotTestSale()));
    connect(btnMakeZReport, SIGNAL(clicked()), this, SLOT(slotMakeZReport()));
    connect(btnFastFiscalization, SIGNAL(clicked()), this, SLOT(slotFastFiscalization()));
    connect(btnFRReportBegin, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(btnFRReportEnd, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(btnEKLSReportBegin, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(btnEKLSReportEnd, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(slotSelectDate(QDate)));
    connect(btnActivation, SIGNAL(clicked()), this, SLOT(slotActivation()));
    connect(btnCloseArchive, SIGNAL(clicked()), this, SLOT(slotCloseArchive()));
    connect(btnActivationResult, SIGNAL(clicked()), this, SLOT(slotActivationResult()));
    connect(btnPrintShift, SIGNAL(clicked()), this, SLOT(slotPrintShift()));
    connect(btnPrinthiftFromEKLS, SIGNAL(clicked()), this, SLOT(slotPrintShiftFromEKLS()));
    connect(btnPrintByNumber, SIGNAL(clicked()), this, SLOT(slotPrintByNumber()));
    connect(btnEKLSMakeReport, SIGNAL(clicked()), this, SLOT(slotEKLSMakeReport()));
    connect(btnEKLSBreakPrint, SIGNAL(clicked()), this, SLOT(slotEKLSBreakPrint()));
    connect(frmodel, SIGNAL(signalFail(QString)), this, SIGNAL(cancel()));
    connect(searchWindow, SIGNAL(deviceSelected(const ConnectOptions *)), this, SLOT(deviceSelected(const ConnectOptions *)));
    connect(selectedDateRange, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectionChanged()));
    connect(btnExtendedReportBegin, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(btnExtendedReportEnd, SIGNAL(clicked()), this, SLOT(slotShowCalendar()));
    connect(btnPrintReports, SIGNAL(clicked()), SLOT(printReports()));
    connect(btnBreakPrintReports, SIGNAL(clicked()), SLOT(breakPrintReports()));
    connect(this, SIGNAL(currrentReportPrinted(int)), SLOT(reportPrinted(int)));
    connect(frmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));
    connect(eklsmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));

    setFixedSize(800, 600);
    setWindowTitle(name);
    setCenterWindow();
}

void MainWindow::confirmRequest(int * request)
{
    int result = QMessageBox::warning(this, conv("Внимание"), conv("В ККМ закончилась бумага. Для продолжения печати"
                                                                   "необходимо вставить новую бумагу. Продолжить печать?"),
                                            conv("Продолжить"), conv("Прервать"));
    *request = result;

    if (result)
        emit cancel();
}

void MainWindow::setCenterWindow()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void MainWindow::updateInputFields()
{
    switch (protocol)
    {
    case 1:
        edtNewPassword->setMaxLength(8);
        edtPasswordConfirm->setMaxLength(8);
        edtRegistrationNumber->setMaxLength(10);
        break;

    case 2:
        edtNewPassword->setMaxLength(8);
        edtPasswordConfirm->setMaxLength(8);
        edtRegistrationNumber->setMaxLength(10);
        break;

    case 3:
        edtNewPassword->setMaxLength(5);
        edtPasswordConfirm->setMaxLength(5);
        edtRegistrationNumber->setMaxLength(8);
        break;

    case 4:
        edtNewPassword->setMaxLength(6);
        edtPasswordConfirm->setMaxLength(6);
        edtRegistrationNumber->setMaxLength(10);
        break;

    case 5:
        edtNewPassword->setMaxLength(16);
        edtPasswordConfirm->setMaxLength(16);
        edtRegistrationNumber->setMaxLength(10);
        break;
    }
}

void MainWindow::init()
{
    QGridLayout * layout = new QGridLayout;

    QLabel * lblCaption = new QLabel(conv("Параметры устройства"), this);
    layout->addWidget(lblCaption, 0, 0);
    // QTableWidget таблица параметров
    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setRowCount(8);
    itemList = new QList<QTableWidgetItem *>;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 2; ++j)
        {
            QTableWidgetItem * item = new QTableWidgetItem;
            item->setFlags(Qt::ItemIsEnabled);
            table->setItem(i, j, item);
            itemList->append(item);
        }

    QTableWidgetItem * header = new QTableWidgetItem(conv("Параметр"));
    table->setHorizontalHeaderItem(0, header);
    header = new QTableWidgetItem(conv("Значение"));
    table->setHorizontalHeaderItem(1, header);
    layout->addWidget(table, 1, 0);

    // GroupBox установка даты и времени
    QGroupBox * grpDateTime = new QGroupBox(conv("Дата / время"), this);
    QVBoxLayout * vLayout = new QVBoxLayout;
    lblPCDate = new QLabel(QDate::currentDate().toString("dd.MM.yyyy"), this);
    lblPCDate->setFixedWidth(100);
    lblPCTime = new QLabel(QTime::currentTime().toString("hh:mm"), this);
    lblPCTime->setFixedWidth(100);
    QFormLayout * fLayout = new QFormLayout;
    fLayout->addRow(conv("Дата ПК:"), lblPCDate);
    fLayout->addRow(conv("Время ПК:"), lblPCTime);
    vLayout->addLayout(fLayout);
    btnCopyDate = new QPushButton(conv("Скопировать"), this);
    btnCopyDate->setFixedWidth(120);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnCopyDate);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    edtKKMDate = new QDateEdit(this);
    edtKKMDate->setFixedWidth(100);
    edtKKMTime = new QTimeEdit(this);
    edtKKMTime->setFixedWidth(100);
    fLayout = new QFormLayout;
    fLayout->addRow(conv("Дата ККМ:"), edtKKMDate);
    fLayout->addRow(conv("Время ККМ:"), edtKKMTime);
    vLayout->addLayout(fLayout);
    hLayout = new QHBoxLayout;
    btnSetDate = new QPushButton(conv("Установить"), this);
    hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnSetDate);
    hLayout->addStretch();
    btnSetDate->setFixedWidth(120);
    vLayout->addLayout(hLayout);
    grpDateTime->setLayout(vLayout);
    layout->addWidget(grpDateTime, 2, 0);

    // QTabWidget 1 - работа с ЭКЛЗ
    //            2 - работа с ФП
    //            3 - расширенная работа с фискальными отчетами
    tabControl = new QTabWidget(this);
    tabControl->setFixedWidth(550);
    frameEKLS = new QFrame(this);
    tabControl->addTab(frameEKLS, conv("Работа с ЭКЛЗ"));
    frameFP = new QFrame(this);
    tabControl->addTab(frameFP, conv("Работа с ФП"));
    frameReports = new QFrame(this);
    tabControl->addTab(frameReports, conv("Расширенные фискальные отчеты"));

    // Вкладка 1 работа с ЭКЛЗ

    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    btnActivation = new QPushButton(conv("Активизация ЭКЛЗ"), this);
    btnActivation->setFixedWidth(175);
    btnCloseArchive = new QPushButton(conv("Закрытие архива ЭКЛЗ"), this);
    btnCloseArchive->setFixedWidth(175);
    btnActivationResult = new QPushButton(conv("Итог активизации ЭКЛЗ"), this);
    btnActivationResult->setFixedWidth(175);
    hLayout->addWidget(btnActivation);
    hLayout->addStretch();
    hLayout->addWidget(btnCloseArchive);
    hLayout->addStretch();
    hLayout->addWidget(btnActivationResult);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());

    QLabel * lblShiftNumber = new QLabel(conv("Номер смены"), this);
    edtShiftNumber = new QLineEdit(this);
    edtShiftNumber->setFixedWidth(260);
    edtShiftNumber->setValidator(new QIntValidator(edtShiftNumber));
    hLayout = new QHBoxLayout;
    hLayout->addWidget(lblShiftNumber);
    hLayout->addStretch();
    hLayout->addWidget(edtShiftNumber);
    vLayout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    btnPrintShift = new QPushButton(conv("Контрольная лента за смену"), this);
    btnPrintShift->setFixedWidth(260);
    btnPrinthiftFromEKLS = new QPushButton(conv("Итоги смены из ЭКЛЗ"), this);
    btnPrinthiftFromEKLS->setFixedWidth(260);
    hLayout->addWidget(btnPrintShift);
    hLayout->addWidget(btnPrinthiftFromEKLS);

    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());
    hLayout = new QHBoxLayout;
    QLabel * lblDocumentNumber = new QLabel(conv("КПК документа"), this);
    edtDocumentNumber = new QLineEdit(this);
    edtDocumentNumber->setFixedWidth(260);
    edtDocumentNumber->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]*"), edtDocumentNumber));
    btnPrintByNumber = new QPushButton(conv("Печать документа по КПК"), this);
    btnPrintByNumber->setFixedWidth(260);
    hLayout->addWidget(lblDocumentNumber);
    hLayout->addStretch();
    hLayout->addWidget(edtDocumentNumber);
    vLayout->addLayout(hLayout);
    hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(btnPrintByNumber);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());

    QGroupBox * grpEKLSReports = new QGroupBox(conv("Отчеты ЭКЛЗ"), this);
    fLayout = new QFormLayout;
    hLayout = new QHBoxLayout;
    comboEKLSReportType = new QComboBox(this);
    comboEKLSReportType->addItem(conv("Краткий по датам"));
    comboEKLSReportType->addItem(conv("Полный по датам"));
    comboEKLSReportType->addItem(conv("Краткий по сменам"));
    comboEKLSReportType->addItem(conv("Полный по сменам"));

    fLayout->addRow(conv("Тип отчета"), comboEKLSReportType);
    {
        QHBoxLayout * hLayout = new QHBoxLayout;
        edtEKLSReportBegin = new QLineEdit(this);
        edtEKLSReportBegin->setValidator(new QIntValidator(edtEKLSReportBegin));
        btnEKLSReportBegin = new QPushButton(QIcon(":/calendar"), QString(), this);
        btnEKLSReportBegin->setObjectName("btnEKLSReportBegin");
        btnEKLSReportBegin->setFixedSize(25, 25);
        hLayout->addWidget(edtEKLSReportBegin);
        hLayout->addWidget(btnEKLSReportBegin);
        fLayout->addRow(conv("Начало"), hLayout);
    }
    {
        QHBoxLayout * hLayout = new QHBoxLayout;
        edtEKLSReportEnd = new QLineEdit(this);
        edtEKLSReportEnd->setValidator(new QIntValidator(edtEKLSReportEnd));
        btnEKLSReportEnd = new QPushButton(QIcon(":/calendar"), QString(), this);
        btnEKLSReportEnd->setObjectName("btnEKLSReportEnd");
        btnEKLSReportEnd->setFixedSize(25, 25);
        hLayout->addWidget(edtEKLSReportEnd);
        hLayout->addWidget(btnEKLSReportEnd);
        fLayout->addRow(conv("Конец"), hLayout);
    }
    slotEKLSCurrentTypeChanged(0);
    hLayout->addLayout(fLayout);
    {
        QVBoxLayout * vLayout = new QVBoxLayout;
        btnEKLSMakeReport = new QPushButton(conv("Снять отчет"), this);
        btnEKLSMakeReport->setFixedWidth(150);
        vLayout->addWidget(btnEKLSMakeReport);
        btnEKLSBreakPrint = new QPushButton(conv("Прервать печать"), this);
        btnEKLSBreakPrint->setFixedWidth(150);
        vLayout->addWidget(btnEKLSBreakPrint);
        hLayout->addLayout(vLayout);
    }
    grpEKLSReports->setLayout(hLayout);
    vLayout->addWidget(grpEKLSReports);

    vLayout->addStretch();
    frameEKLS->setLayout(vLayout);

    // Вкладка 2 работа с ФП
    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    lblSerialNumber = new QLabel(conv("Заводской номер"), this);
    edtSerialNumber = new QLineEdit(this);
    edtSerialNumber->setFixedWidth(200);
    edtSerialNumber->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]*"), edtSerialNumber));
    btnSetSerialNumber = new QPushButton(conv("Установить номер"), this);
    btnSetSerialNumber->setFixedWidth(150);
    hLayout->addWidget(lblSerialNumber);
    hLayout->addStretch();
    hLayout->addWidget(edtSerialNumber);
    hLayout->addWidget(btnSetSerialNumber);

    vLayout->addLayout(hLayout);
    line = addLine();
    vLayout->addWidget(line);

    fLayout = new QFormLayout;
    QLabel * lblNewPassword = new QLabel(conv("Новый пароль налогового инспектора"), this);
    lblNewPassword->setWordWrap(true);
    edtNewPassword = new QLineEdit(this);
    edtNewPassword->setEchoMode(QLineEdit::Password);
    edtNewPassword->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), edtNewPassword));
    fLayout->addRow(lblNewPassword, edtNewPassword);
    edtPasswordConfirm = new QLineEdit(this);
    edtPasswordConfirm->setEchoMode(QLineEdit::Password);
    edtPasswordConfirm->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), edtPasswordConfirm));
    fLayout->addRow(conv("Подтверждение пароля"), edtPasswordConfirm);
    edtRegistrationNumber = new QLineEdit(this);
    edtRegistrationNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), edtRegistrationNumber));
    fLayout->addRow(conv("Регистрационный номер КММ"), edtRegistrationNumber);
    edtINN = new QLineEdit(this);
    edtINN->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), edtINN));
    fLayout->addRow(conv("ИНН"), edtINN);
    hLayout = new QHBoxLayout;
    btnFiscalization = new QPushButton(conv("Фискализация"), this);
    btnFiscalization->setFixedWidth(150);
    hLayout->addLayout(fLayout);
    hLayout->addWidget(btnFiscalization);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());

    QGroupBox * grpFiscalReports = new QGroupBox(conv("Фискальные отчеты"), this);
    fLayout = new QFormLayout;
    hLayout = new QHBoxLayout;
    comboFRReportType = new QComboBox(this);
    comboFRReportType->addItem(conv("Краткий по датам"));
    comboFRReportType->addItem(conv("Полный по датам"));
    comboFRReportType->addItem(conv("Краткий по сменам"));
    comboFRReportType->addItem(conv("Полный по сменам"));

    fLayout->addRow(conv("Тип отчета"), comboFRReportType);
    hLayout = new QHBoxLayout;
    edtFRReportBegin = new QLineEdit(this);
    edtFRReportBegin->setValidator(new QIntValidator(edtFRReportBegin));
    btnFRReportBegin = new QPushButton(QIcon(":/calendar"), QString(), this);
    btnFRReportBegin->setObjectName("btnFRReportBegin");
    QLabel * lblFRReportBegin = new QLabel(conv("Конец"), this);
    lblFRReportBegin->setFixedWidth(200);
    btnFRReportBegin->setFixedSize(25, 25);
    hLayout->addWidget(edtFRReportBegin);
    hLayout->addWidget(btnFRReportBegin);
    fLayout->addRow(lblFRReportBegin, hLayout);

    hLayout = new QHBoxLayout;
    edtFRReportEnd = new QLineEdit(this);
    edtFRReportEnd->setValidator(new QIntValidator(edtFRReportEnd));
    btnFRReportEnd = new QPushButton(QIcon(":/calendar"), QString(), this);
    btnFRReportEnd->setObjectName("btnFRReportEnd");
    btnFRReportEnd->setFixedSize(25, 25);
    QLabel * lblFRReportEnd = new QLabel(conv("Конец"), this);
    lblFRReportEnd->setFixedWidth(200);
    hLayout->addWidget(edtFRReportEnd);
    hLayout->addWidget(btnFRReportEnd);
    fLayout->addRow(lblFRReportEnd, hLayout);

    hLayout = new QHBoxLayout;
    btnFRMakeReport = new QPushButton(conv("Снять отчет"), this);
    btnFRMakeReport->setFixedWidth(150);
    hLayout->addStretch();
    hLayout->addWidget(btnFRMakeReport);
    fLayout->addRow(hLayout);
    slotFRCurrentTypeChanged(0);

    grpFiscalReports->setLayout(fLayout);
    vLayout->addWidget(grpFiscalReports);
    vLayout->addWidget(addLine());
    hLayout = new QHBoxLayout;
    QLabel * lblTestSale = new QLabel(conv("Тестовая продажа"), this);
    edtTestSale = new QLineEdit("1.11", this);
    edtTestSale->setFixedWidth(200);
    edtTestSale->setValidator(new QDoubleValidator(edtTestSale));
    btnTestSale = new QPushButton(conv("Пробить чек"), this);
    btnTestSale->setFixedWidth(150);
    hLayout->addWidget(lblTestSale);
    hLayout->addStretch();
    hLayout->addWidget(edtTestSale);
    hLayout->addWidget(btnTestSale);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());
    hLayout = new QHBoxLayout;
    QLabel * lblZReport = new QLabel(conv("Отчет с гашением"), this);
    btnMakeZReport = new QPushButton(conv("Снять Z-отчет"), this);
    btnMakeZReport->setFixedWidth(150);
    hLayout->addWidget(lblZReport);
    hLayout->addStretch();
    hLayout->addWidget(btnMakeZReport);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(addLine());
    hLayout = new QHBoxLayout;
    btnFastFiscalization = new QPushButton(conv("Быстрая фискализация"), this);
    btnFastFiscalization->setFixedSize(200, 40);
    chkEKLSFullFiscalization = new QCheckBox(conv("Включить отчеты ЭКЛЗ"), this);
    hLayout->addWidget(chkEKLSFullFiscalization);
    hLayout->addStretch();
    hLayout->addWidget(btnFastFiscalization);
    vLayout->addStretch();
    vLayout->addLayout(hLayout);
    vLayout->addStretch();
    frameFP->setLayout(vLayout);

    // Вкладка 3 расширенная работа с фискальными отчетами
    vLayout = new QVBoxLayout;
    fLayout = new QFormLayout;
    hLayout = new QHBoxLayout;

    QGroupBox * grpExtendedReports = new QGroupBox(conv("Фискальные отчеты"), this);
    comboExtendedReportType = new QComboBox(this);
    comboExtendedReportType->addItem(conv("Краткий по датам"));
    comboExtendedReportType->addItem(conv("Полный по датам"));

    fLayout->addRow(conv("Тип отчета"), comboExtendedReportType);
    {
        QHBoxLayout * hLayout = new QHBoxLayout;
        edtExtendedReportBegin = new QLineEdit(this);
        edtExtendedReportBegin->setValidator(new QIntValidator(edtExtendedReportBegin));
        btnExtendedReportBegin = new QPushButton(QIcon(":/calendar"), QString(), this);
        btnExtendedReportBegin->setObjectName("btnExtendedReportBegin");
        edtExtendedReportBegin->setInputMask("99.99.9999");
        edtExtendedReportBegin->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnExtendedReportBegin->setFixedSize(25, 25);
        QLabel * lblExtendedReportBegin = new QLabel(conv("Начало"), this);
        lblExtendedReportBegin->setFixedWidth(200);
        hLayout->addWidget(edtExtendedReportBegin);
        hLayout->addWidget(btnExtendedReportBegin);
        fLayout->addRow(lblExtendedReportBegin, hLayout);
    }
    {
        QHBoxLayout * hLayout = new QHBoxLayout;
        edtExtendedReportEnd = new QLineEdit(this);
        edtExtendedReportEnd->setValidator(new QIntValidator(edtExtendedReportEnd));
        btnExtendedReportEnd = new QPushButton(QIcon(":/calendar"), QString(), this);
        btnExtendedReportEnd->setObjectName("btnExtendedReportEnd");
        edtExtendedReportEnd->setInputMask("99.99.9999");
        edtExtendedReportEnd->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnExtendedReportEnd->setFixedSize(25, 25);
        QLabel * lblExtendedReportEnd = new QLabel(conv("Конец"), this);
        lblExtendedReportEnd->setFixedWidth(200);
        hLayout->addWidget(edtExtendedReportEnd);
        hLayout->addWidget(btnExtendedReportEnd);
        fLayout->addRow(lblExtendedReportEnd, hLayout);
    }

    grpExtendedReports->setLayout(fLayout);
    vLayout->addWidget(grpExtendedReports);

    QSplitter * splitter = new QSplitter(this);
    tableRange = new QTableWidget(0, 3, this);
    QTableWidgetItem * tableItem = new QTableWidgetItem;
    tableRange->setHorizontalHeaderItem(0, tableItem);
    tableItem = new QTableWidgetItem(conv("Диапазон"));
    tableRange->setHorizontalHeaderItem(1, tableItem);
    tableItem = new QTableWidgetItem(conv("Тип диапозона"));
    tableRange->setHorizontalHeaderItem(2, tableItem);
    tableRange->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tableRange->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);

    selectedDateRange = new QListWidget(this);
    QListWidgetItem * listItem = new QListWidgetItem(conv("По месяцам"));
    listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
    listItem->setCheckState(Qt::Unchecked);
    selectedDateRange->addItem(listItem);
    listItem = new QListWidgetItem(conv("По кварталам"));
    listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
    listItem->setCheckState(Qt::Unchecked);
    selectedDateRange->addItem(listItem);
    listItem = new QListWidgetItem(conv("По годам"));
    listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
    listItem->setCheckState(Qt::Unchecked);
    selectedDateRange->addItem(listItem);
    listItem = new QListWidgetItem(conv("Весь дипазон"));
    listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
    listItem->setCheckState(Qt::Unchecked);
    selectedDateRange->addItem(listItem);

    splitter->addWidget(selectedDateRange);
    splitter->addWidget(tableRange);
    vLayout->addWidget(splitter);
    hLayout = new QHBoxLayout;
    btnPrintReports = new QPushButton(conv("Печать отчетов"));
    btnPrintReports->setFixedWidth(150);
    btnBreakPrintReports = new QPushButton(conv("Прервать печать"));
    btnBreakPrintReports->setFixedWidth(150);
    hLayout->addStretch();
    hLayout->addWidget(btnPrintReports);
    hLayout->addWidget(btnBreakPrintReports);
    vLayout->addLayout(hLayout);
    frameReports->setLayout(vLayout);
    layout->addWidget(tabControl, 1, 1, 2, 2);

    setLayout(layout);
}

bool MainWindow::showWindow()
{
    bool connection = frmodel->checkConnection();
    if (connection)
    {
        updateInputFields();
        frmodel->updateInformation();
        show();
    }
    else
        searchWindow->show();

    return connection;
}

void MainWindow::finalize()
{
    qDeleteAll(itemList->begin(), itemList->end());
    itemList->clear();

    delete calendar;
    delete eklsmodel;
    delete frmodel;
    delete searchWindow;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    finalize();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_F12)
        close();
}

void MainWindow::slotFillTable(const FRParam * param)
{
    protocol = param->protocol;

    for (int i = 0; i < 8; ++i)
    {
        QTableWidgetItem * value = itemList->value(2 * i + 1);
        QTableWidgetItem * name = itemList->value(2 * i);
        switch (i)
        {
        case 0:
            {
                QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
                name->setText(conv("Название"));
                value->setText(QString(codec->toUnicode(param->description)));
            }
            break;

        case 1:
            {
                QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
                name->setText(conv("Заводской номер"));
                QString str = QString(codec->toUnicode(param->serialNumber));
                bool flag;
                int number = str.toInt(&flag);
                if (!flag || number == 0)
                    str = "";

                if (!str.isEmpty())
                {
                    switch (param->protocol)
                    {
                    case 1:
                        edtSerialNumber->setMaxLength(10);
                        edtSerialNumber->setValidator(new QDoubleValidator(edtSerialNumber));
                        lblSerialNumber->setText(conv("Код лицензии"));
                        btnSetSerialNumber->setText(conv("Установить код"));
                        btnSetSerialNumber->setObjectName("btnSetLicense");
                        break;

                    case 2:
                        edtSerialNumber->setMaxLength(16);
                        edtSerialNumber->setValidator(new QDoubleValidator(edtSerialNumber));
                        lblSerialNumber->setText(conv("Код лицензии"));
                        btnSetSerialNumber->setText(conv("Установить код"));
                        btnSetSerialNumber->setObjectName("btnSetLicense");
                        break;

                    default:
                        lblSerialNumber->hide();
                        edtSerialNumber->hide();
                        btnSetSerialNumber->hide();
                        line->hide();
                    }
                }
                else
                {
                    str = conv("номер не введен");
                    btnSetSerialNumber->setObjectName("btnSetSerialNumber");
                }
                value->setText(str);
            }
            break;

        case 2:
            name->setText(conv("Дата"));
            value->setText(QDate(param->year, param->mounth, param->day).toString("dd.MM.yyyy"));
            edtKKMDate->setDate(QDate(param->year, param->mounth, param->day));
            break;

        case 3:
            name->setText(conv("Время"));
            value->setText(QTime(param->hour, param->minute).toString("hh:mm"));
            edtKKMTime->setTime(QTime(param->hour, param->minute, param->second));
            break;

        case 4:
            name->setText(conv("Смена в ККМ"));
            value->setText(conv(param->sessionOpened ? "открыта" : "закрыта"));
            break;

        case 5:
            name->setText(conv("Фискализирован"));
            value->setText(conv(param->fiscal ? "да" : "нет"));
            break;

        case 6:
            license = param->license;
            name->setText(conv("Лицензия"));
            value->setText(conv(param->license ? "введена" : "не введена"));
            if (license)
            {
                lblSerialNumber->hide();
                edtSerialNumber->hide();
                btnSetSerialNumber->hide();
                line->hide();
            }
            break;

        case 7:
            name->setText(conv("Режим / подрежим"));
            value->setText(QString::number(param->mode) + " / " + QString::number(param->advancedMode));
            break;
        }
    }
    table->resizeRowsToContents();
    table->resizeRowsToContents();
}

void MainWindow::slotFail(const QString &text)
{
    QMessageBox::critical(this, conv("Ошибка"), text);
}

void MainWindow::slotCopyDate()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    lblPCDate->setText(QString::number(date.day()) + "." + QString::number(date.month()) + "." + QString::number(date.year()));
    lblPCTime->setText(QString::number(time.hour()) + ":" + QString::number(time.minute()));
    edtKKMDate->setDate(QDate::currentDate());
    edtKKMTime->setTime(QTime::currentTime());
}

void MainWindow::slotSetDate()
{
    int day = edtKKMDate->date().day();
    int month = edtKKMDate->date().month();
    int year = edtKKMDate->date().year();
    int hour = edtKKMTime->time().hour();
    int minute = edtKKMTime->time().minute();
    int second = edtKKMTime->time().second();
    frmodel->setDateTime(day, month, abs(year - 2000), hour, minute, second);
}

void MainWindow::slotSetSerialNumber()
{
    if (!edtSerialNumber->text().isEmpty())
    {
        QObject * object = QObject::sender();
        if (QPushButton * button = qobject_cast<QPushButton *>(object))
        {
            int result, msgResult = 0;
            if (button->objectName() == "btnSetSerialNumber")
            {
                bool ok;
                QString password = QInputDialog::getText(this, conv("Пароль налогового инспектора"), conv("Введите пароль налогового инспектора"),
                                                         QLineEdit::Password, "", &ok, Qt::Dialog);
                if (ok && !password.isEmpty())
                {
                    result = frmodel->setSerialNumber(edtSerialNumber->text(), password);
                    if (!result)
                        QMessageBox::information(this, conv("Внимание"), conv("Серийный номер успешно установлен"));
                }
            }
            else if (button->objectName() == "btnSetLicense")
            {
                if (license)
                    msgResult = QMessageBox::question(this, conv("Внимание"), conv("Номер лицензии успешно введен. Ввод неверной лицензии может заблокировать работу устройства"),
                                                          conv("Продолжить"), conv("Отмена"));

                if (!msgResult)
                {
                    result = frmodel->setLicence(edtSerialNumber->text());
                    if (!result)
                        QMessageBox::information(this, conv("Внимание"), conv("Код лицензии успешно введен"));
                }
            }
        }
    }
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Не введено значение параметра"));
}

void MainWindow::slotFiscalization()
{
    QString registrationNumber = edtRegistrationNumber->text();
    QString INN = edtINN->text();
    QString newPassword = edtNewPassword->text();

    int result = QMessageBox::information(this, conv("Внимание"), conv("Внимание! ККМ может быть ЗАБЛОКИРОВАНА при вводе неправильного ") +\
                                                conv("пароля налогового инспектора. Вы собираетесь провести регистрацию ") +
                                                conv("со следующими реквизитами:\n") +
                                                conv("- регистрационный номер ККМ: ") + registrationNumber + "\n" +
                                                conv("- ИНН владельца ККМ: ") + INN + "\n" +
                                                conv("- дата/время: ") + edtKKMDate->text() + " " + edtKKMTime->text(),
                                                conv("Продолжить"), conv("Отмена"));
    if (result == 0)
    {
        bool ok;
        QString password = QInputDialog::getText(this, conv("Пароль налогового инспектора"), conv("Введите пароль налогового инспектора"),
                                                 QLineEdit::Password, "", &ok, Qt::Dialog);
        if (ok && !password.isEmpty())
            if (edtNewPassword->text() == edtPasswordConfirm->text())
                frmodel->fiscalization(registrationNumber, INN, newPassword, password);
            else
                QMessageBox::critical(this, conv("Ошибка"), conv("Не совпадают введенный пароль и подтверждение"));
    }
}

void MainWindow::slotFRMakeReport()
{
    if (QMessageBox::information(this, conv("Внимание"), conv("Внимание! ККМ может быть ЗАБЛОКИРОВАНА ") +
                                 conv("при вводе неправильного пароля налогового инспектора"), conv("Продолжить"), conv("Отмена")) == 0)
    {
        bool ok;
        int index = comboFRReportType->currentIndex();
        QString password = QInputDialog::getText(this, conv("Пароль налогового инспектора"), conv("Введите пароль налогового инспектора"),
                         QLineEdit::Password, "", &ok, Qt::Dialog);
        if (index < 2)
        {
            QDate dateBegin = QDate::fromString(edtFRReportBegin->text(), "dd.MM.yyyy");
            QDate dateEnd = QDate::fromString(edtFRReportEnd->text(), "dd.MM.yyyy");

            if (ok && !password.isEmpty())
                makeReportsByDate(frmodel, index, &dateBegin, &dateEnd, password);
        }
        else
        {
            int sessionBegin = edtFRReportBegin->text().toInt();
            int sessionEnd = edtFRReportEnd->text().toInt();

            if (ok && !password.isEmpty())
                makeReportsBySession(frmodel, index % 2, sessionBegin, sessionEnd, password);
        }
    }
}

QFrame * MainWindow::addLine()
{
    QFrame * line = new QFrame;
    line->resize(500, 3);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}

void MainWindow::slotEKLSCurrentTypeChanged(int index)
{
    if (index > 1)
    {
        edtEKLSReportBegin->setInputMask("");
        edtEKLSReportBegin->setText("1");
        btnEKLSReportBegin->hide();
        edtEKLSReportEnd->setInputMask("");
        edtEKLSReportEnd->setText("1");
        btnEKLSReportEnd->hide();
    }
    else
    {
        edtEKLSReportBegin->setInputMask("99.99.9999");
        edtEKLSReportBegin->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnEKLSReportBegin->show();
        edtEKLSReportEnd->setInputMask("99.99.9999");
        edtEKLSReportEnd->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnEKLSReportEnd->show();
    }
}

void MainWindow::slotFRCurrentTypeChanged(int index)
{
    if (index > 1)
    {
        edtFRReportBegin->setInputMask("");
        edtFRReportBegin->setText("1");
        btnFRReportBegin->hide();
        edtFRReportEnd->setInputMask("");
        edtFRReportEnd->setText("1");
        btnFRReportEnd->hide();
    }
    else
    {
        edtFRReportBegin->setInputMask("99.99.9999");
        edtFRReportBegin->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnFRReportBegin->show();
        edtFRReportEnd->setInputMask("99.99.9999");
        edtFRReportEnd->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        btnFRReportEnd->show();
    }
}

void MainWindow::slotTestSale()
{
    double price = edtTestSale->text().toDouble();
    frmodel->testSale(price);
}

void MainWindow::slotMakeZReport()
{
    frmodel->report();
}

void MainWindow::fastFiscalization(const QString password)
{
    QString registrationNumber = edtRegistrationNumber->text();
    QString INN = edtINN->text();
    QString newPassword = edtNewPassword->text();

    emit setOperationDescription(conv("Фискализация"));
    int result = frmodel->fiscalization(registrationNumber, INN, newPassword, password);

    if (!result)
    {
        frmodel->waitForRelease();

        emit setOperationDescription(conv("Печать чека"));
        double price = edtTestSale->text().toDouble();
        result = frmodel->testSale(price);

        if (!result)
        {
            frmodel->waitForRelease();
            emit setOperationDescription(conv("Снятие отчета с гашением"));
            result = frmodel->report();

            if (!result)
            {
                frmodel->waitForRelease();

                Model * model = frmodel;
                int index = comboFRReportType->currentIndex();
                QDate dateBegin = QDate::fromString(edtFRReportBegin->text(), "dd.MM.yyyy");
                QDate dateEnd = QDate::fromString(edtFRReportEnd->text(), "dd.MM.yyyy");
                int sessionBegin = edtFRReportBegin->text().toInt();
                int sessionEnd = edtFRReportEnd->text().toInt();

                emit setOperationDescription(conv("Снятие фискального отчета"));
                if (index < 2)
                    result = makeReportsByDate(model, index, &dateBegin, &dateEnd, newPassword);
                else
                    result = makeReportsBySession(model, index % 2, sessionBegin, sessionEnd, newPassword);

                if (!result)
                {
                    frmodel->waitForRelease();
                    if (chkEKLSFullFiscalization->checkState() == Qt::Checked)
                    {
                        model = eklsmodel;
                        emit setOperationDescription(conv("Снятие отчета из ЭКЛЗ"));
                        if (index < 2)
                            result = makeReportsByDate(model, index, &dateBegin, &dateEnd);
                        else
                            result = makeReportsBySession(model, index % 2, sessionBegin, sessionEnd);

                        if (!result)
                        {
                            frmodel->waitForRelease();
                            emit setOperationDescription(conv("Результат активизаиции"));
                            eklsmodel->activationResult();
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::slotFastFiscalization()
{
    QString registrationNumber = edtRegistrationNumber->text();
    QString INN = edtINN->text();

    QString text = conv("Внимание! ККМ может быть ЗАБЛОКИРОВАНА при вводе неправильного ") +
                   conv("пароля налогового инспектора. Вы собираетесь провести регистрацию ") +
                   conv("со следующими реквизитами:\n") +
                   conv("- регистрационный номер ККМ: ") + registrationNumber + "\n" +
                   conv("- ИНН владельца ККМ: ") + INN + "\n" +
                   conv("- дата/время: ") + edtKKMDate->text() + " " + edtKKMTime->text() + "\n" +
                   conv("- тестовая продажа на сумму ") + edtTestSale->text() + "\n" +
                   conv("- отчет с гашением (Z-отчет)\n") +
                   conv("- фискальный отчет (тип: ") + comboFRReportType->currentText() + conv(") в диапазоне ") +
                   edtFRReportBegin->text() + " - " + edtFRReportEnd->text();
    if (chkEKLSFullFiscalization->checkState() == Qt::Checked)
    {
        text = text + conv("\n- отчет ЭКЛЗ (тип: ") + comboFRReportType->currentText() + conv(") в диапазоне ") +
                      edtFRReportBegin->text() + " - " + edtFRReportEnd->text() + "\n" +
                      conv("- итог активизации");
    }

    int result = QMessageBox::information(this, conv("Внимание"),  text, conv("Продолжить"), conv("Отмена"));
    if (result == 0)
    {
        if (edtNewPassword->text() == edtPasswordConfirm->text())
        {
            bool ok;
            QString password = QInputDialog::getText(this, conv("Пароль налогового инспектора"), conv("Введите пароль налогового инспектора"),
                                                     QLineEdit::Password, "", &ok, Qt::Dialog);
            if (ok && !password.isEmpty())
            {
                setFormEnabled(false);
                progress = new Progress;
                connect(this, SIGNAL(cancel()), this, SLOT(finished()));
                connect(progress, SIGNAL(signalFinished()), this, SLOT(finished()));
                connect(this, SIGNAL(setOperationDescription(QString)), progress, SIGNAL(setOperationDescription(QString)));
                disconnect(frmodel, SIGNAL(requestContinuePrint(int *)), this, SLOT(confirmRequest(int *)));
                disconnect(eklsmodel, SIGNAL(requestContinuePrint(int *)), this, SLOT(confirmRequest(int *)));
                progress->run(this, &MainWindow::fastFiscalization, password);
            }
        }
        else
            QMessageBox::critical(this, conv("Ошибка"), conv("Не совпадают введенный пароль и подтверждение"));
    }
}

void MainWindow::setFormEnabled(bool enabled)
{
    tabControl->setEnabled(enabled);
    btnSetDate->setEnabled(enabled);
    btnCopyDate->setEnabled(enabled);
    edtKKMDate->setEnabled(enabled);
    edtKKMTime->setEnabled(enabled);
}

void MainWindow::finished()
{
    setFormEnabled(true);
    connect(frmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));
    connect(eklsmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));
    disconnect(this, SIGNAL(cancel()), this, SLOT(finished()));
    disconnect(progress, SIGNAL(signalFinished()), this, SLOT(finished()));
    disconnect(this, SIGNAL(setOperationDescription(QString)), progress, SIGNAL(setOperationDescription(QString)));
    delete progress;
}

void MainWindow::slotShowCalendar()
{
    QObject * object = QObject::sender();
    if (QPushButton * button = qobject_cast<QPushButton *>(object))
        sender = button->objectName();

    calendar->setSelectedDate(QDate::currentDate());
    calendar->show();
}

void MainWindow::slotSelectDate(const QDate &date)
{
    if (sender == "btnFRReportBegin")
        edtFRReportBegin->setText(date.toString("dd.MM.yyyy"));
    else if (sender == "btnFRReportEnd")
        edtFRReportEnd->setText(date.toString("dd.MM.yyyy"));
    else if (sender == "btnEKLSReportBegin")
        edtEKLSReportBegin->setText(date.toString("dd.MM.yyyy"));
    else if (sender == "btnEKLSReportEnd")
        edtEKLSReportEnd->setText(date.toString("dd.MM.yyyy"));
    else if (sender == "btnExtendedReportBegin")
    {
        edtExtendedReportBegin->setText(date.toString("dd.MM.yyyy"));
        selectionChanged();
    }
    else if (sender == "btnExtendedReportEnd")
    {
        edtExtendedReportEnd->setText(date.toString("dd.MM.yyyy"));
        selectionChanged();
    }
}

void MainWindow::slotActivation()
{
    int result = QMessageBox::warning(this, conv("Внимание"),
                                      conv("Вы хотите провести активизацию ЭКЛЗ?"), conv("Продолжить"), conv("Отмена"));
    if (result == 0)
        eklsmodel->activation();
}

void MainWindow::slotCloseArchive()
{
    int result = QMessageBox::warning(this, conv("Внимание"), conv("Вы хотите закрыть архив ЭКЛЗ? ") +
                                                                           conv("После этой команды на данном блоке ЭКЛЗ будут ") +
                                                                           conv("доступны только команды отчётов ЭКЛЗ."),  conv("Продолжить"), conv("Отмена"));
    if (result == 0)
        eklsmodel->closeArchive();
}

void MainWindow::slotActivationResult()
{
    eklsmodel->activationResult();
}

void MainWindow::slotPrintShift()
{
    if (!edtShiftNumber->text().isNull())
        eklsmodel->printShift(edtShiftNumber->text().toInt());
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Не введен номер смены"));
}

void MainWindow::slotPrintShiftFromEKLS()
{
    if (!edtShiftNumber->text().isNull())
        eklsmodel->printShiftFromEKLS(edtShiftNumber->text().toInt());
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Не введен номер смены"));
}

void MainWindow::slotPrintByNumber()
{
    if (!edtDocumentNumber->text().isNull())
        eklsmodel->printByNumber(edtDocumentNumber->text());
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Не введен криптографический первичный код документа"));
}

int MainWindow::makeReportsByDate(Model * model, int type, const QDate * dateBegin, const QDate * dateEnd, QString password)
{
    int result = 0;
    if (* dateEnd >= * dateBegin)
    {
        if (type == 0)
            result = model->makeBriefReport(dateBegin->day(), dateBegin->month(), abs(dateBegin->year() - 2000),
                                            dateEnd->day(), dateEnd->month(), abs(dateEnd->year() - 2000), password);
        else
            result = model->makeFullReport(dateBegin->day(), dateBegin->month(), abs(dateBegin->year() - 2000),
                                           dateEnd->day(), dateEnd->month(), abs(dateEnd->year() - 2000), password);
    }
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Дата начала позднее чем дата окончания"));

    return result;
}

int MainWindow::makeReportsBySession(Model * model, int type, int sessionBegin, int sessionEnd, QString password)
{
    int result = 0;
    if (sessionEnd >= sessionBegin)
    {
        if (type == 0)
            result = model->makeBriefReport(sessionBegin, sessionEnd, password);
        else
            result = model->makeFullReport(sessionBegin, sessionEnd, password);
    }
    else
        QMessageBox::critical(this, conv("Ошибка"), conv("Стартовая сессия больше чем последняя смена"));

    return result;
}

void MainWindow::slotEKLSMakeReport()
{
    int index = comboEKLSReportType->currentIndex();
    if (index < 2)
    {
        QDate dateBegin = QDate::fromString(edtEKLSReportBegin->text(), "dd.MM.yyyy");
        QDate dateEnd = QDate::fromString(edtEKLSReportEnd->text(), "dd.MM.yyyy");

        makeReportsByDate(eklsmodel, index, &dateBegin, &dateEnd);
    }
    else
    {
        int sessionBegin = edtEKLSReportBegin->text().toInt();
        int sessionEnd = edtEKLSReportEnd->text().toInt();

        makeReportsBySession(eklsmodel, index % 2, sessionBegin, sessionEnd);
    }
}

void MainWindow::slotEKLSBreakPrint()
{
    eklsmodel->breakPrint();
}

void MainWindow::deviceSelected(const ConnectOptions * options)
{
    searchWindow->hide();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    frmodel->connect(options);
    if (frmodel->checkConnection())
    {
        updateInputFields();
        frmodel->updateInformation();
        show();
    }
    else
        qApp->exit();

    QApplication::restoreOverrideCursor();
}

int quarter(int month)
{
    return (month - 1) / 3 + 1;
}

void MainWindow::selectionChanged()
{
    tableRange->clearContents();
    tableRange->setRowCount(0);

    for (int i = 0; i < selectedDateRange->count(); ++i)
        if (selectedDateRange->item(i)->checkState() == Qt::Checked)
        {
            QDate dateBegin = QDate::fromString(edtExtendedReportBegin->text(), "dd.MM.yyyy");
            QDate dateEnd   = QDate::fromString(edtExtendedReportEnd->text(), "dd.MM.yyyy");
            QString type    = selectedDateRange->item(i)->text();
            QList<Value<QDate> *> dates;

            if (dateEnd >= dateBegin)
            {
                switch (i)
                {
                case 0:
                    {
                        int yearsRange = dateEnd.year() - dateBegin.year();
                        int monthsRange = (12 - dateBegin.month()) + 12 * (yearsRange - 1) + dateEnd.month();

                        if (monthsRange != 0)
                        {
                            for (int i = 0; i <= monthsRange; ++i)
                            {
                                QDate begin, end;
                                if (i == 0)
                                {
                                    begin = dateBegin;
                                    end   = QDate(dateBegin.year(), dateBegin.month(), 1).addMonths(1).addDays(- 1);
                                }
                                else if (i == monthsRange)
                                {
                                    begin = QDate(dateBegin.year(), dateBegin.month(), 1).addMonths(i);
                                    end   = dateEnd;
                                }
                                else
                                {
                                    begin = QDate(dateBegin.year(), dateBegin.month(), 1).addMonths(i);
                                    end   = QDate(dateBegin.year(), dateBegin.month(), 1).addMonths(i + 1).addDays(- 1);
                                }
                                dates.append(new Value<QDate>(begin, end));
                            }
                        }
                        else
                            dates.append(new Value<QDate>(dateBegin, dateEnd));
                    }
                    break;

                case 1:
                    {
                        int yearsRange = dateEnd.year() - dateBegin.year();
                        int quat = quarter(dateBegin.month());
                        int quartersRange = (4 - quat) + 4 * (yearsRange - 1) + quarter(dateEnd.month());

                        if (quartersRange != 0)
                        {
                            for (int i = 0; i <= quartersRange; ++i)
                            {
                                QDate begin, end;
                                if (i == 0)
                                {
                                    begin = dateBegin;
                                    end   = QDate(dateBegin.year(), 3 * quat, 1).addMonths(1).addDays(- 1);
                                }
                                else if (i == quartersRange)
                                {
                                    begin = QDate(dateBegin.year(), 3 * quat, 1).addMonths(3 * (i - 1) + 1);
                                    end   = dateEnd;
                                }
                                else
                                {
                                    begin = QDate(dateBegin.year(), 3 * quat, 1).addMonths(3 * (i - 1) + 1);
                                    end   = QDate(dateBegin.year(), 3 * quat, 1).addMonths(3 * i + 1).addDays(- 1);
                                }
                                dates.append(new Value<QDate>(begin, end));
                            }
                        }
                        else
                            dates.append(new Value<QDate>(dateBegin, dateEnd));
                    }
                    break;

                case 2:
                    {
                        int yearsRange = dateEnd.year() - dateBegin.year();
                        if (yearsRange != 0)
                        {
                            for (int i = 0; i <= yearsRange; ++i)
                            {
                                QDate begin, end;
                                if (i == 0)
                                {
                                    begin = dateBegin;
                                    end   = QDate(dateBegin.year(), 1, 1).addYears(1).addDays(- 1);
                                }
                                else if (i == yearsRange)
                                {
                                    begin = QDate(dateBegin.year(), 1, 1).addYears(i);
                                    end   = dateEnd;
                                }
                                else
                                {
                                    begin = QDate(dateBegin.year(), 1, 1).addYears(i);
                                    end   = QDate(dateBegin.year(), 1, 1).addYears(i + 1).addDays(- 1);
                                }
                                dates.append(new Value<QDate>(begin, end));
                            }
                        }
                        else
                            dates.append(new Value<QDate>(dateBegin, dateEnd));
                    }
                    break;

                case 3:
                    dates.append(new Value<QDate>(dateBegin, dateEnd));
                    break;

                }

                foreach (Value<QDate> * date, dates)
                {
                    addDateRangeToTable(date->first().toString("dd.MM.yyyy") + '-' + date->second().toString("dd.MM.yyyy"), type);
                }
            }
            else
                QMessageBox::critical(this, conv("Ошибка"), conv("Дата начала позднее чем дата окончания"));
        }
}

void MainWindow::addDateRangeToTable(const QString &range, const QString &type)
{
    int rowCount = tableRange->rowCount();
    tableRange->setRowCount(rowCount + 1);
    QTableWidgetItem * item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    tableRange->setItem(rowCount, 0, item);
    item = new QTableWidgetItem(range);
    item->setFlags(Qt::ItemIsEnabled);
    tableRange->setItem(rowCount, 1, item);
    item = new QTableWidgetItem(type);
    item->setFlags(Qt::ItemIsEnabled);
    tableRange->setItem(rowCount, 2, item);
    tableRange->resizeColumnsToContents();
}

void MainWindow::printReports()
{
    if (QMessageBox::information(this, conv("Внимание"), conv("Внимание! ККМ может быть ЗАБЛОКИРОВАНА ") +
                                 conv("при вводе неправильного пароля налогового инспектора"), conv("Продолжить"), conv("Отмена")) == 0)
    {
        bool ok;
        QString password = QInputDialog::getText(this, conv("Пароль налогового инспектора"), conv("Введите пароль налогового инспектора"),
                                                 QLineEdit::Password, "", &ok, Qt::Dialog);

        if (ok && !password.isEmpty())
        {
            cancelFlag = false;
            setPrintEnabled(false);
            progress = new Progress;
            connect(this, SIGNAL(cancel()), this, SLOT(printFinished()));
            connect(progress, SIGNAL(signalFinished()), this, SLOT(printFinished()));
            connect(this, SIGNAL(setOperationDescription(QString)), progress, SIGNAL(setOperationDescription(QString)));
            disconnect(frmodel, SIGNAL(requestContinuePrint(int *)), this, SLOT(confirmRequest(int *)));
            disconnect(eklsmodel, SIGNAL(requestContinuePrint(int *)), this, SLOT(confirmRequest(int *)));
            progress->run(this, &MainWindow::print, password);
        }
    }
}

void MainWindow::print(const QString &password)
{
    int rowCount = tableRange->rowCount();
    int type = comboExtendedReportType->currentIndex();
    for (int i = 0; i < rowCount && !cancelFlag; ++i)
    {
        sleep(1);
        if (tableRange->item(i, 0)->checkState() == Qt::Checked)
        {
            QString str = tableRange->item(i, 1)->text();
            QStringList list = str.split('-');
            QDate dateBegin = QDate::fromString(list.at(0), "dd.MM.yyyy");
            QDate dateEnd   = QDate::fromString(list.at(1), "dd.MM.yyyy");

            emit setOperationDescription(conv("Диапазон дат: ") + str);

            int result = makeReportsByDate(frmodel, type, &dateBegin, &dateEnd, password);
            frmodel->waitForRelease();
            if (!cancelFlag)
                cancelFlag = result;

            if (!cancelFlag)
                emit currrentReportPrinted(i);
        }
    }
}

void MainWindow::reportPrinted(int row)
{
    tableRange->item(row, 0)->setCheckState(Qt::Unchecked);
}

void MainWindow::breakPrintReports()
{
    cancelFlag = true;
}

void MainWindow::setPrintEnabled(bool enabled)
{
    btnSetDate->setEnabled(enabled);
    btnCopyDate->setEnabled(enabled);
    edtKKMDate->setEnabled(enabled);
    edtKKMTime->setEnabled(enabled);
    frameEKLS->setEnabled(enabled);
    frameFP->setEnabled(enabled);
    edtExtendedReportBegin->setEnabled(enabled);
    edtExtendedReportEnd->setEnabled(enabled);
    btnExtendedReportBegin->setEnabled(enabled);
    btnExtendedReportEnd->setEnabled(enabled);
    btnPrintReports->setEnabled(enabled);
    tableRange->setEnabled(enabled);
    selectedDateRange->setEnabled(enabled);
}

void MainWindow::printFinished()
{
    setPrintEnabled(true);
    connect(frmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));
    connect(eklsmodel, SIGNAL(requestContinuePrint(int *)), SLOT(confirmRequest(int *)));
    disconnect(this, SIGNAL(cancel()), this, SLOT(printFinished()));
    disconnect(this, SIGNAL(setOperationDescription(QString)), progress, SIGNAL(setOperationDescription(QString)));
    disconnect(progress, SIGNAL(signalFinished()), this, SLOT(printFinished()));
    delete progress;
}
