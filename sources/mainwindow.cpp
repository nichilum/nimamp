#include "../headers/mainwindow.hpp"

#include <QFileDialog>

#include "../headers/player.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto player = Player::getInstance();
    connect(ui->playButton, &QPushButton::clicked, player, &Player::togglePlayPause);
    connect(ui->nextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->prevButton, &QPushButton::clicked, player, &Player::next);  // TODO: implement previous
    connect(ui->actionFolderToQueue, &QAction::triggered, this, &MainWindow::openFolderDialog);

    Queue *queue = new Queue(this);
    player->setQueue(*queue);
    ui->scrollArea->setWidget(player->getQueue());

    QWidget *scrollWidget = new QWidget(this);

    // Optionally set a layout for the scroll widget
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);

    // Add some content (e.g., QLabel) to the scroll widget
    for (int i = 0; i < 20; ++i) {
        layout->addWidget(new QLabel(QString("Label %1").arg(i + 1), scrollWidget));
    }

    // Set the scrollWidget as the widget for the scroll area
    ui->scrollArea->setWidget(scrollWidget);

    // Enable automatic resizing of the scroll widget
    scrollWidget->setMinimumSize(scrollWidget->sizeHint());
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