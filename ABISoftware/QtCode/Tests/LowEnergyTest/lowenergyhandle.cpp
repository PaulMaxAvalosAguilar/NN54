#include "lowenergyhandle.h"
#include <QDebug>

LowEnergyHandle::LowEnergyHandle() :
    QObject(),
    address_toFind("00:1E:C0:41:F6:4A"),
    deviceAgent(new QBluetoothDeviceDiscoveryAgent(this)),
    LEdevice(new QBluetoothDeviceInfo())
{
    qDebug()<<"Creating Low Energy Hanle ";

    //SIGNALS AND SLOTS BEGIN
    connect(deviceAgent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,this, &LowEnergyHandle::addDevice);
    connect(deviceAgent.get(), &QBluetoothDeviceDiscoveryAgent::finished, this,[](){
        qDebug()<<"Scan was finished";
    });
    connect(deviceAgent.get(), &QBluetoothDeviceDiscoveryAgent::canceled, this,[](){
        qDebug()<<"Scan was canceled";
    });
    connect(this, SIGNAL(foundDevice()), this, SLOT(connectToDevice()));
    //SIGNALS AND SLOTS END
}

void LowEnergyHandle::startScan()
{
    qDebug()<<"LEbluetooth Device scan started";
    qDebug()<<"Searching for LEbluetooth Device with address "<<address_toFind;
    deviceAgent->setLowEnergyDiscoveryTimeout(5000);
    deviceAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

QString LowEnergyHandle::getAddress_toFind() const
{
    return address_toFind;
}

void LowEnergyHandle::setAddress_toFind(const QString &value)
{
    address_toFind = value;
}


void LowEnergyHandle::addDevice(const QBluetoothDeviceInfo &device)
{
    if(device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        if(device.address().toString() == address_toFind){
            qDebug()<<"LEbluetooth Device found with address"<<device.address().toString()
                           <<" and name"<<device.name()<<endl;
            LEdevice->operator =(device);
            deviceAgent->stop();
            emit foundDevice();
        }
    }
}

void LowEnergyHandle::connectToDevice()
{
    qDebug()<<"attempting to connect to Device";
    std::unique_ptr<QLowEnergyController>
            leController(new QLowEnergyController(*LEdevice.get(),this));


    //SIGNALS AND SLOTS BEGIN
    connect(leController.get(), &QLowEnergyController::connected, this, [this, &leController](){
       qDebug()<<"Connected to LEbluetooth device";
       leController->discoverServices();
    });

    connect(leController.get(), &QLowEnergyController::disconnected, this, [this]() {
        qDebug()<<"Disconnected from LEbluetooth Device";
    });

    connect(leController.get(), static_cast<void (QLowEnergyController::*)
            (QLowEnergyController::Error)>(&QLowEnergyController::error),
                this, [this]() {
            qDebug()<<"Cannot connect to LEbluetooth Device";
    });

    connect(leController.get(), &QLowEnergyController::serviceDiscovered, this, &LowEnergyHandle::serviceDiscovered);
    //SIGNALS AND SLOTS END

    leController->setRemoteAddressType(QLowEnergyController::PublicAddress);
    leController->connectToDevice();
}

void LowEnergyHandle::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if(gatt == QBluetoothUuid(QBluetoothUuid::HeartRate)){
        qDebug()<<"Found decired service";
    }
}

