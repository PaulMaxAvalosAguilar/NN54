#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <memory>
#include "blescannermodel.h"

#include <QtTextToSpeech>

int main(int argc, char *argv[]){

    QApplication app(argc,argv);
    std::unique_ptr<BleScannerModel> blescannermodel(new BleScannerModel);

    /*
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("blescannermodel", blescannermodel.get());
    context->setContextProperty("connhandling", blescannermodel->getConnHandling().get());


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    */

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("blescannermodel", blescannermodel.get());
    context->setContextProperty("connhandling", blescannermodel->getConnHandling().get());
    QObject *object = component.create(context);


    QObject *rect = object->findChild<QObject*>("encoderPage");
    if (rect)    QMetaObject::invokeMethod(rect, "encoderHello");


    return app.exec();
}
