#include "../headers/song_item.hpp"
#include "ui_SongItem.h"

SongItem::SongItem(const QString &songName, QWidget *parent)
    : QWidget(parent), ui(new Ui::SongItem) {
    ui->setupUi(this);
    ui->label->setText(songName);

    name = songName;

    // Connect remove button
    connect(ui->deleteButton, &QPushButton::clicked, [this, songName]() {
        emit removeRequested(songName);
    });
}

SongItem::~SongItem() {
    delete ui;
}