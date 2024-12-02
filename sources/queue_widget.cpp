#include "../headers/queue_widget.hpp"

#include <QMenu>

#include "../headers/player.hpp"
#include "../headers/queue_song_item.hpp"
#include "ui_QueueWidget.h"

QueueWidget::QueueWidget(QWidget *parent) : QWidget(parent), ui(new Ui::QueueWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    connect(player, &Player::queueChanged, this, &QueueWidget::updateQueue);
    connect(ui->queueListWidget->model(), &QAbstractItemModel::rowsMoved, this, &QueueWidget::onRowsMoved);
    connect(ui->queueListWidget, &QListWidget::customContextMenuRequested, this, &QueueWidget::onQueueItemRightClicked);
}

QueueWidget::~QueueWidget() {
    delete ui;
}

void QueueWidget::updateQueue() {
    auto player = Player::getInstance();
    ui->queueListWidget->clear();

    for (const auto &song : *player->getQueue()) {
        auto *songWidget = new QueueSongItem(song, this);

        auto *item = new QListWidgetItem(ui->queueListWidget);
        item->setSizeHint(songWidget->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        ui->queueListWidget->addItem(item);
        ui->queueListWidget->setItemWidget(item, songWidget);
    }
}

void QueueWidget::onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
    Q_UNUSED(parent);
    Q_UNUSED(end);

    auto player = Player::getInstance();

    if (row > start) {
        row -= 1;
    }

    if (start >= 0 && row >= 0 && start < player->getQueue()->size() && row <= player->getQueue()->size()) {
        auto movedItem = player->getQueue()->takeAt(start);
        player->getQueue()->insert(row, movedItem);
    }

    qDebug() << "Updated queue:" << *player->getQueue();
}

void QueueWidget::onQueueItemRightClicked(const QPoint &pos) {
    auto player = Player::getInstance();

    QListWidgetItem *item = ui->queueListWidget->itemAt(pos);
    if (!item) return;  // Ignore if no item was clicked

    QMenu menu(this);
    auto song = item->data(Qt::UserRole).value<Song>();
    auto *addToPlaylistMenu = menu.addMenu("Add to Playlist");

    auto playlists = player->getPlaylists();
    for (const auto &playlist : playlists) {
        QAction *action = addToPlaylistMenu->addAction(playlist.getName());
        connect(action, &QAction::triggered, [playlist, player, song]() {
            player->addToPlaylist(song, playlist);
        });
    }

    // Add other context menu actions
    menu.addSeparator();
    menu.addAction("Remove from Queue", [this, player, song]() {
        player->removeSongFromQueue(song);
    });

    menu.exec(ui->queueListWidget->mapToGlobal(pos));
}