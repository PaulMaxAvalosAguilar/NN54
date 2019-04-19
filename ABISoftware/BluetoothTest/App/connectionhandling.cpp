#include "connectionhandling.h"
#include <QDebug>
#include <QLowEnergyController>

ConnectionHandling::ConnectionHandling(QObject *parent):
    QObject(parent),
    connected(false),
    connecting(false),
    devicename(""),
    deviceaddress(""),
    computedValue(),
    controller(),
    foundBatteryService(false),
    batteryService(),
    battLevelCharacteristic(),
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
    if (uuid == QBluetoothUuid(QBluetoothUuid::BatteryService)) {
        qDebug()<<"Battery service discovered.";
        foundBatteryService = true;
    }
}

void ConnectionHandling::errorReceived(QLowEnergyController::Error)
{

}

void ConnectionHandling::serviceScanDone()
{
    if(foundBatteryService == true){
        batteryService.reset(
                    controller->createServiceObject(
                        QBluetoothUuid(QBluetoothUuid::BatteryService)));
    }

    //Checking whether service creation was successful or not
    if(batteryService.get() != nullptr){
        connect(batteryService.get(), &QLowEnergyService::stateChanged,
                this, &ConnectionHandling::serviceStateChanged);
        connect(batteryService.get(), &QLowEnergyService::characteristicChanged,
                this, &ConnectionHandling::updateHeartRateValue);
        connect(batteryService.get(), &QLowEnergyService::descriptorWritten,
                this, &ConnectionHandling::confirmedDescriptorWritten);
        batteryService->discoverDetails();
        qDebug()<<"Battery service created.";
    }else{
        qDebug()<<"Battery service not created.";
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

        battLevelCharacteristic = batteryService->characteristic(QBluetoothUuid(QBluetoothUuid::BatteryLevel));
        if (!battLevelCharacteristic.isValid()) {
            qDebug()<<"No battery level characteristic";
            break;
        }

        batteryDescriptor =
                battLevelCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (battLevelCharacteristic.isValid())
            qDebug()<<"Activating notifications";
            batteryService->writeDescriptor(batteryDescriptor, QByteArray::fromHex("0100"));

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
    if (c.uuid() == battLevelCharacteristic.uuid()){

        QString hexValue = value.toHex();

        qDebug()<<hexValue.toUInt(nullptr,16);

    }
}

void ConnectionHandling::confirmedDescriptorWritten(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == batteryDescriptor && value == QByteArray::fromHex("0100")) {
        qDebug() <<"Notifications activated";
    }
}
