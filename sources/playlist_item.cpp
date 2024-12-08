#include "../headers/playlist_item.hpp"

#include <QPainter>

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

    QVector<QImage> albumArts;
    for (const auto &song : songs) {
        if (!song.albumArt.isNull()) {
            albumArts.append(song.albumArt);
            if (albumArts.size() == 4) {
                break;
            }
        }
    }

    while (albumArts.size() < 4) {
        albumArts.append(QImage(":/resources/empty_cover.jpg"));
    }

    int gridSize = 2;
    int artSize = 16;
    QImage gridImage(artSize * gridSize, artSize * gridSize, QImage::Format_ARGB32);
    gridImage.fill(Qt::transparent);

    QPainter painter(&gridImage);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int index = i * gridSize + j;
            QImage scaledArt = albumArts[index].scaled(artSize, artSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint topLeft(j * artSize, i * artSize);
            painter.drawImage(topLeft, scaledArt);
        }
    }
    painter.end();

    auto pixmap = QPixmap::fromImage(gridImage);
    ui->playlistItemImageLabel->setPixmap(pixmap);
}