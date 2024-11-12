#include <QDir>
#include <QGuiApplication>
#include <QMediaMetaData>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringListModel>
#include <iostream>

#include "headers/player.hpp"
#include "headers/song_model.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    Player player;

    QVector<Song> urls;
    QDir directory(QCoreApplication::applicationDirPath() + "/../music");
    QStringList files = directory.entryList(QStringList() << "*.wav"
                                                          << "*.mp3",
                                            QDir::Files);
    foreach (QString filename, files) {
        auto song = Song(QUrl::fromLocalFile(directory.absoluteFilePath(filename)), filename);
        urls.append(song);
    }

    Playlist playlist("default", urls);
    player.addPlaylist(playlist);
    player.playPlaylist("default");
    player.setVolume(0.1);

    SongModel queueModel;
    queueModel.addSongs(player.getQueue());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("player", &player);
    engine.rootContext()->setContextProperty("queueModel", &queueModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("nimamp", "Main");

    return app.exec();
}
