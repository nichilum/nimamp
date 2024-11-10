#pragma once
#include <QVector>
#include <QUrl>

class Playlist {
private:
    QVector<QUrl> songs;
    QString name;
public:
    explicit Playlist(const QString &name);
    Playlist(const QString &name, const QVector<QUrl> &songs);
    void addSong(const QUrl &song);
    void addSongs(const QVector<QUrl> &songs);
    void removeSong(const QUrl &song);

    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QVector<QUrl> getSongs() const { return songs; }
};
