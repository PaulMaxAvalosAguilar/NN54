#ifndef BLUETOOTHCONNECTION_H
#define BLUETOOTHCONNECTION_H

#include "Connection/connection.h"
#include <QtBluetooth>
#include <memory>



class BluetoothConnection : public Connection
{
    Q_OBJECT

public:

    static BluetoothConnection *getInstance();    
    ~BluetoothConnection();

    void  connect() override;
    void disconnect() override;


    void setDeviceAddress(QString &);
    QString &getDeviceAddress();

signals:
    void alreadyConnected();
    void connected();
    void disconnected();

private slots:
    void receiveDeviceInfo() override;

    void connectedDevice();
    void disconnectedDevice();
    void handleError(QBluetoothSocket::SocketError error);


private:
    //Functions
    BluetoothConnection();
    void initiateSocket();

    //Class Members
    std::shared_ptr<QBluetoothSocket> socket;
    QString deviceName;
    QString address;
    bool socketConnected;
    bool socketWaitingConnection;
    QString connecteDevice;

};


#endif // BLUETOOTHCONNECTION_H
