#pragma once

#include <taglib/apetag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>

#include <QImage>
#include <QPixmap>
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

// inline QPixmap getSongThumbnail(const QString &filePath) {
//     TagLib::MPEG::File file(filePath.toUtf8().data());
//     if (file.ID3v2Tag()) {
//         TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
//         TagLib::ID3v2::FrameList frames = tag->frameList("APIC");  // APIC is the frame for album art
//         if (!frames.isEmpty()) {
//             auto *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
//             if (pictureFrame) {
//                 QByteArray imageData = QByteArray::fromRawData(pictureFrame->picture().data(), pictureFrame->picture().size());
//                 QImage albumArt = QImage::fromData(imageData);
//                 if (!albumArt.isNull()) {
//                     // Display the album art or use it as needed
//                     return QPixmap::fromImage(albumArt);
//                 }
//             }
//         }
//     }
// }