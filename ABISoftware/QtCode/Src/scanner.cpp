#include "scanner.h"
#include "ui_scanner.h"
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include "Connection/Bluetooth/Classic/bluetoothconnection.h"
#include <iostream>
#include <string>

Scanner::Scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scanner),
    devicesAgent(),
    timer(new QTimer(this)),
    devicesTimer(new QTimer(this))
{
    ui->setupUi(this);

    BluetoothConnection::getInstance();

    qDebug()<<"New Scanner Object created";


    //Item selected connection
    connect(ui->deviceListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onDoubleClicked(QListWidgetItem*)));

    //Timers connections
    connect(timer, SIGNAL(timeout()), this,SLOT(finishedScanTime()));
    connect(devicesTimer, SIGNAL(timeout()), this, SLOT(listDevicesScanned()));

    //Bluetooth Signals connection


    connect(BluetoothConnection::getInstance(), SIGNAL(alreadyConnected()), this, SLOT(alreadyConnected()));
    connect(BluetoothConnection::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(BluetoothConnection::getInstance(), SIGNAL(connected()), this, SLOT(connected()));


}

Scanner::~Scanner()
{
    delete ui;
}

void Scanner::startScan()
{

    qDebug()<<"Started scan from Scanner";
    timer->start(10000);
    devicesTimer->start(1000);
    devicesAgent.startScan();
    ui->status->setText("Scanning....");
    scanRunning = true;
}

void Scanner::stopScan()
{
    qDebug()<<"Finished scan";
    ui->status->setText("Scanning finished");
    timer->stop();
    devicesTimer->stop();
    devicesAgent.stopScan();
    devicesAgent.clearDeviceList();
    scanRunning = false;
}

void Scanner::alreadyConnected()
{
    QMessageBox msgBox;
    msgBox.setText("Already connected to device");
    msgBox.exec();
    ui->status->setText("Connected");
}

void Scanner::disconnected()
{
    QMessageBox msgBox;
    msgBox.setText("Disconnected from device or failed to connect");
    msgBox.exec();
    ui->status->setText("Disconnected");
}

void Scanner::connected()
{
    QMessageBox msgBox;
    msgBox.setText("Connected to device ");
    msgBox.exec();
    ui->status->setText("Connected");
}



void Scanner::listDevicesScanned()
{
    deviceList = devicesAgent.getDeviceList();
    for(int i= 0; i< deviceList.size(); i++)
    {
        textToAppend = "";
        textToAppend += deviceList.at(i).get()->getName();
        textToAppend += ",";
        textToAppend += deviceList.at(i).get()->getAddress();
        ui->deviceListWidget->addItem(textToAppend);
    }
    devicesAgent.clearDeviceList();
}



void Scanner::finishedScanTime()
{
    stopScan();
}

void Scanner::on_scanButton_clicked()
{
    stopScan();
    ui->deviceListWidget->clear();
    startScan();
}

void Scanner::onDoubleClicked(QListWidgetItem *item)
{
    QString clickedItem = item->text();
    QStringList split = clickedItem.split(",");
    QString addressToConnect = split.at(1);


    qDebug()<<"Item double clicked "<<clickedItem;
    qDebug()<<"Address to connect"<<addressToConnect;
    QMessageBox msgBox;
    msgBox.setText("Attempting to connect to device with address "+addressToConnect);
    msgBox.exec();


    if(scanRunning)
    {
        qDebug()<<"Scan is stopped because of connection attempt";
        stopScan();
    }

    BluetoothConnection::getInstance()->setDeviceAddress(addressToConnect);
    BluetoothConnection::getInstance()->connect();
}





