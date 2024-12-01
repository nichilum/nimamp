#pragma once

#include <QUrl>
#include <QVector>
#include <algorithm>

inline QStringList convertToQStringList(const QVector<Song>& songVector) {
    QStringList stringList;
    std::transform(songVector.begin(), songVector.end(), std::back_inserter(stringList), [](const Song& song) {
        return song.getFilename();
    });
    return stringList;
}

inline QString msToString(const qint64 position) {
    int seconds = static_cast<int>(position / 1000);
    int minutes = seconds / 60;
    int hours = minutes / 60;

    seconds %= 60;
    minutes %= 60;

    QString formattedTime;
    if (hours > 0) {
        formattedTime = QString("%1:%2:%3")
                            .arg(hours, 2, 10, QLatin1Char('0'))
                            .arg(minutes, 2, 10, QLatin1Char('0'))
                            .arg(seconds, 2, 10, QLatin1Char('0'));
    } else {
        formattedTime = QString("%1:%2")
                            .arg(minutes, 2, 10, QLatin1Char('0'))
                            .arg(seconds, 2, 10, QLatin1Char('0'));
    }

    return formattedTime;
}