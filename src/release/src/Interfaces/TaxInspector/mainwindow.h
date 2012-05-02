#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QWindowsStyle>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QListWidget>
#include <QSplitter>
#include <QStyle>
#include <QLabel>

#include "frmodel.h"
#include "eklsmodel.h"
#include "calendar.h"
#include "translator.h"
#include "progressbar.h"
#include "SearchDeviceApp/searchdevicewindow.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(const QString &name = "", QWidget * parent = 0);

    bool showWindow();

signals:
    void setOperationDescription(const QString &desc);
    void currrentReportPrinted(int row);
    void cancel();

protected:
    void closeEvent(QCloseEvent * event);
    void keyPressEvent(QKeyEvent * event);

private slots:
    void slotFillTable(const FRParam * param);
    void slotFail(const QString &text);
    void slotCopyDate();
    void slotSetDate();
    void slotFRCurrentTypeChanged(int index);
    void slotEKLSCurrentTypeChanged(int index);
    void slotSetSerialNumber();
    void slotFiscalization();
    void slotFRMakeReport();
    void slotTestSale();
    void slotMakeZReport();
    void slotFastFiscalization();
    void slotShowCalendar();
    void slotSelectDate(const QDate &date);
    void slotActivation();
    void slotCloseArchive();
    void slotActivationResult();
    void slotPrintShift();
    void slotPrintShiftFromEKLS();
    void slotPrintByNumber();
    void slotEKLSMakeReport();
    void slotEKLSBreakPrint();
    void finished();
    void deviceSelected(const ConnectOptions * options);
    void setFormEnabled(bool enabled);
    void setPrintEnabled(bool enabled);
    void selectionChanged();
    void printReports();
    void breakPrintReports();
    void reportPrinted(int row);
    void printFinished();
    void confirmRequest(int * result);

private:
    void init();
    void finalize();
    QFrame * addLine();
    void setCenterWindow();
    void updateInputFields();
    void fastFiscalization(const QString password);
    int makeReportsBySession(Model * model, int type, int sessionBegin, int sessionEnd,
                             QString password = QString::null);
    int makeReportsByDate(Model * model, int type, const QDate * dateBegin, const QDate * dateEnd,
                          QString password = QString::null);
    void addDateRangeToTable(const QString &range, const QString &type);
    void print(const QString &password);
    QList<QTableWidgetItem *> * itemList;
    QString sender;
    int protocol;
    bool license;
    bool cancelFlag;

    FRModel   * frmodel;
    EKLSModel * eklsmodel;
    Calendar  * calendar;
    Progress  * progress;
    SeachDeviceWindow * searchWindow;

    QTableWidget * table;
    QTabWidget   * tabControl;
    QFrame       * frameFP;
    QFrame       * frameEKLS;
    QFrame       * frameReports;
    QLabel       * lblPCDate,
                 * lblPCTime;
    QDateEdit    * edtKKMDate;
    QTimeEdit    * edtKKMTime;
    QPushButton  * btnCopyDate,
                 * btnSetDate;
    QPushButton  * btnActivation,
                 * btnCloseArchive,
                 * btnActivationResult;
    QLineEdit    * edtShiftNumber;
    QPushButton  * btnPrintShift,
                 * btnPrinthiftFromEKLS;
    QComboBox    * comboEKLSReportType;
    QLineEdit    * edtEKLSReportBegin,
                 * edtEKLSReportEnd;
    QPushButton  * btnEKLSReportBegin,
                 * btnEKLSReportEnd;
    QPushButton  * btnEKLSMakeReport,
                 * btnEKLSBreakPrint;
    QLineEdit    * edtDocumentNumber;
    QPushButton  * btnPrintByNumber;
    QPushButton  * btnSetSerialNumber;
    QLineEdit    * edtSerialNumber;
    QLabel       * lblSerialNumber;
    QFrame       * line;
    QLineEdit    * edtNewPassword,
                 * edtPasswordConfirm;
    QLineEdit    * edtRegistrationNumber,
                 * edtINN;
    QPushButton  * btnFiscalization;
    QPushButton  * btnFRMakeReport;
    QComboBox    * comboFRReportType;
    QLineEdit    * edtFRReportBegin,
                 * edtFRReportEnd;
    QPushButton  * btnFRReportBegin,
                 * btnFRReportEnd;
    QLineEdit    * edtTestSale;
    QPushButton  * btnTestSale;
    QPushButton  * btnMakeZReport;
    QPushButton  * btnFastFiscalization;
    QCheckBox    * chkEKLSFullFiscalization;
    QComboBox    * comboExtendedReportType;
    QLineEdit    * edtExtendedReportBegin,
                 * edtExtendedReportEnd;
    QPushButton  * btnExtendedReportBegin,
                 * btnExtendedReportEnd;
    QTableWidget * tableRange;
    QListWidget  * selectedDateRange;
    QPushButton  * btnPrintReports;
    QPushButton  * btnBreakPrintReports;

};

#endif // MAINWINDOW_H
