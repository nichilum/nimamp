#include "../headers/playlist_view_widget.hpp"

#include <QInputDialog>

#include "../headers/mainwindow.hpp"
#include "../headers/player.hpp"
#include "../headers/playlist_item.hpp"
#include "ui_PlaylistViewWidget.h"

PlaylistViewWidget::PlaylistViewWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistViewWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();
    connect(ui->createPlaylistButton, &QPushButton::clicked, this, &PlaylistViewWidget::createPlaylistButtonClicked);
    connect(player, &Player::playlistsChanged, this, &PlaylistViewWidget::updatePlaylists);
}

PlaylistViewWidget::~PlaylistViewWidget() {
    delete ui;
}

void PlaylistViewWidget::updatePlaylists() {
    auto player = Player::getInstance();
    ui->playlistListWidget->clear();

    for (const auto &playlist : player->getPlaylists()) {
        auto *playlistWidget = new PlaylistItem(playlist, this);

        auto *item = new QListWidgetItem(ui->playlistListWidget);
        item->setSizeHint(playlistWidget->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(playlist));
        ui->playlistListWidget->addItem(item);
        ui->playlistListWidget->setItemWidget(item, playlistWidget);
    }
}

void PlaylistViewWidget::createPlaylistButtonClicked() {
    bool ok;
    auto playlistName = QInputDialog::getText(this,
                                              tr("Create Playlist"),
                                              tr("Enter playlist name:"),
                                              QLineEdit::Normal,
                                              "",
                                              &ok);
    if (ok && !playlistName.isEmpty()) {
        auto player = Player::getInstance();
        auto playlist = Playlist(playlistName);
        player->addPlaylist(playlist);

        qDebug() << "Playlist created:" << playlistName;
    } else {
        qDebug() << "User canceled or entered an empty name.";
    }
}
