#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QObject>
#include "Connection/connection.h"
#include <QSerialPort>
#include <QString>
#include <memory>


class SerialConnection : public Connection
{

    Q_OBJECT

public:
    static SerialConnection *getInstance();
    ~SerialConnection();

    void connect() override;
    void disconnect() override;

signals:
    void resourceError(QSerialPort::SerialPortError error);

private slots:
    void receiveDeviceInfo() override;
    void handleError(QSerialPort::SerialPortError error);

private:
    //Functions
    SerialConnection();
    void openSerialPort();
    void closeSerialPort();

    //Class members
    std::unique_ptr<QSerialPort> serial;

};

#endif // SERIALCONNECTION_H
