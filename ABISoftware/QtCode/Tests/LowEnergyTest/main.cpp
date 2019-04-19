#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include <lowenergyhandle.h>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    qDebug()<<"Search dir for qt installation is"<<QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    qDebug()<< "STARTING PROGRAM FOR LOW ENERGY TEST";
    std::shared_ptr<LowEnergyHandle> lEh(new LowEnergyHandle);
    lEh->startScan();

    return a.exec();
}
