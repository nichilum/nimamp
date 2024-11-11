#pragma once
#include <QUrl>

class Song {
private:
    QUrl url;
    QString filename;
public:
    Song(const QUrl &url, const QString &filename) : url(url), filename(filename) {}
    [[nodiscard]] QUrl getUrl() const { return url; }
    [[nodiscard]] QString getFilename() const { return filename; }
};