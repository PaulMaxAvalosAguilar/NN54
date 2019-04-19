#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "albummodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<AlbumModel>("sdt.albumModel",1,0,"AlbumModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
