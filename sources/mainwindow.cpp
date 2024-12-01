#include "../headers/mainwindow.hpp"

#include <QFileDialog>

#include "../headers/player.hpp"
#include "../headers/song_item.hpp"
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
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

    // seek slider
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateSeekDuration);
    connect(ui->seekSlider, &QSlider::sliderReleased, this, &MainWindow::seekToReleasedPosition);

    // update queue
    connect(player, &Player::queueChanged, this, &MainWindow::updateQueue);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::onRowsMoved);

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);
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
}

void MainWindow::updateSeekDuration(const qint64 duration) const {
    ui->seekSlider->setRange(0, static_cast<int>(duration));
}

void MainWindow::seekToReleasedPosition() {
    auto player = Player::getInstance();
    player->setPosition(ui->seekSlider->value());
}

void MainWindow::updateVolume(int volume) const {
    auto player = Player::getInstance();
    player->setVolume(volume / 100.);
}

void MainWindow::updateQueue() {
    auto player = Player::getInstance();
    ui->listWidget->clear();

    for (const auto &song : *player->getPriorityQueue()) {
        auto *songWidget = new SongItem(song.getFilename(), this);

        auto *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(songWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, songWidget);
    }
    if (!player->isPriorityQueueEmpty()) {  // hline between queues
        auto line = new QFrame();
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        auto *item = new QListWidgetItem(ui->listWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setSizeHint(line->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, line);
    }
    for (const auto &song : *player->getQueue()) {
        auto *songWidget = new SongItem(song.getFilename(), this);

        auto *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(songWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, songWidget);
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