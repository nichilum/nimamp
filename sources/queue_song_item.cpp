#include "../headers/player.hpp"
#include "../headers/queue_song_item.hpp"
#include "ui_QueueSongItem.h"

QueueSongItem::QueueSongItem(const Song &song, QWidget *parent)
    : QWidget(parent), ui(new Ui::QueueSongItem) {
    ui->setupUi(this);
    ui->label->setText(song.getFilename());

    name = song.getFilename();
    auto player = Player::getInstance();

    // connect play button
    connect(ui->playButton, &QPushButton::clicked, [player, song]() {
        player->playSongFromQueue(song);
    });
}

QueueSongItem::~QueueSongItem() {
    delete ui;
}