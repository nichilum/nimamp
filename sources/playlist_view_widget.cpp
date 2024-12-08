#include "../headers/playlist_view_widget.hpp"

#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>

#include "../headers/player.hpp"
#include "../headers/playlist_item.hpp"
#include "ui_PlaylistViewWidget.h"

PlaylistViewWidget::PlaylistViewWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistViewWidget) {
    ui->setupUi(this);

    ui->playlistListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playlistListWidget->setWordWrap(true);

    auto player = Player::getInstance();
    connect(ui->createPlaylistButton, &QPushButton::clicked, this, &PlaylistViewWidget::createPlaylistButtonClicked);
    connect(player, &Player::playlistsChanged, this, &PlaylistViewWidget::updatePlaylists);
    connect(ui->playlistListWidget, &QListWidget::customContextMenuRequested, this, &PlaylistViewWidget::onPlaylistItemRightClicked);

    updatePlaylists();
}

PlaylistViewWidget::~PlaylistViewWidget() {
    delete ui;
}

/**
 * Re-renders the list of playlists
 */
void PlaylistViewWidget::updatePlaylists() {
    auto player = Player::getInstance();
    ui->playlistListWidget->clear();

    for (const auto &playlist : player->getPlaylists()) {
        auto *playlistWidget = new PlaylistItem(playlist, this);

        auto *item = new QListWidgetItem(ui->playlistListWidget);
        auto size = playlistWidget->sizeHint().boundedTo(ui->playlistListWidget->size());
        size.setHeight(80);
        item->setSizeHint(size);
        item->setData(Qt::UserRole, QVariant::fromValue(playlist));
        ui->playlistListWidget->addItem(item);
        ui->playlistListWidget->setItemWidget(item, playlistWidget);
    }
}

/**
 * Handles the creation of a new playlist
 */
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

/**
 * Handles right-click events on playlist items
 * @param pos The position of the right click (relative to the widget)
 */
void PlaylistViewWidget::onPlaylistItemRightClicked(const QPoint &pos) {
    auto player = Player::getInstance();

    QListWidgetItem *item = ui->playlistListWidget->itemAt(pos);
    if (!item) return;

    QMenu menu(this);
    auto playlist = item->data(Qt::UserRole).value<Playlist>();

    menu.addAction("Add Folder to Playlist", [this, player, playlist]() {
        auto folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder to Add"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        player->addFolderToPlaylist(folderPath, playlist);
    });

    menu.addAction("Delete", [this, player, playlist]() {
        player->removePlaylist(playlist);
    });

    menu.exec(ui->playlistListWidget->mapToGlobal(pos));
}
