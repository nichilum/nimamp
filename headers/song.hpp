#pragma once
#include <QDebug>
#include <QUrl>

class Song {
   public:
    QUrl url;
    QString filename;

    Song(const QUrl &url, const QString &filename) : url(url), filename(filename) {}
    Song(const QUrl &url) : url(url), filename(url.fileName()) {}
    [[nodiscard]] QUrl getUrl() const { return url; }
    [[nodiscard]] QString getFilename() const { return filename; }

    bool operator==(Song const &) const = default;
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