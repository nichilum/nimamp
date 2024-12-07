#include "../headers/equalizer_widget.hpp"

#include "../headers/player.hpp"
#include "ui_EqualizerWidget.h"

EqualizerWidget::EqualizerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::EqualizerWidget) {
    ui->setupUi(this);

    auto player = Player::getInstance();

    ui->volumeSlider->setValue(static_cast<int>(player->getVolume() * 100.));

    // volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &EqualizerWidget::updateVolume);
}

EqualizerWidget::~EqualizerWidget() {
    delete ui;
}

void EqualizerWidget::updateVolume(const int volume) {
    auto player = Player::getInstance();
    player->setVolume(static_cast<float>(volume) / 100);
}