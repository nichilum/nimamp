#include "../include/lyrics_widget.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMediaMetaData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScrollArea>

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

    QScrollArea *scrollArea = ui->lyricsScrollArea;
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);

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
                                    lyrics = value.toString();
                                }
                            }
                        }
                    }
                }

                label = new QLabel(lyrics);
                label->setObjectName("lyricsLabel");
                label->setWordWrap(true);
                label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

                QScrollArea *scrollArea = ui->lyricsScrollArea;
                scrollArea->setWidget(label);
                scrollArea->setWidgetResizable(true);
            });
}

LyricsWidget::~LyricsWidget() {
    delete manager;
    delete label;
    delete ui;
}

void LyricsWidget::loadLyrics() {
    auto player = Player::getInstance();

    auto title = player->metaData().value(QMediaMetaData::Title).toString();
    auto artist = player->metaData().value(QMediaMetaData::ContributingArtist).toString();

    QUrl url("https://api.lyrics.ovh/v1/" + artist + "/" + title);
    QNetworkRequest request(url);

    manager->get(request);
}