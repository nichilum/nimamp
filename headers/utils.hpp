#pragma once

#include <QVector>
#include <QUrl>
#include <algorithm>

inline QStringList convertToQStringList(const QVector<Song>& songVector) {
    QStringList stringList;
    std::transform(songVector.begin(), songVector.end(), std::back_inserter(stringList), [](const Song& song) {
        return song.getFilename();
    });
    return stringList;
}