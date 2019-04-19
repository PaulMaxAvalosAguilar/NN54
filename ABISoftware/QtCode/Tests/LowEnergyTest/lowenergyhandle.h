#ifndef LOWENERGYHANDLE_H
#define LOWENERGYHANDLE_H

#include <QObject>
#include <memory>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QString>

class LowEnergyHandle : public QObject
{
    Q_OBJECT
public:
    LowEnergyHandle();
    void startScan();

    QString getAddress_toFind() const;
    void setAddress_toFind(const QString &value);

signals:
    void foundDevice();
public slots:
    void addDevice(const QBluetoothDeviceInfo &device);
    void connectToDevice();
    void serviceDiscovered(const QBluetoothUuid &gatt);

private:
    QString address_toFind;
    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> deviceAgent;
    std::unique_ptr<QBluetoothDeviceInfo> LEdevice;
};

#endif // LOWENERGYHANDLE_H
