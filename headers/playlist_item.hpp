#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "../headers/playlist.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class PlaylistItem;
}
QT_END_NAMESPACE

class PlaylistItem : public QWidget {
    Q_OBJECT
   public:
    explicit PlaylistItem(const Playlist &playlist, QWidget *parent = nullptr);

    ~PlaylistItem() override;

   private:
    Ui::PlaylistItem *ui;
    Playlist playlist;
    void playPlaylist() const;
    void queuePlaylist() const;
    void removePlaylist() const;
};