#include "../headers/player.hpp"
#include "../headers/song_item.hpp"
#include "ui_SongItem.h"

SongItem::SongItem(const Song &song, QWidget *parent)
    : QWidget(parent), ui(new Ui::SongItem) {
    ui->setupUi(this);
    ui->label->setText(song.getFilename());

    name = song.getFilename();
    auto player = Player::getInstance();

    // Connect remove button
    connect(ui->deleteButton, &QPushButton::clicked, [player, song]() {
        player->removeSongFromQueue(song);
    });


    // connect play button
    connect(ui->playButton, &QPushButton::clicked, [player, song]() {
        player->playSongFromQueue(song);
    });
}

SongItem::~SongItem() {
    delete ui;
}