#include "connectionhandling.h"
#include <QDebug>
#include <QLowEnergyController>

ConnectionHandling::ConnectionHandling(QObject *parent):
    QObject(parent),
    connected(false),
    activated(false),
    connecting(false),
    devicename(""),
    deviceaddress(""),
    encoderStartMessage(),
    traveledDist(),
    meanPropVel(),
    peakVel(),
    encService("11223344-5566-7788-9900-AABBCCDDEEFF"),
    encCharacteristic("01020304-0506-0708-0900-0A0B0C0D0FFF"),
    controller(),
    foundBatteryService(false),
    encoderService(),
    encoderCharacteristic(),
    batteryDescriptor(),
    timer(new QTimer),
    speech(new QTextToSpeech),
    seffect(new QSoundEffect)

{
//    connect(timer.get(), &QTimer::timeout, this, &ConnectionHandling::sendADC);
    seffect->setSource(QUrl(QStringLiteral("qrc:/SoundEffects/fatigue-alert.wav")));
    seffect->setVolume(1);
}

ConnectionHandling::~ConnectionHandling()
{
    if(controller)
    controller->disconnectFromDevice();
}

void ConnectionHandling::connectToDevice(QBluetoothDeviceInfo device, QString name, QString address)
{
    qDebug()<<"Connecting to "<<name<<" with address: "<<address;
    setDevicename(name);
    setDeviceaddress(address);

    if(!device.isValid()){
        qDebug()<<"Device is not valid";
        return;
    }

    if(controller){
        qDebug()<< "Disconnecting if connected";
        controller->disconnectFromDevice();
    }

    controller.reset(QLowEnergyController::createCentral(device));

    connect(controller.get(), &QLowEnergyController::connected,
            this, &ConnectionHandling::deviceConnected);
    connect(controller.get(), QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, &ConnectionHandling::errorReceived);
    connect(controller.get(), &QLowEnergyController::disconnected,
            this, &ConnectionHandling::deviceDisconnected);
    connect(controller.get(), &QLowEnergyController::serviceDiscovered,
            this, &ConnectionHandling::addLowEnergyService);
    connect(controller.get(), &QLowEnergyController::discoveryFinished,
            this, &ConnectionHandling::serviceScanDone);

    controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    setConnecting(true);
    controller->connectToDevice();
}

int ConnectionHandling::getConnected() const
{
    return connected;
}

void ConnectionHandling::setConnected(int value)
{
    connected = value;
    emit connectedChanged();
}

int ConnectionHandling::getActivated() const
{
    return activated;
}

void ConnectionHandling::setActivated(int value)
{
    activated = value;
    emit activatedChanged();
}


int ConnectionHandling::getConnecting() const
{
    return connecting;
}

void ConnectionHandling::setConnecting(int value)
{

    connecting = value;
    emit connectingChanged();
}

QString ConnectionHandling::getDevicename() const
{
    return devicename;
}

void ConnectionHandling::setDevicename(const QString &value)
{
    devicename = value;
    emit devicenameChanged();
}

QString ConnectionHandling::getDeviceaddress() const
{
    return deviceaddress;
}

void ConnectionHandling::setDeviceaddress(const QString &value)
{
    deviceaddress = value;
    emit deviceadressChanged();
}

int ConnectionHandling::getEncoderStartMessage() const
{
    return encoderStartMessage;
}

void ConnectionHandling::setEncoderStartMessage(int value)
{
    encoderStartMessage = value;
    emit encoderStartMessageChanged();
}

uint ConnectionHandling::getTraveledDist() const
{
    return traveledDist;
}

void ConnectionHandling::setTraveledDist(const uint &value)
{
    traveledDist = value;
    emit traveledDistanceChanged();
}

uint ConnectionHandling::getMeanPropVel() const
{
    return meanPropVel;
}

void ConnectionHandling::setMeanPropVel(const uint &value)
{
    meanPropVel = value;
    emit meanPropVelChanged();
}

uint ConnectionHandling::getPeakVel() const
{
    return peakVel;
}

void ConnectionHandling::setPeakVel(const uint &value)
{
    peakVel = value;
    emit peakVelChanged();
}

void ConnectionHandling::disconnect()
{
    qDebug()<< "Disconnecting from device";

    controller->disconnectFromDevice();
}

void ConnectionHandling::sendADC()
{
    if(connected){
        char code = static_cast<char>(3);

        QByteArray c;
        c.append('|');
        c.append(code);

        c.append('|');
        encoderService->writeCharacteristic(encoderCharacteristic, c);
    }
}

void ConnectionHandling::sendStart(uint minDistToTravel, uint desiredCountDir, uint desiredRepDir)
{
    if(connected){
        char twoByteBuffer[3];
        encodeTwoBytes(twoByteBuffer,minDistToTravel);

        char code = static_cast<char>(1);

        QByteArray c;
        c.append('|');
        c.append(code);

        c.append(static_cast<char>(twoByteBuffer[0]));
        c.append(static_cast<char>(twoByteBuffer[1]));
        c.append(static_cast<char>(desiredCountDir+1));
        c.append(static_cast<char>(desiredRepDir+1));

        c.append('|');
        encoderService->writeCharacteristic(encoderCharacteristic, c);
        timer->stop();
        qDebug() << "Start Sended";
    }
}

