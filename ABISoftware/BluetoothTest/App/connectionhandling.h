#ifndef CONNECTIONHANDLING_H
#define CONNECTIONHANDLING_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyDescriptor>
#include <memory>
#include <string>
#include <QPoint>

class ConnectionHandling : public QObject
{
    Q_OBJECT

    //QML Properties related
    Q_PROPERTY(int connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(int connecting READ getConnecting WRITE setConnecting NOTIFY connectingChanged)
    Q_PROPERTY(QString devicename READ getDevicename WRITE setDevicename NOTIFY devicenameChanged)
    Q_PROPERTY(QString deviceaddress READ getDeviceaddress
               WRITE setDeviceaddress NOTIFY deviceadressChanged)
    Q_PROPERTY(QPointF computedValue READ getComputedValue NOTIFY computedValueChanged)
    //QML Properties related*

public:
    explicit ConnectionHandling(QObject *parent= 0);
    ~ConnectionHandling();
    void connectToDevice(QBluetoothDeviceInfo device,
                         QString name, QString address);

    //QML Properties related
    int getConnected() const;
    void setConnected(int value);

    int getConnecting() const;
    void setConnecting(int value);

    QString getDevicename() const;
    void setDevicename(const QString &value);

    QString getDeviceaddress() const;
    void setDeviceaddress(const QString &value);

    QPoint getComputedValue() const;
    void setComputedValue(const int y);
    //QML Properties related*



signals:
    //QML Properties related
    void connectedChanged();
    void connectingChanged();
    void devicenameChanged();
    void deviceadressChanged();
    void computedValueChanged();
    //QML Properties related*

    void controllerDisconnected();

public slots:
    void disconnect();
    void startEncoder();

private slots:

    //QLowEnergyController related
    void deviceConnected();
    void deviceDisconnected();
    void addLowEnergyService(const QBluetoothUuid &uuid);
    void errorReceived(QLowEnergyController::Error);
    void serviceScanDone();

    //QLowEnergyService related
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateEncoderValue(const QLowEnergyCharacteristic &c,
                              const QByteArray &value);
    void confirmedDescriptorWritten(const QLowEnergyDescriptor &d,
                                  const QByteArray &value);

private:

    //QML Properties related
    int connected;
    int connecting;
    QString devicename;
    QString deviceaddress;
    QPoint computedValue;
    //QML Properties related*
    QString encService;
    QString encCharacteristic;
    std::unique_ptr<QLowEnergyController> controller;
    bool foundBatteryService;
    std::unique_ptr<QLowEnergyService> encoderService;
    QLowEnergyCharacteristic encoderCharacteristic;
    QLowEnergyDescriptor batteryDescriptor;
};

#endif // CONNECTIONHANDLING_H
