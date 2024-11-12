#pragma once

#include "song.hpp"
#include "utils.hpp"

class Playlist {
   private:
    QVector<Song> songs;
    QString name;

   public:
    explicit Playlist(const QString &name);
    Playlist(const QString &name, const QVector<Song> &songs);
    void addSong(const Song &song);
    void addSongs(const QVector<Song> &songs);
    void removeSong(const Song &song);

    [[nodiscard]] QStringList getSongUrls() const { return convertToQStringList(songs); }
    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QVector<Song> getSongs() const { return songs; }
};
