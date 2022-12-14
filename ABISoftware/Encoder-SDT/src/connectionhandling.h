#ifndef CONNECTIONHANDLING_H
#define CONNECTIONHANDLING_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyDescriptor>
#include <QTimer>
#include <memory>
#include <string>
#include <QPoint>
#include <QTextToSpeech>
#include <QSoundEffect>

class ConnectionHandling : public QObject
{
    Q_OBJECT

    //QML Properties related
    Q_PROPERTY(int connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(int activated READ getActivated WRITE setActivated NOTIFY activatedChanged)
    Q_PROPERTY(int connecting READ getConnecting WRITE setConnecting NOTIFY connectingChanged)
    Q_PROPERTY(QString devicename READ getDevicename WRITE setDevicename NOTIFY devicenameChanged)
    Q_PROPERTY(QString deviceaddress READ getDeviceaddress
               WRITE setDeviceaddress NOTIFY deviceadressChanged)

    Q_PROPERTY(int encoderStartMessage READ getEncoderStartMessage WRITE setEncoderStartMessage NOTIFY encoderStartMessageChanged)

    Q_PROPERTY(uint traveledDistance READ getTraveledDist WRITE setTraveledDist NOTIFY traveledDistanceChanged)
    Q_PROPERTY(uint meanPropVel READ getMeanPropVel WRITE setMeanPropVel NOTIFY meanPropVelChanged)
    Q_PROPERTY(uint peakVel READ getPeakVel WRITE setPeakVel NOTIFY peakVelChanged)
    //QML Properties related*

public:
    explicit ConnectionHandling(QObject *parent= 0);
    ~ConnectionHandling();
    void connectToDevice(QBluetoothDeviceInfo device,
                         QString name, QString address);

    //QML Properties related
    int getConnected() const;
    void setConnected(int value);

    int getActivated() const;
    void setActivated(int value);

    int getConnecting() const;
    void setConnecting(int value);

    QString getDevicename() const;
    void setDevicename(const QString &value);

    QString getDeviceaddress() const;
    void setDeviceaddress(const QString &value);

    int getEncoderStartMessage() const;
    void setEncoderStartMessage(int value);

    uint getTraveledDist() const;
    void setTraveledDist(const uint &value);

    uint getMeanPropVel() const;
    void setMeanPropVel(const uint &value);

    uint getPeakVel() const;
    void setPeakVel(const uint &value);
    //QML Properties related*

signals:
    //QML Properties related
    void connectedChanged();
    void activatedChanged();
    void connectingChanged();
    void devicenameChanged();
    void deviceadressChanged();
    void encoderStartMessageChanged();
    void traveledDistanceChanged();
    void meanPropVelChanged();
    void peakVelChanged();
    //QML Properties related*

public slots:
    void disconnect();
    void sendStart(uint minDistToTravel, uint desiredCountDir,
                   uint desiredRepDir);
    void sendStop();
    void sendADC();
    void saySpeed(uint number);
    void sayCount(uint number);
    void soundeffect();

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

    void encodeTwoBytes(char *twoByteBuffer, uint numberToEncode);
    uint16_t decodeTwoBytes(uint8_t msb, uint8_t lsb);
    void encodeOneByte(char *oneByteBuffer, uint32_t numberToEncode);
    uint8_t decodeOneByte(uint8_t byte);


    //QML Properties related
    int connected;
    int activated;
    int connecting;
    QString devicename;
    QString deviceaddress;

    int encoderStartMessage;

    uint traveledDist;
    uint meanPropVel;
    uint peakVel;
    //QML Properties related*

    QString encService;
    QString encCharacteristic;
    std::unique_ptr<QLowEnergyController> controller;
    bool foundBatteryService;
    std::unique_ptr<QLowEnergyService> encoderService;
    QLowEnergyCharacteristic encoderCharacteristic;
    QLowEnergyDescriptor batteryDescriptor;

    //Timer Related
    std::unique_ptr<QTimer> timer;
    //Timer Related

    std::unique_ptr<QTextToSpeech> speech;
    std::unique_ptr<QSoundEffect> seffect;
};

#endif // CONNECTIONHANDLING_H
