#include "../headers/song_item.hpp"

#include "../headers/player.hpp"
#include "ui_SongItem.h"

SongItem::SongItem(const Song &song, SongItemType songItemType, QWidget *parent)
    : SongItem(song, songItemType, -1, parent) {
}

SongItem::SongItem(const Song &song, SongItemType songItemType, int index, QWidget *parent)
    : QWidget(parent), ui(new Ui::SongItem) {
    ui->setupUi(this);

    ui->songItemSongLabel->setText(song.title == "" ? song.getFilename() : song.title);
    ui->songItemArtistLabel->setText(song.artist == "" ? "Unknown artist" : song.artist);
    auto image = song.albumArt.isNull() ? QImage(":/resources/empty_cover.jpg") : song.albumArt;
    image = image.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->songItemCover->setPixmap(QPixmap::fromImage(image));
    if (index >= 0) {
        ui->songItemNumber->setText(QString::number(index));
    } else {
        ui->songItemNumber->setText("");
    }

    auto player = Player::getInstance();

    // connect play button
    switch (songItemType) {
        case SongItemType::Queue: {
            connect(ui->songItemPlayButton, &QPushButton::clicked, [player, song]() {
                player->playSongFromQueue(song);
            });
        }
        case SongItemType::Playlist: {
            connect(ui->songItemPlayButton, &QPushButton::clicked, [player, song]() {
                player->playSong(song);
            });
        }
    }
}

SongItem::~SongItem() {
    delete ui;
}