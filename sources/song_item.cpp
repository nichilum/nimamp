#include "../headers/song_item.hpp"

#include "../headers/player.hpp"
#include "ui_SongItem.h"

SongItem::SongItem(const Song &song, SongItemType songItemType, QWidget *parent)
    : QWidget(parent), ui(new Ui::SongItem) {
    ui->setupUi(this);
    ui->label->setText(song.getFilename());

    name = song.getFilename();
    auto player = Player::getInstance();

    // connect play button
    switch (songItemType) {
        case SongItemType::Queue: {
            connect(ui->playButton, &QPushButton::clicked, [player, song]() {
                player->playSongFromQueue(song);
            });
        }
        case SongItemType::Playlist: {
            connect(ui->playButton, &QPushButton::clicked, [player, song]() {
                player->playSong(song);
            });
        }
    }
}

SongItem::~SongItem() {
    delete ui;
}