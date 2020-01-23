#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <memory>
#include "blescannermodel.h"

#include <QtTextToSpeech>

int main(int argc, char *argv[]){

    QApplication app(argc,argv);
    std::unique_ptr<BleScannerModel> blescannermodel(new BleScannerModel);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("blescannermodel", blescannermodel.get());
    context->setContextProperty("connhandling", blescannermodel->getConnHandling().get());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
