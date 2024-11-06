#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("nimamp", "Main");

    return app.exec();
}
