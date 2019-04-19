#ifndef DEVICE_H
#define DEVICE_H

#include <QString>


class Device
{
public:
    Device();
    ~Device();

    QString getAddress() const;
    void setAddress(const QString &value);

    QString getName() const;
    void setName(const QString &value);

private:
    QString address;
    QString name;
};

#endif // DEVICE_H
