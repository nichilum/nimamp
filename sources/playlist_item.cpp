#include "../headers/playlist_item.hpp"

#include "../headers/player.hpp"
#include "ui_PlaylistItem.h"

PlaylistItem::PlaylistItem(const Playlist &playlist, QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaylistItem), playlist(playlist) {
    ui->setupUi(this);

    auto player = Player::getInstance();
    loadPlaylistImage();

    ui->playlistItemNameLabel->setText(playlist.getName());

    connect(ui->playlistItemPlayButton, &QPushButton::clicked, this, &PlaylistItem::playPlaylist);
    connect(ui->playlistItemQueueButton, &QPushButton::clicked, this, &PlaylistItem::queuePlaylist);
    connect(player, &Player::playlistChanged, this, [this](const Playlist &playlist) {
        if (this->playlist.getUuid() == playlist.getUuid()) {
            loadPlaylistImage();
        }
    });
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

void PlaylistItem::loadPlaylistImage() const {
    auto songs = playlist.getSongs();

    if (songs.isEmpty()) {
        return;
    }

    auto song = std::find_if(songs.begin(), songs.end(), [](const Song &s) {
        return !s.albumArt.isNull();
    });

    if (song == songs.end()) {
        qDebug() << "No image found for playlist:" << playlist.getName();
        return;
    }

    qDebug() << "Using image from song:" << song->title;

    auto pixmap = QPixmap::fromImage(song->albumArt);
    pixmap = pixmap.scaled(QSize(32, 32), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->playlistItemImageLabel->setPixmap(pixmap);
}