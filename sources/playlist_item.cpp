#include "../headers/playlist_item.hpp"

#include "../headers/player.hpp"
#include "ui_PlaylistItem.h"

PlaylistItem::PlaylistItem(const Playlist &playlist, QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaylistItem), playlist(playlist) {
    ui->setupUi(this);

    ui->nameLabel->setText(playlist.getName());

    connect(ui->playButton, &QPushButton::clicked, this, &PlaylistItem::playPlaylist);
    connect(ui->queueButton, &QPushButton::clicked, this, &PlaylistItem::queuePlaylist);
}

PlaylistItem::~PlaylistItem() {
    delete ui;
}
void PlaylistItem::playPlaylist() const {
    Player::getInstance()->playPlaylist(playlist);
}

void PlaylistItem::queuePlaylist() const {
    Player::getInstance()->queuePlaylist(playlist);
}
void PlaylistItem::removePlaylist() const {
    Player::getInstance()->removePlaylist(playlist);
}
