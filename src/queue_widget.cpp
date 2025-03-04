#include "../include/queue_widget.hpp"

#include <QMenu>

#include "../include/player.hpp"
#include "../include/song_item.hpp"
#include "ui_QueueWidget.h"

QueueWidget::QueueWidget(QWidget *parent) : QWidget(parent), ui(new Ui::QueueWidget) {
    ui->setupUi(this);

    ui->queueListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->queueListWidget->setWordWrap(true);

    auto player = Player::getInstance();

    updateQueue();  // render loaded queue

    connect(player, &Player::queueChanged, this, &QueueWidget::updateQueue);
    connect(ui->queueListWidget->model(), &QAbstractItemModel::rowsMoved, this, &QueueWidget::onRowsMoved);
    connect(ui->queueListWidget, &QListWidget::customContextMenuRequested, this, &QueueWidget::onQueueItemRightClicked);
    connect(ui->queueClearButton, &QPushButton::pressed, player, &Player::clearQueue);
}

QueueWidget::~QueueWidget() {
    delete ui;
}

/**
 * Re-renders the queue
 */
void QueueWidget::updateQueue() {
    auto player = Player::getInstance();
    ui->queueListWidget->clear();

    int index = 0;
    for (const auto &song : *player->getQueue()) {
        auto *songWidget = new SongItem(song, SongItemType::Queue, ++index, this);

        auto *item = new QListWidgetItem(ui->queueListWidget);
        auto size = songWidget->sizeHint().boundedTo(ui->queueListWidget->size());
        size.setHeight(64);
        item->setSizeHint(size);
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        ui->queueListWidget->addItem(item);
        ui->queueListWidget->setItemWidget(item, songWidget);
    }
}

/**
 * Handles the movement of songs in the queue
 * @param parent The parent index
 * @param start The starting row
 * @param end The ending row
 * @param destination The destination index
 * @param row The row to move to
 */
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

    emit player->queueChanged();
}

/**
 * Handles right-click events on queue items
 * @param pos The position of the right click (relative to the widget)
 */
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