#include "../headers/mainwindow.hpp"

#include <ui_PlaylistViewWidget.h>

#include <QFileDialog>
#include <QInputDialog>
#include <QMediaMetaData>

#include "../headers/player.hpp"
#include "../headers/playlist_item.hpp"
#include "../headers/queue_song_item.hpp"
#include "../headers/queue_widget.hpp"
#include "../headers/utils.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    queueWidget = new QueueWidget(this);
    playlistViewWidget = new PlaylistViewWidget(this);

    ui->queueWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->queueWidgetPlaceholder->layout()->addWidget(queueWidget);
    ui->playlistViewWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistViewWidgetPlaceholder->layout()->addWidget(playlistViewWidget);

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

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);

    // tabs
    connect(ui->playlistTabs, &QTabWidget::tabCloseRequested, this, &MainWindow::onPlaylistTabCloseRequested);
    connect(player, &Player::playlistChanged, this, &MainWindow::updatePlaylist);
    connect(playlistViewWidget->getUi()->playlistListWidget, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSelected);

    connect(player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetadataChanged);
    connect(player, &QMediaPlayer::playingChanged, this, &MainWindow::changePlayPauseIcon);
}

MainWindow::~MainWindow() {
    delete ui;
    delete queueWidget;
    delete playlistViewWidget;
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

void MainWindow::seekToReleasedPosition() const {
    auto player = Player::getInstance();
    player->setPosition(ui->seekSlider->value());
}

void MainWindow::updateVolume(const int volume) {
    auto player = Player::getInstance();
    player->setVolume(static_cast<float>(volume) / 100);
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

void MainWindow::onPlaylistSelected(const QListWidgetItem *item) const {
    auto player = Player::getInstance();
    auto playlists = player->getPlaylists();

    auto playlist = item->data(Qt::UserRole).value<Playlist>();
    auto uuid = playlist.getUuid();
    auto it = std::ranges::find_if(playlists, [&uuid](const Playlist &p) {
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