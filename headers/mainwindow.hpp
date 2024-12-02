#pragma once

#include <QMainWindow>

#include "playlist.hpp"
#include "playlist_view_widget.hpp"
#include "queue_widget.hpp"
#include "transport_widget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void onPlaylistSelected(const QListWidgetItem *item) const;

   private:
    Ui::MainWindow *ui;
    QueueWidget *queueWidget;
    PlaylistViewWidget *playlistViewWidget;
    TransportWidget *transportWidget;

    void openFolderDialog();
    static void updateVolume(int volume);
    void onPlaylistTabCloseRequested(int index) const;

   public slots:
    void updatePlaylist(const Playlist &playlist) const;
};
