#pragma once

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

inline QVector<Song> sortSongVector(const QVector<Song> &songs, QString sortingType) {
    auto sortedSongs = songs;
    if (sortingType == "Name ASC") {
        std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song &a, const Song &b) {
            return a.filename < b.filename;
        });
    } else if (sortingType == "Name DSC") {
        std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song &a, const Song &b) {
            return a.filename > b.filename;
        });
    } else if (sortingType == "Duration ASC") {
        std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song &a, const Song &b) {
            return a.duration < b.duration;
        });
    } else if (sortingType == "Duration DSC") {
        std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song &a, const Song &b) {
            return a.duration > b.duration;
        });
    }
    return sortedSongs;
}