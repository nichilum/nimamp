#include "../headers/transport_widget.hpp"

#include <QMediaMetaData>

#include "../headers/player.hpp"
#include "ui_TransportWidget.h"

TransportWidget::TransportWidget(QWidget *parent) : QWidget(parent), ui(new Ui::TransportWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    // buttons
    connect(ui->playButton, &QPushButton::clicked, player, &Player::togglePlayPause);
    connect(ui->nextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->prevButton, &QPushButton::clicked, player, &Player::previous);
    connect(ui->loopButton, &QPushButton::clicked, this, &TransportWidget::toggleLoop);

    // seek slider
    connect(player, &QMediaPlayer::positionChanged, this, &TransportWidget::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &TransportWidget::updateSeekDuration);
    connect(ui->seekSlider, &QSlider::sliderReleased, this, &TransportWidget::seekToReleasedPosition);

    connect(player, &QMediaPlayer::metaDataChanged, this, &TransportWidget::onMetadataChanged);
    connect(player, &QMediaPlayer::playingChanged, this, &TransportWidget::changePlayPauseIcon);
}

TransportWidget::~TransportWidget() {
    delete ui;
}

void TransportWidget::updateSeekSlider(const qint64 position) const {
    if (ui->seekSlider->isSliderDown()) {
        return;
    }
    ui->seekSlider->setValue(static_cast<int>(position));

    auto formattedTime = msToString(position);

    ui->currentTimeLabel->setText(formattedTime);
}

void TransportWidget::updateSeekDuration(const qint64 duration) const {
    ui->seekSlider->setRange(0, static_cast<int>(duration));
}

void TransportWidget::seekToReleasedPosition() const {
    auto player = Player::getInstance();
    player->setPosition(ui->seekSlider->value());
}

void TransportWidget::onMetadataChanged() const {
    // update main song image, descriptor etc.
    auto player = Player::getInstance();
    auto data = player->metaData();

    auto title = data.stringValue(QMediaMetaData::Title);
    auto artist = data.stringValue(QMediaMetaData::ContributingArtist);
    auto thumbnail = data.value(QMediaMetaData::ThumbnailImage).value<QImage>();
    if (thumbnail.isNull()) {
        thumbnail = QImage(":/resources/empty_cover.jpg");
    }
    thumbnail = thumbnail.scaled(QSize(100, 100), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto duration = data.value(QMediaMetaData::Duration).toInt();

    ui->songNameLabel->setText(title);
    ui->artistNameLabel->setText(artist);
    ui->coverLabel->setPixmap(QPixmap::fromImage(thumbnail));
    ui->durationLabel->setText(msToString(duration));

    // auto r = getSongThumbnail(player->source().toString());
    // qDebug() << "Thumbnail: " << r;
    // ui->coverLabel->setPixmap(r);
}

void TransportWidget::toggleLoop() const {
    auto player = Player::getInstance();
    player->setLoop(!player->isLooping());
    ui->loopButton->setFlat(!ui->loopButton->isFlat());
}

void TransportWidget::changePlayPauseIcon() const {
    auto player = Player::getInstance();
    if (player->isPlaying() == QMediaPlayer::PlayingState) {
        ui->playButton->setIcon(QIcon(":/resources/pause.svg"));
    } else {
        ui->playButton->setIcon(QIcon(":/resources/play.svg"));
    }
}