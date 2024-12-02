#include "../headers/mainwindow.hpp"

#include <QFileDialog>
#include <QInputDialog>
#include <QMediaMetaData>

#include "../headers/player.hpp"
#include "../headers/playlist_item.hpp"
#include "../headers/queue_song_item.hpp"
#include "../headers/utils.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    // set volume slider
    ui->volumeSlider->setValue(static_cast<int>(player->getVolume() * 100.));

    // buttons
    connect(ui->playButton, &QPushButton::clicked, player, &Player::togglePlayPause);
    connect(ui->nextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->prevButton, &QPushButton::clicked, player, &Player::previous);
    connect(ui->loopButton, &QPushButton::clicked, this, &MainWindow::toggleLoop);
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

    // seek slider
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateSeekDuration);
    connect(ui->seekSlider, &QSlider::sliderReleased, this, &MainWindow::seekToReleasedPosition);

    // queue
    connect(player, &Player::queueChanged, this, &MainWindow::updateQueue);
    connect(ui->queueListWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::onRowsMoved);
    connect(ui->queueListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onQueueItemRightClicked);

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);

    // playlists
    connect(ui->createPlaylistButton, &QPushButton::clicked, this, &MainWindow::createPlaylistButtonClicked);
    connect(player, &Player::playlistsChanged, this, &MainWindow::updatePlaylists);
    connect(ui->playlistListWidget, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSelected);
    connect(ui->playlistTabs, &QTabWidget::tabCloseRequested, this, &MainWindow::onPlaylistTabCloseRequested);
    connect(player, &Player::playlistChanged, this, &MainWindow::updatePlaylist);

    connect(player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetadataChanged);
    connect(player, &QMediaPlayer::playingChanged, this, &MainWindow::changePlayPauseIcon);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openFolderDialog() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    Player::getInstance()->addFolderToQueue(dir);
}

void MainWindow::updateSeekSlider(const qint64 position) const {
    if (ui->seekSlider->isSliderDown()) {
        return;
    }
    ui->seekSlider->setValue(static_cast<int>(position));

    auto formattedTime = msToString(position);

    ui->currentTimeLabel->setText(formattedTime);
}

void MainWindow::updateSeekDuration(const qint64 duration) const {
    ui->seekSlider->setRange(0, static_cast<int>(duration));
}

void MainWindow::seekToReleasedPosition() {
    auto player = Player::getInstance();
    player->setPosition(ui->seekSlider->value());
}

void MainWindow::updateVolume(int volume) {
    auto player = Player::getInstance();
    player->setVolume(volume / 100.);
}

void MainWindow::updateQueue() {
    auto player = Player::getInstance();
    ui->queueListWidget->clear();

    for (const auto &song : *player->getPriorityQueue()) {
        auto *songWidget = new QueueSongItem(song, this);

        auto *item = new QListWidgetItem(ui->queueListWidget);
        item->setSizeHint(songWidget->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        ui->queueListWidget->addItem(item);
        ui->queueListWidget->setItemWidget(item, songWidget);
    }
    if (!player->isPriorityQueueEmpty()) {  // hline between queues
        auto line = new QFrame();
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        auto *item = new QListWidgetItem(ui->queueListWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setSizeHint(line->sizeHint());
        ui->queueListWidget->addItem(item);
        ui->queueListWidget->setItemWidget(item, line);
    }
    for (const auto &song : *player->getQueue()) {
        auto *songWidget = new QueueSongItem(song, this);

        auto *item = new QListWidgetItem(ui->queueListWidget);
        item->setSizeHint(songWidget->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(song));
        ui->queueListWidget->addItem(item);
        ui->queueListWidget->setItemWidget(item, songWidget);
    }
}

void MainWindow::updatePlaylists() {
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

void MainWindow::updatePlaylist(const Playlist &playlist) const {
    for (int i = 0; i < ui->playlistTabs->count(); ++i) {
        if (auto *playlistView = qobject_cast<QListWidget *>(ui->playlistTabs->widget(i))) {
            auto tabPlaylist = playlistView->property("playlistUuid").toUuid();
            if (tabPlaylist == playlist.getUuid()) {
                qDebug() << "Updating playlist:" << playlist.getName();
                playlistView->clear();
                for (const auto &song : playlist.getSongs()) {
                    auto *songItem = new QListWidgetItem(song.getFilename(), playlistView);
                    songItem->setData(Qt::UserRole, QVariant::fromValue(song));
                }
                break;
            }
        }
    }
}

void MainWindow::onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
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

void MainWindow::onMetadataChanged() const {
    // update main song image, descriptor etc.
    auto player = Player::getInstance();
    auto data = player->metaData();

    auto title = data.stringValue(QMediaMetaData::Title);
    auto artist = data.stringValue(QMediaMetaData::ContributingArtist);
    auto thumbnail = data.value(QMediaMetaData::ThumbnailImage).value<QImage>();
    auto duration = data.value(QMediaMetaData::Duration).toInt();

    ui->songNameLabel->setText(title);
    ui->artistNameLabel->setText(artist);
    ui->coverLabel->setPixmap(QPixmap::fromImage(thumbnail));
    ui->durationLabel->setText(msToString(duration));

    // auto r = getSongThumbnail(player->source().toString());
    // qDebug() << "Thumbnail: " << r;
    // ui->coverLabel->setPixmap(r);
}

void MainWindow::toggleLoop() const {
    auto player = Player::getInstance();
    player->setLoop(!player->isLooping());
    ui->loopButton->setFlat(!ui->loopButton->isFlat());
}

void MainWindow::changePlayPauseIcon() const {
    auto player = Player::getInstance();
    if (player->isPlaying() == QMediaPlayer::PlayingState) {
        ui->playButton->setIcon(QIcon(":/resources/pause.svg"));
    } else {
        ui->playButton->setIcon(QIcon(":/resources/play.svg"));
    }
}

void MainWindow::createPlaylistButtonClicked() {
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

void MainWindow::onQueueItemRightClicked(const QPoint &pos) {
    auto player = Player::getInstance();

    QListWidgetItem *item = ui->queueListWidget->itemAt(pos);
    if (!item) return;  // Ignore if no item was clicked

    QMenu menu(this);

    auto *addToPlaylistMenu = menu.addMenu("Add to Playlist");

    auto playlists = player->getPlaylists();
    for (const auto &playlist : playlists) {
        QAction *action = addToPlaylistMenu->addAction(playlist.getName());
        connect(action, &QAction::triggered, [playlist, item, player]() {
            player->addToPlaylist(item, playlist);
        });
    }

    // Add other context menu actions
    menu.addSeparator();
    // menu.addAction("Remove from Queue", [this, item]() {
    //     removeFromQueue(item);
    // });

    menu.exec(ui->queueListWidget->mapToGlobal(pos));
}

void MainWindow::onPlaylistSelected(const QListWidgetItem *item) const {
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
        auto *songItem = new QListWidgetItem(song.getFilename(), playlistView);
        songItem->setData(Qt::UserRole, QVariant::fromValue(song));
    }

    ui->playlistTabs->addTab(playlistView, playlist.getName());
    ui->playlistTabs->setCurrentWidget(playlistView);
}

void MainWindow::onPlaylistTabCloseRequested(const int index) const {
    auto *widget = ui->playlistTabs->widget(index);
    ui->playlistTabs->removeTab(index);
    delete widget;
}