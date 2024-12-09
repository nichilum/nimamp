#include "../headers/song.hpp"

#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

#include <QPixmap>

/**
 * Create a new song from a file
 * @param url The file to create the song from
 */
Song::Song(const QUrl &url) : url(url), filename(url.fileName()) {
    TagLib::MPEG::File file(url.toLocalFile().toUtf8().data());

    if (file.ID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
        TagLib::AudioProperties *properties = file.audioProperties();
        title = QString::fromStdString(tag->title().toCString(true));
        artist = QString::fromStdString(tag->artist().toCString(true));
        duration = properties->lengthInMilliseconds();

        TagLib::ID3v2::FrameList frames = tag->frameList("APIC");  // APIC is the frame for album art
        if (!frames.isEmpty()) {
            if (auto *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front())) {
                auto imageData = QByteArray::fromRawData(pictureFrame->picture().data(), pictureFrame->picture().size());
                auto image = QImage::fromData(imageData);
                if (!image.isNull()) {
                    albumArt = image;
                }
            }
        }
    }
}