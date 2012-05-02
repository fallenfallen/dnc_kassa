#ifndef SEARCHDEVICEWINDOW_H
#define SEARCHDEVICEWINDOW_H

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

#include "searchdevice.h"
#include "searchprogressbar.h"

class SeachDeviceWindow : public QWidget
{
    Q_OBJECT

public:
    SeachDeviceWindow(QWidget *parent = 0);
    ~SeachDeviceWindow();

public slots:
    void show();

private slots:
    void startSearch();
    void currentIndexChanged(int index);
    void addFoundDevice(const QString &deviceName, const QString &port,
                        const QString &protocol, const QString &baudRate);
    void finished();
    void setEnabled(bool enabled);
    void cellDoubleClicked(int row, int column);

signals:
    void setValue(double value);
    void deviceSelected(const ConnectOptions * options);

private:
    void init();
    void setCenterWindow();

    SearchDevice      * searchEngine;
    SearchProgressBar * progressBar;
    SearchProgress    * searchProgress;
    QList<QTableWidgetItem *> * itemList;

    QComboBox    * comboDeviceType;
    QComboBox    * comboDevicePort;
    QComboBox    * comboDeviceBaudRate;
    QComboBox    * comboDeviceProtocol;
    QPushButton  * btnStartSearch;
    QGroupBox    * groupPassword;
    QLineEdit    * edtAccessPassword;
    QLineEdit    * edtAdminPassword;
    QTableWidget * deviceTable;

};

#endif // SEARCHDEVICEWINDOW_H
