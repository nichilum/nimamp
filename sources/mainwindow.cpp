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

    ui->queueWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->queueWidgetPlaceholder->layout()->addWidget(queueWidget);
    ui->playlistViewWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistViewWidgetPlaceholder->layout()->addWidget(playlistViewWidget);
    ui->transportWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->transportWidgetPlaceholder->layout()->addWidget(transportWidget);
    ui->playlistTabsWidgetPlaceholder->setLayout(new QVBoxLayout);
    ui->playlistTabsWidgetPlaceholder->layout()->addWidget(playlistTabsWidget);

    auto player = Player::getInstance();

    ui->volumeSlider->setValue(static_cast<int>(player->getVolume() * 100.));

    // buttons
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

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

void MainWindow::updateVolume(const int volume) {
    auto player = Player::getInstance();
    player->setVolume(static_cast<float>(volume) / 100);
}
