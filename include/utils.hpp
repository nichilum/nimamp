#pragma once

#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

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

inline std::optional<QImage> getAlbumArt(const QUrl &url) {
    TagLib::MPEG::File mpegFile(url.toLocalFile().toUtf8().data());
    if (TagLib::ID3v2::Tag *id3v2Tag = mpegFile.ID3v2Tag()) {
        if (const auto &frameList = id3v2Tag->frameListMap()["APIC"]; !frameList.isEmpty()) {
            if (auto *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front())) {
                QByteArray imageData(pictureFrame->picture().data(), pictureFrame->picture().size());
                auto coverImage = QImage::fromData(imageData);
                coverImage = coverImage.scaled(QSize(100, 100), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                if (!coverImage.isNull()) {
                    return coverImage;
                }
            }
        }
    }

    return std::nullopt;
}