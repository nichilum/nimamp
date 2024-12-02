#include "../headers/mainwindow.hpp"

#include <ui_PlaylistViewWidget.h>

#include <QFileDialog>

#include "../headers/player.hpp"
#include "../headers/queue_widget.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    queueWidget = new QueueWidget(this);
    playlistViewWidget = new PlaylistViewWidget(this);
    transportWidget = new TransportWidget(this);

    ui->queueWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->queueWidgetPlaceholder->layout()->addWidget(queueWidget);
    ui->playlistViewWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistViewWidgetPlaceholder->layout()->addWidget(playlistViewWidget);
    ui->transportWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->transportWidgetPlaceholder->layout()->addWidget(transportWidget);

    auto player = Player::getInstance();

    // set volume slider
    ui->volumeSlider->setValue(static_cast<int>(player->getVolume() * 100.));

    // buttons
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);

    // tabs
    connect(ui->playlistTabs, &QTabWidget::tabCloseRequested, this, &MainWindow::onPlaylistTabCloseRequested);
    connect(player, &Player::playlistChanged, this, &MainWindow::updatePlaylist);
    connect(playlistViewWidget->getUi()->playlistListWidget, &QListWidget::itemClicked, this, &MainWindow::onPlaylistSelected);
}

MainWindow::~MainWindow() {
    delete ui;
    delete queueWidget;
    delete playlistViewWidget;
    delete transportWidget;
}

void MainWindow::openFolderDialog() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    Player::getInstance()->addFolderToQueue(dir);
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