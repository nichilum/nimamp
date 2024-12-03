#include "../headers/metadata_widget.hpp"

#include <QMediaMetaData>

#include "../headers/player.hpp"
#include "ui_MetadataWidget.h"

MetadataWidget::MetadataWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MetadataWidget) {
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto player = Player::getInstance();
    connect(player, &QMediaPlayer::metaDataChanged, this, &MetadataWidget::onMetadataChanged);
}

MetadataWidget::~MetadataWidget() {
    delete ui;
}

void MetadataWidget::onMetadataChanged() const {
    // TODO: do we really need another title and artist label??
    auto player = Player::getInstance();
    auto metadata = player->metaData();
    int row = 0;
    for (const auto &key : metadata.keys()) {
        auto value = metadata.stringValue(key);
        if (!value.isEmpty()) {
            ui->tableWidget->setRowCount(row + 1);
            QTableWidgetItem *keyItem = new QTableWidgetItem(QMediaMetaData::metaDataKeyToString(key));
            QTableWidgetItem *valueItem = new QTableWidgetItem(value);
            ui->tableWidget->setItem(row, 0, keyItem);
            ui->tableWidget->setItem(row, 1, valueItem);
            row++;
        }
    }
}