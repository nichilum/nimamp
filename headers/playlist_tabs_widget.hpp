#pragma once

#include <QListWidgetItem>
#include <QWidget>

#include "../headers/playlist_view_widget.hpp"
#include "playlist.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class PlaylistTabsWidget;
}
QT_END_NAMESPACE

class PlaylistTabsWidget : public QWidget {
    Q_OBJECT

   public:
    explicit PlaylistTabsWidget(PlaylistViewWidget *playlistViewWidget, QWidget *parent = nullptr);
    ~PlaylistTabsWidget() override;

   private:
    Ui::PlaylistTabsWidget *ui;
    void onPlaylistTabCloseRequested(int index) const;
    void onPlaylistSelected(const QListWidgetItem *item) const;

   public slots:
    void updatePlaylist(const Playlist &playlist) const;
};