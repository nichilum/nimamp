#pragma once

#include <QMetaType>
#include <QUuid>

#include "song.hpp"
#include "utils.hpp"

class Playlist {
   private:
    QUuid uuid;
    QVector<Song> songs;
    QString name;

    QString sortingType;

   public:
    Playlist() = default;
    explicit Playlist(const QString &name);
    Playlist(const QString &name, const QUrl &directory);
    Playlist(const QString &name, const QVector<Song> &songs);
    void addSong(const Song &song);
    void addSongs(const QVector<Song> &songs);
    void removeSong(const Song &song);
    void moveSong(int from, int to);
    void setSortingType(const QString sortingType) { this->sortingType = sortingType; };

    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QString getSortingType() const { return sortingType; }
    [[nodiscard]] bool isSorted() const { return sortingType != "Default"; }
    [[nodiscard]] QUuid getUuid() const { return uuid; }
    [[nodiscard]] QVector<Song> getSongs() const { return sortSongVector(songs, sortingType); }
    [[nodiscard]] QVector<Song> *getSongsPointer() { return &songs; }

    friend QDataStream &operator<<(QDataStream &out, const Playlist &playlist) {
        out << playlist.uuid << playlist.songs << playlist.name << playlist.sortingType;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Playlist &playlist) {
        in >> playlist.uuid >> playlist.songs >> playlist.name >> playlist.sortingType;
        return in;
    }
};

Q_DECLARE_METATYPE(Playlist)
