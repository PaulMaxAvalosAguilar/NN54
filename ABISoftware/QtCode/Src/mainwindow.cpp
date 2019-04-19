#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    count(0)
{
    ui->setupUi(this);

    //Enter the number you desire

    int decition = 2;

    if(decition == 1)
    {
        SerialConnection::getInstance()->connect();

    }else if(decition == 2)
    {

        /*
        QString addressToConnect = "98:D3:31:FC:64:DE";

        BluetoothConnection::getInstance();
        BluetoothConnection::getInstance()->setDeviceAddress(addressToConnect);
        BluetoothConnection::getInstance()->connect();

        BluetoothConnection::getInstance();
        BluetoothConnection::getInstance()->setDeviceAddress(addressToConnect);
        BluetoothConnection::getInstance()->connect();
        */




        ui->centralWidget->layout()->addWidget(&sc);
        sc.startScan();



    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataReceived(int counter, int time_interval)
{
    ++count;


    qDebug()<<counter<<" -- "<<time_interval;

}



