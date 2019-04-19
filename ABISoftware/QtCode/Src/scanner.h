#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include "Connection/Bluetooth/Classic/devices.h"
#include <QtCore>
#include <memory>
#include "Connection/Bluetooth/Classic/device.h"
#include <vector>
#include <QString>
#include <QListWidgetItem>
#include <QThread>

namespace Ui {
class Scanner;
}



class Scanner : public QWidget
{
    Q_OBJECT

public:
    explicit Scanner(QWidget *parent = 0);
    ~Scanner();

    void startScan();
    void stopScan();

public slots:

    //Bluetooth signals slots
    void alreadyConnected();
    void disconnected();
    void connected();

    //Timer slots
    void finishedScanTime();
    void listDevicesScanned();

private slots:
    void on_scanButton_clicked();
    void onDoubleClicked(QListWidgetItem *item);

private:
    Ui::Scanner *ui;
    Devices devicesAgent;
    std::vector <std::shared_ptr<Device> > deviceList;
    QString textToAppend;
    QTimer *timer;
    QTimer *devicesTimer;
    bool scanRunning;



};





#endif // SCANNER_H
