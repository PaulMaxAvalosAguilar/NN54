#include "Connection/Serial/serialconnection.h"
#include <QSerialPortInfo>
#include <QDebug>

SerialConnection::SerialConnection()
    :
      serial(std::unique_ptr<QSerialPort>(new QSerialPort)),
      Connection()
{
    QObject::connect(serial.get(),
                     SIGNAL(error(QSerialPort::SerialPortError)),
                     this, SLOT(handleError(QSerialPort::SerialPortError)));
    QObject::connect(serial.get(), &QSerialPort::readyRead,
                     this, &SerialConnection::receiveDeviceInfo);
    qDebug()<<"New static SerialConnection Object created";
}


SerialConnection *SerialConnection::getInstance()
{
    static SerialConnection connection;
    return &connection;
}

SerialConnection::~SerialConnection()
{
    qDebug()<<"Serial Connection Deleted";
}

void SerialConnection::connect()
{
    qDebug()<<"connect() method called";
    openSerialPort();
}

void SerialConnection::disconnect()
{
    qDebug()<<"disconnect() method called";
    closeSerialPort();
}


void SerialConnection::openSerialPort()
{

    QString name;

    const auto serialPortInfos = QSerialPortInfo::availablePorts();


    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        name = serialPortInfo.portName();
    }

    serial.get()->setPortName(name);
    serial.get()->setBaudRate(QSerialPort::Baud9600);
    serial.get()->setDataBits(QSerialPort::Data8);
    serial.get()->setParity(QSerialPort::NoParity);
    serial.get()->setStopBits(QSerialPort::OneStop);
    serial.get()->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        qDebug()<<"Serial port Opened...."<<name;
    } else {
        qDebug()<<"Serial port NOT Opened...."<<serial.get()->errorString();
    }
}

void SerialConnection::closeSerialPort()
{
    if(serial.get()->isOpen())
    {
        qDebug()<<"Closed Serial Port";
        serial.get()->close();
    }
}


void SerialConnection::receiveDeviceInfo()
{

    static int contador = 0;
    if (!serial.get())
        return;

    while (serial->canReadLine()) {
        line = serial->readLine();
        split;
        text = QString::fromStdString(line.toStdString());
        split = text.split(",");
        parsedDATA = split.at(0);
        numero = parsedDATA.toInt();
        parsedDATA = split.at(1);
        intervalo = parsedDATA.toInt();


        emit dataReceived(numero, intervalo);

        qDebug()<<++contador<<numero <<" "<<intervalo;

    }
}


void SerialConnection::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug()<<"Error handled emmited signal";
        emit resourceError(error);
        disconnect();
    }
}







