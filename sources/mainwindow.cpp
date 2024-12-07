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
    playlistTabsWidget = new PlaylistTabsWidget(playlistViewWidget, this);
    metadataWidget = new MetadataWidget(this);

    ui->queueWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->queueWidgetPlaceholder->layout()->addWidget(queueWidget);
    ui->queueWidgetPlaceholder->layout()->setContentsMargins(0, 0, 0, 0);

    ui->playlistViewWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistViewWidgetPlaceholder->layout()->addWidget(playlistViewWidget);
    ui->playlistViewWidgetPlaceholder->layout()->setContentsMargins(0, 0, 0, 0);

    ui->transportWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->transportWidgetPlaceholder->layout()->addWidget(transportWidget);
    ui->transportWidgetPlaceholder->layout()->setContentsMargins(0, 0, 0, 0);

    ui->playlistTabsWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistTabsWidgetPlaceholder->layout()->addWidget(playlistTabsWidget);
    ui->playlistTabsWidgetPlaceholder->layout()->setContentsMargins(0, 0, 0, 0);

    ui->metadataWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->metadataWidgetPlaceholder->layout()->addWidget(metadataWidget);
    ui->metadataWidgetPlaceholder->layout()->setContentsMargins(0, 0, 0, 0);

    auto player = Player::getInstance();

    ui->volumeSlider->setValue(static_cast<int>(player->getVolume() * 100.));

    // menu buttons
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);
    connect(ui->actionFilesToQueue, &QAction::triggered, this, &MainWindow::openFileDialog);

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);
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

void MainWindow::openFileDialog() {
    QString fileFilter = "Audio Files (*.mp3 *.wav)";
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "Select an Audio File",
        "",
        fileFilter);

    auto player = Player::getInstance();
    for (QString filePath : filePaths) {
        player->queueSong(Song(filePath));
    }
}

void MainWindow::updateVolume(const int volume) {
    auto player = Player::getInstance();
    player->setVolume(static_cast<float>(volume) / 100);
}
