#ifndef DEVICES_H
#define DEVICES_H

#include <QObject>
#include <QtBluetooth>
#include <memory>
#include <vector>
#include "device.h"

class Devices: public QObject
{

    Q_OBJECT
public:
    Devices();
    ~Devices();
    void startScan();
    void stopScan();

    void clearDeviceList();

    std::vector<std::shared_ptr<Device> > &getDeviceList();

public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> agent;
    std::vector <std::shared_ptr<Device> > deviceList;
};

#endif // DEVICES_H
