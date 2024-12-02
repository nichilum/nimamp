#pragma once

#include "song.hpp"
#include "utils.hpp"
#include <QUuid>
#include <QMetaType>

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

    [[nodiscard]] QStringList getSongUrls() const { return convertToQStringList(songs); }
    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QUuid getUuid() const { return uuid; }
    [[nodiscard]] QVector<Song> getSongs() const { return songs; }
};

Q_DECLARE_METATYPE(Playlist)
