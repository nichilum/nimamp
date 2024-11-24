#include "../headers/mainwindow.hpp"

#include <QFileDialog>

#include "../headers/player.hpp"
#include "../headers/song_item.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    // buttons
    connect(ui->playButton, &QPushButton::clicked, player, &Player::togglePlayPause);
    connect(ui->nextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->prevButton, &QPushButton::clicked, player, &Player::next);  // TODO: implement previous
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

    // seek slider
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->seekSlider, &QSlider::sliderReleased, this, &MainWindow::seekToReleasedPosition);

    // update queue
    connect(player, &Player::queueChanged, this, &MainWindow::updateQueue);
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

void MainWindow::updateSlider(const qint64 position) const {
    if (ui->seekSlider->isSliderDown()) {
        return;
    }
    ui->seekSlider->setValue(static_cast<int>(position));
}

void MainWindow::updateDuration(const qint64 duration) const {
    ui->seekSlider->setRange(0, static_cast<int>(duration));
}

void MainWindow::seekToReleasedPosition() {
    auto player = Player::getInstance();
    player->setPosition(ui->seekSlider->value());
}

void MainWindow::updateQueue() {
    auto player = Player::getInstance();
    ui->listWidget->clear();

    for (const auto &song : *player->getQueue()) {
        auto *songWidget = new SongItem(song.getFilename(), this);

        auto *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(songWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, songWidget);
    }
}