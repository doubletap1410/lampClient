#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <lampclient.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<LampClient>("lampclient", 1, 0, "LampClient");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

