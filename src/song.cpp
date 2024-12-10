#include "../include/song.hpp"

#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

#include <QPixmap>

/**
 * Create a new song from a file
 * @param url The file to create the song from
 */
Song::Song(const QUrl &url) : url(url), filename(url.fileName()) {
    TagLib::FileRef file(url.toLocalFile().toUtf8().data());
    if (!file.isNull() && file.tag()) {
        TagLib::Tag *tag = file.tag();
        title = QString::fromStdString(tag->title().toCString(true));
        artist = QString::fromStdString(tag->artist().toCString(true));
        duration = file.audioProperties()->lengthInMilliseconds();

        TagLib::MPEG::File mpegFile(url.toLocalFile().toUtf8().data());
        TagLib::ID3v2::Tag *id3v2Tag = mpegFile.ID3v2Tag();
        if (id3v2Tag) {
            if (const auto &frameList = id3v2Tag->frameListMap()["APIC"]; !frameList.isEmpty()) {
                if (auto *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front())) {
                    QByteArray imageData(pictureFrame->picture().data(), pictureFrame->picture().size());
                    auto coverImage = QImage::fromData(imageData);
                    coverImage = coverImage.scaled(QSize(100, 100), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    if (!coverImage.isNull()) {
                        albumArt = coverImage;
                    }
                }
            }
        }
    }
}