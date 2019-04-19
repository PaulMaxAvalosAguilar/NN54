#include "devices.h"
#include <QDebug>

Devices::Devices():
    QObject(),
    agent(std::unique_ptr<QBluetoothDeviceDiscoveryAgent>(new QBluetoothDeviceDiscoveryAgent))
{
    connect(agent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Devices::deviceDiscovered);
    qDebug()<<"New Devices object created";
}

Devices::~Devices()
{
    qDebug()<<"Devices deleted";
}

void Devices::startScan()
{
    if(!(deviceList.empty()))
    {
        qDebug()<<"deviceList is not empty";
        clearDeviceList();
    }
    agent.get()->start();
    qDebug()<<"Scan is started from Devices class";
}

void Devices::stopScan()
{
    agent.get()->stop();
    qDebug()<<"Scan stopped";
}

void Devices::clearDeviceList()
{
    deviceList.clear();
    qDebug()<<"deviceList cleared";
}

void Devices::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    std::shared_ptr<Device> newDevice(new Device);
    qDebug()<<"New shared_ptr created for device with name "<<device.name()
           <<"and address: "<<device.address();
    newDevice.get()->setName(device.name());
    newDevice.get()->setAddress(device.address().toString());
    deviceList.push_back(newDevice);

}

std::vector<std::shared_ptr<Device> > &Devices::getDeviceList()
{
    return deviceList;
}




