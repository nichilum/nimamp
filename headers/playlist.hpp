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

   public:
    Playlist() = default;
    explicit Playlist(const QString &name);
    Playlist(const QString &name, const QUrl &directory);
    Playlist(const QString &name, const QVector<Song> &songs);
    void addSong(const Song &song);
    void addSongs(const QVector<Song> &songs);
    void removeSong(const Song &song);

    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QUuid getUuid() const { return uuid; }
    [[nodiscard]] QVector<Song> getSongs() const { return songs; }

    friend QDataStream &operator<<(QDataStream &out, const Playlist &playlist) {
        out << playlist.uuid << playlist.songs << playlist.name;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Playlist &playlist) {
        in >> playlist.uuid >> playlist.songs >> playlist.name;
        return in;
    }
};

Q_DECLARE_METATYPE(Playlist)
