#include "../include/playlist.hpp"

#include <QDir>

#include "../include/player.hpp"

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

/**
 * Adds a song to the playlist
 * @param song Song to be added to the playlist
 */
void Playlist::addSong(const Song &song) {
    songs.push_back(song);
}

/**
 * Adds multiple songs to the playlist
 * @param songs Songs to be added to the playlist
 */
void Playlist::addSongs(const QVector<Song> &songs) {
    this->songs += songs;
}

/**
 * Removes a song from the playlist
 * @param song Song to be removed from the playlist
 */
void Playlist::removeSong(const Song &song) {
    songs.removeOne(song);
}

/**
 * Moves a song from one position to another
 * @param from Source index
 * @param to Destination index
 */
void Playlist::moveSong(int from, int to) {
    if (from < 0 || from >= songs.size() || to < 0 || to >= songs.size()) {
        return;
    }

    auto movedItem = songs.takeAt(from);
    songs.insert(to, movedItem);
}

/**
 * Compares two playlists by their UUID
 * @param lhs Left-hand side playlist
 * @param rhs Right-hand side playlist
 * @return True if the playlists have the same UUID, false otherwise
 */
bool operator==(const Playlist &lhs, const Playlist &rhs) {
    return lhs.getUuid() == rhs.getUuid();
}