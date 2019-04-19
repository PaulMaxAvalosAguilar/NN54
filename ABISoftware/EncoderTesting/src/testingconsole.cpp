#include "testingconsole.h"
#include "ui_testingconsole.h"
#include <QtMath>
#include <QDebug>
#include <iostream>

TestingConsole::TestingConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingConsole),
    m_timer(new QTimer(this)),
    encNumberofValues(10),
    encByteSize(2),
    encmeasuringRate(1),
    discToothNumber(10),
    discChangingStatesNumber(discToothNumber*2),
    discPerimeter(.120),
    encoderTimesWritten(0),
    encoderPosition(0),
    extractedValues()
{
    ui->setupUi(this);
    QString physicalPropertiesText;

    physicalPropertiesText+= "Tooth number: ";
    physicalPropertiesText+= QString::number(discToothNumber);
    ui->toothLabel->setText(physicalPropertiesText);

    physicalPropertiesText = "Turn(m): ";
    physicalPropertiesText += QString::number(discPerimeter);
    ui->turnmmLabel->setText(physicalPropertiesText);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateEncoderWrtitingCounter()));
}

TestingConsole::~TestingConsole()
{
    delete ui;
}

void TestingConsole::parseLine(const QString &line)
{
    ui->m_console->putData(line);

    QByteArray sendMessage = "";
    static int configurations = 0;
    static int advertisings = 0;
    if(line == "InitializingBluetooth\n"){
        configurations = 0;
        advertisings = 0;
        encoderTimesWritten = 0;
        ui->m_console->clear();

        sendMessage = "CMD\n";
        emit writeData(sendMessage);
        ui->m_console->putData("Testing initialized\n");


    }else if(line.contains("SF") ||
             line.contains("PZ") ||
             line.contains("SR") ||
             line.contains("SS") ||
             line.contains("PS") ||
             line.contains("PC") ||
             line.contains("SN") ||
             line.contains("LS")){
        configurations++;
        QString config;

        if(line == "LS\n"){
            sendMessage =
                    "180F\n"
                    "  2A19,0018,V\n"
                    "  2A19,0019,C\n"
                    "11223344556677889900AABBCCDDEEFF\n"
                    "  010203040506070809000A0B0C0D1FFF,001A,02,20\n"
                    "  010203040506070809000A0B0C0D0E0F,001B,10,20\n"
                    "  010203040506070809000A0B0C0D0E0F,001C,10,20\n"
                    "  111213141516171819101A1B1C1D1E1F,001D,02,20\n"
                    "END\n";
        }else{
            sendMessage = "AOK\n";
        }
        emit writeData(sendMessage);

        ui->m_console->putData(config.append("Configuration: ").append(
                               QString::number(configurations).append(
                                   "/10\n")));
        if(configurations == 10){
            ui->m_console->putData("ConfigurationTests: Passed\n");
        }


    }else if(line == "A\n"){
        advertisings++;

        sendMessage = "AOK\n";
        emit writeData(sendMessage);

        if(advertisings == 1){
            sendMessage = "Connected\n";
            emit writeData(sendMessage);
            sendMessage = "Connection End\n";
            emit writeData(sendMessage);
            //Start advertising
            sendMessage = "WC,001C,0100\n";
            emit writeData(sendMessage);

        }else{
            ui->m_console->putData("Connectivity Test: Passed\n");

        }

    }else if(line == "U\n"){
        sendMessage = "AOK\n";
        emit writeData(sendMessage);
    }else if(line.contains("SHW")){
        sendMessage = "AOK\n";
        emit writeData(sendMessage);
        if(line.contains("001B")){

            readEncoder(line, extractedValues);

            static int firstTimeTimer = 1;
            if(firstTimeTimer == 1){
                firstTimeTimer = 0;
                m_timer->start(1000);
            }
            encoderTimesWritten++;

            sendMessage = "AOK\n";
            emit writeData(sendMessage);

            QString outputData;
            for(uint i = 0; i < encNumberofValues; i ++){
                int num = extractedValues.at(i);
                encoderPosition = num;
                outputData.append((num >= 0)? "+":"-");
                outputData.append(QString::number((num >= 0)? num: -num));
                ui->posvalLabel->setText(outputData);
                outputData.clear();
            }
            extractedValues.clear();
        }
        /*
        }else if(line.contains("0018")){

            const int numberOfValues = 1;
            const int byteSize = 1;
            readNXBytesCharacteristic(line, numberOfValues, byteSize, signs,numbers);

            QString completeData;
            for(int i = 0; i < numberOfValues; i ++){
                completeData.append(signs.at(i));
                num = numbers.at(i).toUInt(nullptr,16);
                completeData.append(QString::number(num));
                completeData.append("%");

                ui->battvalLabel->setText(completeData);
            }
        }
       */
    }
}

Console *TestingConsole::getConsole()
{
    return ui->m_console;
}

void TestingConsole::readEncoder(const QString &line, std::deque<int> &numbers)
{
    const int numberOfValues = 10;
    const int valueBytesSize = 2;
    const uint16_t encoderStandardValue = 32768;
    auto encoderStringIterator = line.begin()+9;

    QString dataStringValue = "";
    int encoderPosition = 0;

    for(int i = 0; i < numberOfValues; i ++){

        for(auto iterator = encoderStringIterator; iterator < encoderStringIterator + (valueBytesSize*2);
            iterator++){
            dataStringValue.append(*iterator);
        }

        encoderPosition = dataStringValue.toInt(nullptr,16) - encoderStandardValue;

        calculateVelocity(encoderPosition);
        numbers.push_back(encoderPosition);

        encoderStringIterator += (valueBytesSize*2);
        dataStringValue.clear();
    }
}

void TestingConsole::calculateVelocity(int encoderPosition)
{
    //tracking variables
    static int lastEncoderPosition = 0;
    static double lastVelocity = 0;
    static int timeCount = 0;

    //calculated variables
    static int traveledPulses = 0;
    static double velocity = 0;

    timeCount++;
    if(timeCount == 200){
        timeCount = 0;

        traveledPulses = encoderPosition - lastEncoderPosition;
        velocity = ((traveledPulses) * (discPerimeter/ (discChangingStatesNumber)))/encmeasuringRate;
        qDebug() << encoderPosition << "traveledPulses:" << traveledPulses <<" Velocity:"<<velocity
                 << " AvgVelocity:"<< (lastVelocity + velocity)/2;

        //Assigned tracking variables
        lastVelocity = velocity;
        lastEncoderPosition = encoderPosition;

    }




}

void TestingConsole::updateEncoderWrtitingCounter()
{
    //tracking variables
    static int lastTimesCalled = 0;    
    static int lastEncoderPosition = 0;

    //calculated variables
    static int traveledPulses = 0;
    static double velocity = 0;

    uint lectures = encoderTimesWritten-lastTimesCalled;
    traveledPulses = encoderPosition - lastEncoderPosition;
    velocity = traveledPulses * (discPerimeter / discChangingStatesNumber);


    ui->lecturesvalLabel->setText(QString::number(lectures));
    ui->pulsvalLabel->setText(QString::number(qFabs(traveledPulses)));
    ui->metersvalLabel->setText(QString::number(velocity));

    //Assigned tracking variables
    lastTimesCalled = encoderTimesWritten;
    lastEncoderPosition = encoderPosition;
    m_timer->start(1000);
}
