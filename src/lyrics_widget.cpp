#include "../include/lyrics_widget.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMediaMetaData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScrollArea>
#include <QScrollBar>

#include "../include/player.hpp"
#include "ui_LyricsWidget.h"

LyricsWidget::LyricsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LyricsWidget) {
    ui->setupUi(this);
    auto player = Player::getInstance();

    manager = new QNetworkAccessManager();

    label = new QLabel("No lyrics found :(");
    label->setObjectName("lyricsLabel");
    label->setWordWrap(true);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    label->setAlignment(Qt::AlignCenter);

    ui->lyricsScrollArea->setWidget(label);
    ui->lyricsScrollArea->setWidgetResizable(true);

    connect(player, &Player::metaDataChanged, this, &LyricsWidget::loadLyrics);

    connect(manager, &QNetworkAccessManager::finished,
            [this](QNetworkReply *reply) {
                QString lyrics = "No lyrics found :(";  // default text
                if (!reply->error()) {
                    QString answer = reply->readAll();
                    auto doc = QJsonDocument::fromJson(answer.toUtf8());
                    if (!doc.isNull()) {
                        if (doc.isObject()) {
                            QJsonObject obj = doc.object();
                            if (obj.contains("lyrics")) {
                                QJsonValue value = obj.value("lyrics");
                                if (value.isString()) {
                                    lyrics = "\n\n\n\n\n\n\n" + value.toString() + "\n\n\n\n\n\n\n";
                                }
                            }
                        }
                    }
                }

                label = new QLabel(lyrics);
                label->setObjectName("lyricsLabel");
                label->setWordWrap(true);
                label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
                label->setAlignment(Qt::AlignCenter);

                ui->lyricsScrollArea->setWidget(label);
                ui->lyricsScrollArea->setWidgetResizable(true);
            });

    connect(player, &QMediaPlayer::positionChanged, [this, player](qint64 position) {
        QScrollBar *scrollBar = ui->lyricsScrollArea->verticalScrollBar();
        int maxScroll = scrollBar->maximum();
        int scrollValue = (position * maxScroll) / (player->duration() + 1);
        scrollBar->setValue(scrollValue);
    });
}

LyricsWidget::~LyricsWidget() {
    delete manager;
    delete label;
    delete ui;
}

/**
 * Fetches the lyrics for the currently playing track.
 */
void LyricsWidget::loadLyrics() {
    auto player = Player::getInstance();

    auto title = player->metaData().value(QMediaMetaData::Title).toString();
    auto artist = player->metaData().value(QMediaMetaData::ContributingArtist).toString();

    QUrl url("https://api.lyrics.ovh/v1/" + artist + "/" + title);
    QNetworkRequest request(url);

    manager->get(request);
}