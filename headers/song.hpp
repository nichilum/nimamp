#pragma once

#include <QDataStream>
#include <QDebug>
#include <QMetaType>
#include <QUrl>
#include <QVariant>

class Song : public QVariant {
   public:
    QUrl url;
    QString filename;

    Song() = default;
    Song(const QUrl &url, const QString &filename) : url(url), filename(filename) {}
    explicit Song(const QUrl &url) : url(url), filename(url.fileName()) {}
    [[nodiscard]] QUrl getUrl() const { return url; }
    [[nodiscard]] QString getFilename() const { return filename; }

    bool operator==(Song const &) const = default;

    friend QDataStream &operator<<(QDataStream &out, const Song &song) {
        out << song.url << song.filename;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Song &song) {
        in >> song.url >> song.filename;
        return in;
    }
};

inline QDebug operator<<(QDebug debug, const Song &song) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "Song(" << song.getUrl() << ", " << song.getFilename() << ")";
    return debug;
}

inline QDebug operator<<(QDebug debug, const QVector<Song> &songs) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "QVector<Song> [";
    for (const auto &song : songs) {
        debug.nospace() << "\n  " << song;
    }
    debug.nospace() << "\n]";
    return debug;
}

Q_DECLARE_METATYPE(Song)