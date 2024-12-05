#include "../headers/playlist_tabs_widget.hpp"

#include "../headers/player.hpp"
#include "../headers/playlist_view_widget.hpp"
#include "../headers/song_item.hpp"
#include "ui_PlaylistTabsWidget.h"
#include "ui_PlaylistViewWidget.h"

PlaylistTabsWidget::PlaylistTabsWidget(PlaylistViewWidget *playlistViewWidget, QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistTabsWidget) {
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
                    item->setSizeHint(songWidget->sizeHint());
                    item->setData(Qt::UserRole, QVariant::fromValue(song));
                    playlistView->addItem(item);
                    playlistView->setItemWidget(item, songWidget);

                    // auto *songItem = new QListWidgetItem(song.getFilename(), playlistView);
                    // songItem->setData(Qt::UserRole, QVariant::fromValue(song));
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
    for (const auto &song : it->getSongs()) {
        auto *songWidget = new SongItem(song, SongItemType::Playlist);

        auto *item = new QListWidgetItem(playlistView);
        item->setSizeHint(songWidget->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        playlistView->addItem(item);
        playlistView->setItemWidget(item, songWidget);

        // auto *songItem = new QListWidgetItem(song.getFilename(), playlistView);
        // songItem->setData(Qt::UserRole, QVariant::fromValue(song));
    }

    ui->playlistTabs->addTab(playlistView, playlist.getName());
    ui->playlistTabs->setCurrentWidget(playlistView);
}