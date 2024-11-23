#pragma once

#include <QPushButton>
#include <QWidget>
#include <QObject>

#include "ui_SongItem.h"

class SongItem : public QWidget {
    Q_OBJECT
   public:
    explicit SongItem(const QString &songName, int index, QWidget *parent = nullptr)
        : QWidget(parent), ui(new Ui_SongItem), songIndex(index) {
        ui->setupUi(this);
        ui->label->setText(songName);

        // Connect remove button
        connect(ui->pushButton, &QPushButton::clicked, [this]() {
            emit removeRequested(songIndex);
        });
    }

    ~SongItem() { delete ui; }

    signals:
     void removeRequested(int index);

private:
    Ui_SongItem *ui;
    int songIndex;
};