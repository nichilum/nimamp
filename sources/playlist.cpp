#include "headers/playlist.hpp"

Playlist::Playlist(const QString &name) : name(name) {}

Playlist::Playlist(const QString &name, const QVector<QUrl> &songs) : name(name), songs(songs) {}

void Playlist::addSong(const QUrl &song) {
    songs.push_back(song);
}

void Playlist::addSongs(const QVector<QUrl> &songs) {
    this->songs += songs;
}

void Playlist::removeSong(const QUrl &song) {
    songs.removeOne(song);
}