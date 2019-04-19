#include "device.h"
#include <QDebug>

Device::Device()
{
    qDebug()<<"New Device object created";
}

Device::~Device()
{
    qDebug()<<"Device deleted";
}

QString Device::getAddress() const
{
    return address;
}

void Device::setAddress(const QString &value)
{
    address = value;
}

QString Device::getName() const
{
    return name;
}

void Device::setName(const QString &value)
{
    name = value;
}
