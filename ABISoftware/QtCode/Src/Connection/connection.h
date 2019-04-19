#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class Connection: public QObject
{

    Q_OBJECT

public:

    virtual ~Connection();


    virtual void connect()=0;
    virtual void disconnect()=0;


signals:
   void dataReceived(int counter, int time_interval);

private slots:
    virtual void receiveDeviceInfo()=0;

protected:
    Connection();
    QByteArray line;
    QStringList split;
    QString text;
    QString parsedDATA;
    int numero;
    int intervalo;

};

#endif // CONNECTION_H
