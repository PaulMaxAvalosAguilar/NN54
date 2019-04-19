#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <qbluetoothdeviceinfo.h>
#include <qbluetoothaddress.h>
#include "deviceinfo.h"

class DeviceInfo
{

public:
    DeviceInfo();
    DeviceInfo(const QBluetoothDeviceInfo &d);
    QString getName() const;
    QString getAddress() const;
    QString getRssi() const;
    QBluetoothDeviceInfo getDevice();
    void setDevice(const QBluetoothDeviceInfo &dev);

private:
    QBluetoothDeviceInfo device;
};

#endif // DEVICEINFO_H
