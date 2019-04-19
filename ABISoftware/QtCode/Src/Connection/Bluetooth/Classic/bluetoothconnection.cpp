#include "bluetoothconnection.h"
#include <QDebug>

BluetoothConnection::BluetoothConnection():

    socket(std::shared_ptr<QBluetoothSocket>(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol)))
{


    //socket connected device connection
    QObject::connect(socket.get(), SIGNAL(connected()),this, SLOT(connectedDevice()));
    //socket disconnected device connection
    QObject::connect(socket.get(), &QBluetoothSocket::disconnected,
                     this, &BluetoothConnection::disconnectedDevice);
    //socket error connection
    QObject::connect(socket.get(), SIGNAL(error(QBluetoothSocket::SocketError)),
                     this, SLOT(handleError(QBluetoothSocket::SocketError)));
    //socket read information
    QObject::connect(socket.get(), &QBluetoothSocket::readyRead,
                     this, &BluetoothConnection::receiveDeviceInfo);
    qDebug()<<"New static BluetoothConnection Object created";

}


BluetoothConnection *BluetoothConnection::getInstance()
{
    static BluetoothConnection connection;
    return &connection;
}

void BluetoothConnection::setDeviceAddress(QString &deviceA)
{
    address = deviceA;
}

QString &BluetoothConnection::getDeviceAddress()
{
    return address;
}

BluetoothConnection::~BluetoothConnection()
{
    qDebug()<<"BluetoothConnection deleted";
}

//Cases to examine:
/*
 * You cannot try to connect again to a device already connected because connect to service won't
 * respond
 *
 */
void BluetoothConnection::connect()
{



    if(socketConnected)
    {

        if(address !=connecteDevice){
            qDebug()<<"Device connected proceding to disconnect";
            socketWaitingConnection=true;
            socket.get()->abort();
        }else{

            qDebug()<<"Attempting to connect an already connected device";
            emit alreadyConnected();
        }

    }else{
        initiateSocket();
    }
}

void BluetoothConnection::disconnect()
{
    socket.get()->disconnectFromService();
}

void BluetoothConnection::receiveDeviceInfo()
{
    static int contador = 0;
    if (!socket.get())
        return;

    while (socket->canReadLine()) {
        line = socket->readLine();
        split;
        text = QString::fromStdString(line.toStdString());
        split = text.split(",");
        parsedDATA = split.at(0);
        numero = parsedDATA.toInt();
        parsedDATA = split.at(1);
        intervalo = parsedDATA.toInt();


        emit dataReceived(numero, intervalo);
        //internal debug
        /*
        qDebug()<<++contador<<numero <<" "<<intervalo;
        */
    }
}

void BluetoothConnection::connectedDevice()
{
    connecteDevice= address;
    qDebug()<<"Connected to device with address"<<address;
    socketConnected = true;
    emit connected();
}

void BluetoothConnection::disconnectedDevice()
{
    qDebug()<<"Disconnected from previous device or failed to connect";
    socketConnected = false;

    if(socketWaitingConnection)
    {
        socketWaitingConnection=false;
        connect();
    }else{
        connecteDevice= "";
        emit disconnected();
    }
}

void BluetoothConnection::handleError(QBluetoothSocket::SocketError error)
{


}


void BluetoothConnection::initiateSocket()
{
    qDebug()<<"Trying to initialize connection";

    socket.get()->connectToService(QBluetoothAddress(address),1,QIODevice::ReadWrite);
}





