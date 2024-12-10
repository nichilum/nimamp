#include "../include/playlist_item.hpp"

#include <QPainter>

#include "../include/player.hpp"
#include "ui_PlaylistItem.h"

PlaylistItem::PlaylistItem(const Playlist &playlist, QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaylistItem), playlist(playlist) {
    ui->setupUi(this);

    auto player = Player::getInstance();
    loadPlaylistImage();

    ui->playlistItemNameLabel->setText(playlist.getName());
    ui->playlistItemAmountLabel->setText(QString::number(playlist.getSongs().size()) + " Songs");

    connect(ui->playlistItemPlayButton, &QPushButton::clicked, this, &PlaylistItem::playPlaylist);
    connect(ui->playlistItemQueueButton, &QPushButton::clicked, this, &PlaylistItem::queuePlaylist);
    connect(player, &Player::playlistChanged, this, [this](const Playlist &p) {
        if (this->playlist.getUuid() == p.getUuid()) {
            this->playlist = p;
            ui->playlistItemAmountLabel->setText(QString::number(p.getSongs().size()) + " Songs");
            loadPlaylistImage();
        }
    });
}

PlaylistItem::~PlaylistItem() {
    delete ui;
}

/**
 * Play the playlist
 */
void PlaylistItem::playPlaylist() const {
    Player::getInstance()->playPlaylist(playlist);
}

/**
 * Append the playlist to the end of the queue
 */
void PlaylistItem::queuePlaylist() const {
    Player::getInstance()->queuePlaylist(playlist);
}

/**
 * Remove the playlist from the collection of playlists
 */
void PlaylistItem::removePlaylist() const {
    Player::getInstance()->removePlaylist(playlist);
}

/**
 * Load the playlist image.
 * Creates a grid of album arts from the first four songs in the playlist
 */
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

    auto gridSize = 2;
    auto artSize = 16;
    QImage gridImage(artSize * gridSize, artSize * gridSize, QImage::Format_ARGB32);
    gridImage.fill(Qt::transparent);

    QPainter painter(&gridImage);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            auto index = i * gridSize + j;
            auto scaledArt = albumArts[index].scaled(artSize, artSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint topLeft(j * artSize, i * artSize);
            painter.drawImage(topLeft, scaledArt);
        }
    }
    painter.end();

    auto pixmap = QPixmap::fromImage(gridImage);
    ui->playlistItemImageLabel->setPixmap(pixmap);
}