void ConnectionHandling::sendStop()
{
    if(connected){
        char code = static_cast<char>(2);

        QByteArray c;
        c.append('|');
        c.append(code);

        c.append('|');
        encoderService->writeCharacteristic(encoderCharacteristic, c);
        timer->start();
        qDebug() << "Stop Sended";
    }
}

void ConnectionHandling::saySpeed(uint number)
{
    QString string;
    uint a = number/100;
    uint b = number - (a*100);

    string.append(QString::number(a));

    if(number != 0){
        string.append(" ");

        if(b < 10){
            string.append(" 0 ");
        }
        string.append(QString::number(b));
    }
    speech->say(string);
}

void ConnectionHandling::sayCount(uint number)
{

    speech->say(QString::number(number));
}

void ConnectionHandling::soundeffect()
{
    seffect->play();
}

void ConnectionHandling::deviceConnected()
{
    qDebug()<<"Device is connected";
    setConnecting(false);
    setConnected(true);
    controller->discoverServices();
}

void ConnectionHandling::deviceDisconnected()
{
    qDebug()<<"Device is disconnected";
    setConnecting(false);
    setConnected(false);
    setActivated(false);
    timer->stop();
}

void ConnectionHandling::addLowEnergyService(const QBluetoothUuid &uuid)
{
    if (uuid == QBluetoothUuid(encService)) {
        qDebug()<<"Encoder service discovered.";
        foundBatteryService = true;
    }
}

void ConnectionHandling::errorReceived(QLowEnergyController::Error)
{
    qDebug()<< "error";
}

void ConnectionHandling::serviceScanDone()
{
    if(foundBatteryService == true){
        encoderService.reset(
                    controller->createServiceObject(
                        QBluetoothUuid(encService)));
    }

    //Checking whether service creation was successful or not
    if(encoderService.get() != nullptr){
        connect(encoderService.get(), &QLowEnergyService::stateChanged,
                this, &ConnectionHandling::serviceStateChanged);
        connect(encoderService.get(), &QLowEnergyService::characteristicChanged,
                this, &ConnectionHandling::updateEncoderValue);
        connect(encoderService.get(), &QLowEnergyService::descriptorWritten,
                this, &ConnectionHandling::confirmedDescriptorWritten);
        qDebug()<<"Encoder service created.";
        QTimer::singleShot(0, [=] () {
                        encoderService->discoverDetails();
                    });
    }else{
        qDebug()<<"Encoder service not created.";
    }
}

void ConnectionHandling::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug()<<"Discovering details!";
        break;
    case QLowEnergyService::ServiceDiscovered:
    {

        encoderCharacteristic = encoderService->characteristic(QBluetoothUuid(encCharacteristic));
        if (!encoderCharacteristic.isValid()) {
            qDebug()<<"Encoder characteristic not created";
            break;
        }

        batteryDescriptor =
                encoderCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (batteryDescriptor.isValid()){
            qDebug()<<"Activating notifications";
            encoderService->writeDescriptor(batteryDescriptor, QByteArray::fromHex("0100"));
            timer->start(10000);
        }

        break;
    }
    default:
        //nothing for now
        break;
    }
}

void ConnectionHandling::updateEncoderValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    QString parsedValue;
    uint16_t uvalue;

    if (c.uuid() == encoderCharacteristic.uuid()){

        QString hexValue = value.toHex();
        qDebug() << hexValue;

        for(auto iterator = hexValue.begin(); iterator < hexValue.begin() + 2;
            iterator++){
            parsedValue.append(*iterator);
        }

        uvalue = parsedValue.toUInt(nullptr,16);
        parsedValue.clear();

        if(uvalue == 255){
            for(auto iterator = hexValue.begin()+2; iterator < hexValue.begin() + 6;
                iterator++){
                parsedValue.append(*iterator);
            }
            uvalue = parsedValue.toUInt(nullptr,16);
            parsedValue.clear();
            setTraveledDist(uvalue);

            for(auto iterator = hexValue.begin()+6; iterator < hexValue.begin() + 10;
                iterator++){
                parsedValue.append(*iterator);
            }
            uvalue = parsedValue.toUInt(nullptr,16);
            parsedValue.clear();
            setMeanPropVel(uvalue);

            for(auto iterator = hexValue.begin()+10; iterator < hexValue.begin() + 14;
                iterator++){
                parsedValue.append(*iterator);
            }
            uvalue = parsedValue.toUInt(nullptr,16);
            parsedValue.clear();
            setPeakVel(uvalue);
        }else if(uvalue == 200){
            setEncoderStartMessage(encoderStartMessage++);
        }
    }
}

void ConnectionHandling::confirmedDescriptorWritten(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == batteryDescriptor && value == QByteArray::fromHex("0100")) {
        qDebug() <<"Notifications activated";
        setActivated(true);
    }
}

void ConnectionHandling::encodeTwoBytes(char *twoByteBuffer, uint32_t numberToEncode){

    static uint8_t lowPart = 0;
    static uint8_t highPart = 0;

    lowPart = ((numberToEncode & 0x7F) << 1) | 1;
    highPart = (numberToEncode >>6) | 1;
    twoByteBuffer[0] = highPart;
    twoByteBuffer[1] = lowPart;
    twoByteBuffer[2] = 0;

}

uint16_t ConnectionHandling::decodeTwoBytes(uint8_t msb, uint8_t lsb){

  return (lsb>>1) | ((msb & 0xFE)<<6);
}




