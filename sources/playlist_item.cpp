#include "../headers/player.hpp"
#include "../headers/playlist_item.hpp"
#include "ui_PlaylistItem.h"

PlaylistItem::PlaylistItem(const Playlist &playlist, QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaylistItem) {
    ui->setupUi(this);

    ui->nameLabel->setText(playlist.getName());
}

PlaylistItem::~PlaylistItem() {
    delete ui;
}