#include "../headers/player.hpp"
#include "../headers/song_item.hpp"
#include "ui_SongItem.h"

SongItem::SongItem(const Song &song, QWidget *parent)
    : QWidget(parent), ui(new Ui::SongItem) {
    ui->setupUi(this);
    ui->label->setText(song.getFilename());

    name = song.getFilename();

    // Connect remove button
    connect(ui->deleteButton, &QPushButton::clicked, [this, song]() {
        emit removeRequested(song.getFilename());
    });

    auto player = Player::getInstance();

    // connect play button
    connect(ui->playButton, &QPushButton::clicked, [player, song]() {
        player->playSong(song);
    });
}

SongItem::~SongItem() {
    delete ui;
}