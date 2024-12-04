#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "song.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class SongItem;
}
QT_END_NAMESPACE

enum class SongItemType {
    Queue,
    Playlist
};

class SongItem : public QWidget {
    Q_OBJECT
   public:
    QString name;
    explicit SongItem(const Song &song, SongItemType songItemType, QWidget *parent = nullptr);

    ~SongItem() override;

   private:
    Ui::SongItem *ui;
};