#include "../headers/song.hpp"

#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <QPixmap>

Song::Song(const QUrl &url) : url(url), filename(url.fileName()) {
    TagLib::MPEG::File file(url.toLocalFile().toUtf8().data());

    if (file.ID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
        title = QString::fromStdString(tag->title().toCString(true));
        artist = QString::fromStdString(tag->artist().toCString(true));

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