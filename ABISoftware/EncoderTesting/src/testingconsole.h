#ifndef TESTINGCONSOLE_H
#define TESTINGCONSOLE_H

#include <QWidget>
#include <QString>
#include <QSerialPort>
#include <QTimer>
#include "console.h"
#include <deque>

namespace Ui {
class TestingConsole;
}

class TestingConsole : public QWidget
{
    Q_OBJECT

public:
    TestingConsole(QWidget *parent = 0);
    ~TestingConsole();
    void parseLine(const QString &line);
    Console *getConsole();

private:
    void readEncoder(const QString &line, std::deque<int> &numbers);
    void calculateVelocity(int encoderPosition);

signals:
    void writeData(const QByteArray &data);

public slots:
    void updateEncoderWrtitingCounter();

private:
    Ui::TestingConsole *ui;

    QTimer *m_timer;

    //encodingFormat
    const uint encNumberofValues;
    const uint encByteSize;
    const double encmeasuringRate;

    //physical dimensions
    const uint discToothNumber;
    const uint discChangingStatesNumber;
    const double discPerimeter;

    //read characteristics
    uint encoderTimesWritten;
    int encoderPosition;
    std::deque<int> extractedValues;

};

#endif // TESTINGCONSOLE_H
