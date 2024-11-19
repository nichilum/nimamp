#include "mainwindow.hpp"
#include "ui_MainWindow.h"
#include "headers/player.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto player = Player::getInstance();
    connect(ui->playButton, &QPushButton::clicked, player, &MainWindow::togglePlayPause);
    connect(ui->nextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->prevButton, &QPushButton::clicked, player, &Player::next); //TODO: implement previous
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::togglePlayPause() {
    auto player = Player::getInstance();
    if (player->isPlaying() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}
