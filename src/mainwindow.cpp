#include "../include/mainwindow.hpp"

#include <ui_PlaylistViewWidget.h>

#include <QFileDialog>
#include <QSplitter>
#include <QTextEdit>

#include "../include/player.hpp"
#include "../include/queue_widget.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    queueWidget = new QueueWidget(this);
    playlistViewWidget = new PlaylistViewWidget(this);
    transportWidget = new TransportWidget(this);
    playlistTabsWidget = new PlaylistTabsWidget(playlistViewWidget, this);
    metadataWidget = new MetadataWidget(this);
    equalizerWidget = new EqualizerWidget(this);

    leftSplitter = new QSplitter(Qt::Vertical);
    leftSplitter->addWidget(transportWidget);
    leftSplitter->addWidget(equalizerWidget);
    leftSplitter->addWidget(metadataWidget);
    leftSplitter->setStretchFactor(0, 1);
    leftSplitter->setStretchFactor(1, 2);
    leftSplitter->setStretchFactor(2, 3);

    rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(playlistViewWidget);
    rightSplitter->addWidget(playlistTabsWidget);
    rightSplitter->setStretchFactor(0, 1);
    rightSplitter->setStretchFactor(1, 2);

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(queueWidget);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 2);
    mainSplitter->setStretchFactor(2, 3);

    ui->centralwidget->layout()->addWidget(mainSplitter);

    // menu buttons
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);
    connect(ui->actionFilesToQueue, &QAction::triggered, this, &MainWindow::openFileDialog);
}

MainWindow::~MainWindow() {
    delete ui;
    delete queueWidget;
    delete playlistViewWidget;
    delete transportWidget;
    delete leftSplitter;
    delete rightSplitter;
    delete mainSplitter;
}

/**
 * Opens a folder dialog and adds the selected folder to the queue
 */
void MainWindow::openFolderDialog() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        return;
    }

    Player::getInstance()->addFolderToQueue(dir);
}

/**
 * Opens a file dialog and adds the selected files to the queue
 */
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
