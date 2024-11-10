#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QDir>

#include <iostream>

#include "headers/player.hpp"

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

    QAudioOutput audioOutput;
    Player player;

    player.print("init");
    player.setAudioOutput(&audioOutput);
    player.setSource(QUrl::fromLocalFile(QFileInfo("../music/442.wav").absoluteFilePath()));
    audioOutput.setVolume(0.2);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("nimamp", "Main");
    engine.rootContext()->setContextProperty("player", &player);

    return app.exec();
}
