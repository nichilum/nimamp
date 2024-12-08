#include "../headers/playlist_tabs_widget.hpp"

#include <QMenu>

#include "../headers/player.hpp"
#include "../headers/playlist_view_widget.hpp"
#include "../headers/song_item.hpp"
#include "ui_PlaylistTabsWidget.h"
#include "ui_PlaylistViewWidget.h"

PlaylistTabsWidget::PlaylistTabsWidget(const PlaylistViewWidget *playlistViewWidget, QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistTabsWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    connect(ui->playlistTabs, &QTabWidget::tabCloseRequested, this, &PlaylistTabsWidget::onPlaylistTabCloseRequested);
    connect(player, &Player::playlistChanged, this, &PlaylistTabsWidget::updatePlaylist);
    connect(playlistViewWidget->getUi()->playlistListWidget, &QListWidget::itemClicked, this, &PlaylistTabsWidget::onPlaylistSelected);
}

PlaylistTabsWidget::~PlaylistTabsWidget() {
    delete ui;
}

void PlaylistTabsWidget::onPlaylistTabCloseRequested(const int index) const {
    auto *widget = ui->playlistTabs->widget(index);
    ui->playlistTabs->removeTab(index);
    delete widget;
}

void PlaylistTabsWidget::updatePlaylist(const Playlist &playlist) const {
    for (int i = 0; i < ui->playlistTabs->count(); ++i) {
        if (auto *playlistView = qobject_cast<QListWidget *>(ui->playlistTabs->widget(i))) {
            auto tabPlaylist = playlistView->property("playlistUuid").toUuid();
            if (tabPlaylist == playlist.getUuid()) {
                qDebug() << "Updating playlist:" << playlist.getName();
                playlistView->clear();
                for (const auto &song : playlist.getSongs()) {
                    auto *songWidget = new SongItem(song, SongItemType::Playlist);

                    auto *item = new QListWidgetItem(playlistView);
                    auto size = songWidget->sizeHint().boundedTo(playlistView->size());
                    size.setHeight(64);
                    item->setSizeHint(size);
                    item->setData(Qt::UserRole, QVariant::fromValue(song));
                    playlistView->addItem(item);
                    playlistView->setItemWidget(item, songWidget);
                }
                break;
            }
        }
    }
}

void PlaylistTabsWidget::onPlaylistSelected(const QListWidgetItem *item) const {
    auto player = Player::getInstance();
    auto playlists = player->getPlaylists();

    auto playlist = item->data(Qt::UserRole).value<Playlist>();
    auto uuid = playlist.getUuid();
    auto it = std::find_if(playlists.begin(), playlists.end(), [&uuid](const Playlist &p) {
        return p.getUuid() == uuid;
    });

    if (it == playlists.end()) {
        qDebug() << "Playlist not found!";
        return;
    }

    // switch to the tab if it's already open
    for (int i = 0; i < ui->playlistTabs->count(); ++i) {
        if (auto *playlistView = qobject_cast<QListWidget *>(ui->playlistTabs->widget(i))) {
            auto tabPlaylist = playlistView->property("playlistUuid").toUuid();
            if (tabPlaylist == playlist.getUuid()) {
                ui->playlistTabs->setCurrentIndex(i);
                return;
            }
        }
    }

    auto *playlistView = new QListWidget;
    playlistView->setProperty("playlistUuid", playlist.getUuid());
    playlistView->setObjectName("playlistTabListWidget");
    playlistView->setContextMenuPolicy(Qt::CustomContextMenu);
    playlistView->setDragDropMode(QAbstractItemView::InternalMove);

    connect(playlistView, &QListWidget::customContextMenuRequested, this, [this, playlist, playlistView](const QPoint &pos) {
        auto songItem = playlistView->itemAt(pos);
        auto song = songItem->data(Qt::UserRole).value<Song>();
        auto globalPos = playlistView->mapToGlobal(pos);
        onPlaylistItemRightClicked(globalPos, playlist, song);
    });

    connect(playlistView->model(), &QAbstractItemModel::rowsMoved, this, [this, playlist](const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
        Q_UNUSED(parent);
        Q_UNUSED(end);

        auto player = Player::getInstance();

        if (row > start) {
            row -= 1;
        }

        if (start >= 0 && row >= 0 && start < playlist.getSongs().size() && row <= playlist.getSongs().size()) {
            player->moveSongInPlaylist(playlist, start, row);
        }
    });

    for (const auto &song : it->getSongs()) {
        auto *songWidget = new SongItem(song, SongItemType::Playlist);

        auto *item = new QListWidgetItem(playlistView);
        auto size = songWidget->sizeHint().boundedTo(playlistView->size());
        size.setHeight(64);
        item->setSizeHint(size);
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        playlistView->addItem(item);
        playlistView->setItemWidget(item, songWidget);
    }

    ui->playlistTabs->addTab(playlistView, playlist.getName());
    ui->playlistTabs->setCurrentWidget(playlistView);
}

void PlaylistTabsWidget::onPlaylistItemRightClicked(const QPoint &globalPos, const Playlist &playlist, const Song &song) const {
    auto player = Player::getInstance();
    QMenu menu;

    menu.addAction("Add to Queue", [this, player, song]() {
        player->queueSong(song);
    });

    menu.addAction("Delete", [this, player, playlist, song]() {
        player->removeSongFromPlaylist(song, playlist);
    });

    menu.exec(globalPos);
}