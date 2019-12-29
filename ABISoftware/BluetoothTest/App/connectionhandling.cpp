#include "connectionhandling.h"
#include <QDebug>
#include <QLowEnergyController>

enum messageType{Start=1, Stop=2, ADC=3};

ConnectionHandling::ConnectionHandling(QObject *parent):
    QObject(parent),
    connected(false),
    connecting(false),
    devicename(""),
    deviceaddress(""),
    computedValue(),
    encService("11223344-5566-7788-9900-AABBCCDDEEFF"),
    encCharacteristic("01020304-0506-0708-0900-0A0B0C0D0FFF"),
    controller(),
    foundBatteryService(false),
    encoderService(),
    encoderCharacteristic(),
    batteryDescriptor()
{
}

ConnectionHandling::~ConnectionHandling()
{
    controller->disconnectFromDevice();
}

void ConnectionHandling::connectToDevice(QBluetoothDeviceInfo device, QString name, QString address)
{
    qDebug()<<"Connecting to "<<name<<" with address: "<<address;
    setDevicename(name);
    setDeviceaddress(address);

    if(!device.isValid()){
        qDebug()<<"Device is not valid";
        return;
    }

    if(controller){
        qDebug()<< "Disconnecting if connected";
        controller->disconnectFromDevice();
    }

    controller.reset(QLowEnergyController::createCentral(device));

    connect(controller.get(), &QLowEnergyController::connected,
            this, &ConnectionHandling::deviceConnected);
    connect(controller.get(), QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, &ConnectionHandling::errorReceived);
    connect(controller.get(), &QLowEnergyController::disconnected,
            this, &ConnectionHandling::deviceDisconnected);
    connect(controller.get(), &QLowEnergyController::serviceDiscovered,
            this, &ConnectionHandling::addLowEnergyService);
    connect(controller.get(), &QLowEnergyController::discoveryFinished,
            this, &ConnectionHandling::serviceScanDone);

    controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    setConnecting(true);
    controller->connectToDevice();
}

int ConnectionHandling::getConnected() const
{
    return connected;
}

void ConnectionHandling::setConnected(int value)
{
    connected = value;
    emit connectedChanged();
}

int ConnectionHandling::getConnecting() const
{
    return connecting;
}

void ConnectionHandling::setConnecting(int value)
{

    connecting = value;
    emit connectingChanged();
}

QString ConnectionHandling::getDevicename() const
{
    return devicename;
}

void ConnectionHandling::setDevicename(const QString &value)
{
    devicename = value;
    devicenameChanged();
}

QString ConnectionHandling::getDeviceaddress() const
{
    return deviceaddress;
}

void ConnectionHandling::setDeviceaddress(const QString &value)
{
    deviceaddress = value;
    deviceadressChanged();
}

QPoint ConnectionHandling::getComputedValue() const
{
    return computedValue;
}

void ConnectionHandling::setComputedValue(const int y)
{
    computedValue.setX(computedValue.x()+1);
    computedValue.setY(y);
    emit computedValueChanged();
}

void ConnectionHandling::disconnect()
{
    qDebug()<< "Disconnecting from device";
    controller->disconnectFromDevice();
}

void ConnectionHandling::deviceConnected()
{
    qDebug()<<"Device is connected";
    setConnecting(false);
    setConnected(true);
    controller->discoverServices();
}

void ConnectionHandling::deviceDisconnected()
{
    qDebug()<<"Device is disconnected";
    setConnecting(false);
    setConnected(false);    
    emit controllerDisconnected();
}

void ConnectionHandling::addLowEnergyService(const QBluetoothUuid &uuid)
{
    if (uuid == QBluetoothUuid(encService)) {
        qDebug()<<"Encoder service discovered.";
        foundBatteryService = true;
    }
}

void ConnectionHandling::errorReceived(QLowEnergyController::Error)
{

}

void ConnectionHandling::serviceScanDone()
{
    if(foundBatteryService == true){
        encoderService.reset(
                    controller->createServiceObject(
                        QBluetoothUuid(encService)));
    }

    //Checking whether service creation was successful or not
    if(encoderService.get() != nullptr){
        connect(encoderService.get(), &QLowEnergyService::stateChanged,
                this, &ConnectionHandling::serviceStateChanged);
        connect(encoderService.get(), &QLowEnergyService::characteristicChanged,
                this, &ConnectionHandling::updateHeartRateValue);
        connect(encoderService.get(), &QLowEnergyService::descriptorWritten,
                this, &ConnectionHandling::confirmedDescriptorWritten);
        qDebug()<<"Encoder service created.";
        encoderService->discoverDetails();
    }else{
        qDebug()<<"Encoder service not created.";
    }
}

void ConnectionHandling::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug()<<"Discovering details!";
        break;
    case QLowEnergyService::ServiceDiscovered:
    {

        encoderCharacteristic = encoderService->characteristic(QBluetoothUuid(encCharacteristic));
        if (!encoderCharacteristic.isValid()) {
            qDebug()<<"Encoder characteristic not created";
            break;
        }

        batteryDescriptor =
                encoderCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (encoderCharacteristic.isValid())
            qDebug()<<"Activating notifications";
            encoderService->writeDescriptor(batteryDescriptor, QByteArray::fromHex("0100"));

        break;
    }
    default:
        //nothing for now
        break;
    }
}

void ConnectionHandling::updateHeartRateValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    qDebug()<< "ValueChanged";
    if (c.uuid() == encoderCharacteristic.uuid()){

        QString hexValue = value.toHex();
        qDebug()<<hexValue.toInt(nullptr,16);

/*
        char c = Start;

        QByteArray a;
        a.append(c);

        encoderService->writeCharacteristic(encoderCharacteristic, a);
*/
    }
}

void ConnectionHandling::confirmedDescriptorWritten(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == batteryDescriptor && value == QByteArray::fromHex("0100")) {
        qDebug() <<"Notifications activated";
    }
}
