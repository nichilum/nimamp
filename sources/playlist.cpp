#include "../headers/playlist.hpp"

#include <QDir>

Playlist::Playlist(const QString &name) : name(name), uuid(QUuid::createUuid()) {}

Playlist::Playlist(const QString &name, const QVector<Song> &songs) : name(name), songs(songs), uuid(QUuid::createUuid()) {}

Playlist::Playlist(const QString &name, const QUrl &directory) : name(name), uuid(QUuid::createUuid()) {
    const QDir dir(directory.toLocalFile());
    QStringList files = dir.entryList(QStringList() << "*.wav"
                                                    << "*.mp3",
                                      QDir::Files);
    for (const auto &filename : files) {
        auto song = Song(QUrl::fromLocalFile(dir.absoluteFilePath(filename)));
        this->songs.push_back(song);
    }
}

void Playlist::addSong(const Song &song) {
    songs.push_back(song);
}

void Playlist::addSongs(const QVector<Song> &songs) {
    this->songs += songs;
}

void Playlist::removeSong(const Song &song) {
    songs.removeOne(song);
}

void Playlist::moveSong(int from, int to) {
    if (from < 0 || from >= songs.size() || to < 0 || to >= songs.size()) {
        return;
    }

    auto movedItem = songs.takeAt(from);
    songs.insert(to, movedItem);
}

bool operator==(const Playlist &lhs, const Playlist &rhs) {
    return lhs.getUuid() == rhs.getUuid();
}