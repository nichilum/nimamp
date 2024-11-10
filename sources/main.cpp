#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMediaMetaData>
#include <QDir>

#include <iostream>

#include "headers/player.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Player player;

    QVector<QUrl> urls;
    QDir directory(QCoreApplication::applicationDirPath() + "/../music");
    QStringList images = directory.entryList(QStringList() << "*.wav" << "*.mp3",QDir::Files);
    foreach(QString filename, images) {
        urls.append(QUrl::fromLocalFile(QFileInfo(directory, filename).absoluteFilePath()));
    }

    Playlist playlist("default", urls);
    player.addPlaylist(playlist);
    player.playPlaylist("default");
    player.setVolume(0.1);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("player", &player);

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
