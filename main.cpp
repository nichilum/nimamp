#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>

#include <QDir>

#include <iostream>

int main(int argc, char *argv[])
{

    QDir dir("/home/jonathankr/Github/nimamp/music");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);

    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo file, list)
    {
        std::cout << qPrintable(file.fileName()) << std::endl;
    }

    QGuiApplication app(argc, argv);

    // QMediaPlayer player;
    // QAudioOutput *audioOutput = new QAudioOutput();

    // player.setAudioOutput(audioOutput);
    // player.setSource(QUrl::fromLocalFile("/home/jonathankr/Github/nimamp/music/442.wav"));
    // audioOutput->setVolume(0.5);
    // player.play();

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
