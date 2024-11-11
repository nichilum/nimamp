#include "headers/playlist.hpp"

Playlist::Playlist(const QString &name) : name(name) {}

Playlist::Playlist(const QString &name, const QVector<Song> &songs) : name(name), songs(songs) {}

void Playlist::addSong(const Song &song) {
    songs.push_back(song);
}

void Playlist::addSongs(const QVector<Song> &songs) {
    this->songs += songs;
}

void Playlist::removeSong(const Song &song) {
    songs.removeOne(song);
}