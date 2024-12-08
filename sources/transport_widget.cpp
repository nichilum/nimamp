#include "../headers/transport_widget.hpp"

#include <QMediaMetaData>

#include "../headers/player.hpp"
#include "ui_TransportWidget.h"

TransportWidget::TransportWidget(QWidget *parent) : QWidget(parent), ui(new Ui::TransportWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    // buttons
    connect(ui->transportplayButton, &QPushButton::clicked, player, &Player::togglePlayPause);
    connect(ui->transportnextButton, &QPushButton::clicked, player, &Player::next);
    connect(ui->transportprevButton, &QPushButton::clicked, player, &Player::previous);
    connect(ui->transportloopButton, &QPushButton::clicked, this, &TransportWidget::toggleLoop);
    connect(ui->transportshuffleButton, &QPushButton::clicked, this, &TransportWidget::toggleShuffle);

    // seek slider
    connect(player, &QMediaPlayer::positionChanged, this, &TransportWidget::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &TransportWidget::updateSeekDuration);
    connect(ui->transportseekSlider, &QSlider::sliderReleased, this, &TransportWidget::seekToReleasedPosition);

    connect(player, &QMediaPlayer::metaDataChanged, this, &TransportWidget::onMetadataChanged);
    connect(player, &QMediaPlayer::playingChanged, this, &TransportWidget::changePlayPauseIcon);
}

TransportWidget::~TransportWidget() {
    delete ui;
}

/**
 * Set the seek slider to the current position of the song
 * and update the current time label
 * @param position The position of the song in milliseconds
 */
void TransportWidget::updateSeekSlider(const qint64 position) const {
    if (ui->transportseekSlider->isSliderDown()) {
        return;
    }
    ui->transportseekSlider->setValue(static_cast<int>(position));

    auto formattedTime = msToString(position);

    ui->transportcurrentTimeLabel->setText(formattedTime);
}

/**
 * Set the range of the seek slider to the duration of the song
 * @param duration The duration of the song in milliseconds
 */
void TransportWidget::updateSeekDuration(const qint64 duration) const {
    ui->transportseekSlider->setRange(0, static_cast<int>(duration));
}

/**
 * Set the player position to the value of the seek slider
 */
void TransportWidget::seekToReleasedPosition() const {
    auto player = Player::getInstance();
    player->setPosition(ui->transportseekSlider->value());
}

/**
 * Set the metadata of the song to the labels
 */
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

    if (title != "") {
        ui->transportsongNameLabel->setText(title);
    } else {
        ui->transportsongNameLabel->setText(player->source().fileName());
    }

    ui->transportartistNameLabel->setText(artist);
    ui->transportcoverLabel->setPixmap(QPixmap::fromImage(thumbnail));
    ui->transportdurationLabel->setText(msToString(duration));
}

/**
 * Toggle the loop state of the player
 */
void TransportWidget::toggleLoop() const {
    auto player = Player::getInstance();
    player->setLoop(!player->isLooping());
    if (player->isLooping()) {
        ui->transportloopButton->setStyleSheet("QPushButton { background-color: white; border-radius: 15px }");
        ui->transportloopButton->setIcon(QIcon(":/resources/repeat_b.svg"));
    } else {
        ui->transportloopButton->setStyleSheet("QPushButton { background-color: transparent; }");
        ui->transportloopButton->setIcon(QIcon(":/resources/repeat.svg"));
    }
}

/**
 * Change the play/pause icon based on the player state
 */
void TransportWidget::changePlayPauseIcon() const {
    auto player = Player::getInstance();
    if (player->isPlaying() == QMediaPlayer::PlayingState) {
        ui->transportplayButton->setIcon(QIcon(":/resources/pause.svg"));
    } else {
        ui->transportplayButton->setIcon(QIcon(":/resources/play_b.svg"));
    }
}

/**
 * Toggle the shuffle state of the player
 */
void TransportWidget::toggleShuffle() const {
    auto player = Player::getInstance();
    player->toggleShuffleQueue();
    if (player->isShuffled()) {
        ui->transportshuffleButton->setStyleSheet("QPushButton { background-color: white; border-radius: 15px }");
        ui->transportshuffleButton->setIcon(QIcon(":/resources/shuffle_b.svg"));
    } else {
        ui->transportshuffleButton->setStyleSheet("QPushButton { background-color: transparent; }");
        ui->transportshuffleButton->setIcon(QIcon(":/resources/shuffle.svg"));
    }
}