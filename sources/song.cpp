#include "../headers/song.hpp"

#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

Song::Song(const QUrl &url) : url(url), filename(url.fileName()) {
    TagLib::MPEG::File file(url.toLocalFile().toUtf8().data());

    qDebug() << url.toLocalFile();

    if (file.ID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
        title = QString::fromStdString(tag->title().toCString(true));
        artist = QString::fromStdString(tag->artist().toCString(true));
    }
}