#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "ui_SongItem.h"

class SongItem : public QWidget {
    Q_OBJECT
   public:
    explicit SongItem(const QString &songName, QWidget *parent = nullptr)
        : QWidget(parent), ui(new Ui_SongItem) {
        ui->setupUi(this);
        ui->label->setText(songName);

        // Connect remove button
        connect(ui->pushButton, &QPushButton::clicked, [this, songName]() {
            emit removeRequested(songName);
        });
    }

    ~SongItem() override { delete ui; }

   signals:
    void removeRequested(const QString &songName);

   private:
    Ui_SongItem *ui;
};