#include "blescannermodel.h"
#include <QDebug>

void BleScannerModel::create(const QModelIndex &index, const DeviceInfo &device)
{
    qDebug()<<"create() calling: Index row:" << index.row();
    int row = index.row();
    if(row < 0 || row > rowCount() || !index.isValid()) {
        qDebug()<<"Index is not valid: Appending";
        row = rowCount();
    }

    beginInsertRows(QModelIndex(), row, row);
    std::unique_ptr<DeviceInfo> newDeviceInfo(new DeviceInfo(device));
    devicesInfo->push_back(std::move(newDeviceInfo));
    endInsertRows();
}

bool BleScannerModel::removeRows(const QModelIndex &index, int count, const QModelIndex &parent)
{
    int row = index.row();
    if(!isIndexValid(index) || (row + count) > rowCount()){
        return false;
    }
    beginRemoveRows(parent, row, row + count -1);
    devicesInfo->erase(devicesInfo->begin() + row,
                       devicesInfo->begin() +row + count);
    endRemoveRows();
    return true;
}

bool BleScannerModel::update(const QModelIndex &index, const qint16 &value)
{
    if (!isIndexValid(index)) {
        return false;
    }

    DeviceInfo &device = *devicesInfo->at(index.row());
    device.setRSSI(value);
    emit dataChanged(index,index);

    return true;
}



BleScannerModel::BleScannerModel(QObject *parent):
    QAbstractListModel(parent),
    bleAgent(new QBluetoothDeviceDiscoveryAgent),
    devicesInfo(new std::vector<std::unique_ptr<DeviceInfo>>()),
    connHandling(new ConnectionHandling),
    m_scannerState("Waiting scanning"),
    scanning(false)
{
    bleAgent->setLowEnergyDiscoveryTimeout(0);
    setScannerState("Start Scan");

    connect(bleAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleScannerModel::addDevice);
    connect(bleAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated,this, &BleScannerModel::updateDevice);
    connect(bleAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BleScannerModel::deviceScanError);
    connect(bleAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BleScannerModel::deviceScanFinished);
}

BleScannerModel::~BleScannerModel()
{
    delete bleAgent;
}

int BleScannerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return devicesInfo->size();
}

QVariant BleScannerModel::data(const QModelIndex &index, int role) const
{
    if(!isIndexValid(index)){
        return QVariant();
    }

    const DeviceInfo &deviceinfo = *devicesInfo->at(index.row());

    switch(role) {
    case BleNameRole:
        return deviceinfo.getName();
    case BleAdressRole:
        return deviceinfo.getAddress();
    case BleRssiRole:
        return deviceinfo.getRssi();
    }

    return QVariant();
}

QHash<int, QByteArray> BleScannerModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[Roles::BleNameRole] = "blename";
    roleNames[Roles::BleAdressRole] = "bleaddress";
    roleNames[Roles::BleRssiRole] = "blerssi";
    return roleNames;
}

bool BleScannerModel::isIndexValid(const QModelIndex &index) const
{
    if (index.row() < 0
            || index.row() >= rowCount()
            || !index.isValid()) {
        return false;
    }
    return true;
}

QString BleScannerModel::scannerState() const
{
    return m_scannerState;
}

void BleScannerModel::setScannerState(const QString &scannerState)
{
    m_scannerState = scannerState;
    emit scannerStateChanged();

}

bool BleScannerModel::getScanning() const
{
    return scanning;
}

void BleScannerModel::setScanning(bool value)
{
    scanning = value;
    emit scanningChanged();
}

std::unique_ptr<ConnectionHandling> &BleScannerModel::getConnHandling()
{
    return connHandling;
}

void BleScannerModel::append(const DeviceInfo &device)
{
    qDebug()<<"Calling appending";
    create(index(rowCount(),0),device);
}

void BleScannerModel::clear()
{
    emit beginResetModel();
    setScannerState("Start Scan");
    devicesInfo->clear();
    emit endResetModel();
}

void BleScannerModel::startDiscovery()
{
    qDebug()<<"\nStarted discovery";
    clear();
    setScannerState("Scan in progress");
    bleAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    setScanning(true);
}

void BleScannerModel::stopDiscovery()
{
    qDebug()<<"Stopped discovery";
    bleAgent->stop();

    setScannerState("Last scan list showing");

    qDebug()<<"Scan finished";
    setScanning(false);
}

void BleScannerModel::connectToDevice(int index)
{

    DeviceInfo &deviceinfo = *devicesInfo->at(index);
    if(deviceinfo.getDevice().isValid() &&
            !connHandling->getConnected()){
        connHandling->connectToDevice(deviceinfo.getDevice(),
                                      deviceinfo.getName(),
                                      deviceinfo.getAddress());
    }else{
        qDebug()<< "invalid device";
    }
}

void BleScannerModel::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        DeviceInfo d(info);
        if(d.getName() == "VBT"){


            for(int i = 0; i < devicesInfo->size(); i++){
                if((*devicesInfo->at(i)).getAddress() == info.address().toString()){
                    return;
                }
            }

            qDebug()<< "Added device "<<d.getAddress();
            append(d);
        }        
    }
}

void BleScannerModel::updateDevice(const QBluetoothDeviceInfo &info,
                                   QBluetoothDeviceInfo::Fields updatedFields)
{

    if(updatedFields & QBluetoothDeviceInfo::Field::RSSI){
        for(int i = 0; i < devicesInfo->size(); i++){
            if((*devicesInfo->at(i)).getAddress() == info.address().toString()){
                update(index(i, 0),info.rssi());
            }
        }
    }
}


void BleScannerModel::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setScannerState("Bluetooth adaptor off, power it on!");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setScannerState("Device communication resulted in an error.");
    else {
        setScannerState("Unknown error");
    }

    qDebug()<<"Scann finished with an error";
    setScanning(false);
}

void BleScannerModel::deviceScanFinished()
{

}


