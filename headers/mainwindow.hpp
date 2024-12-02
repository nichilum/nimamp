#pragma once

#include <QListWidgetItem>
#include <QMainWindow>

#include "playlist.hpp"
#include "queue_widget.hpp"

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

   private:
    Ui::MainWindow *ui;
    QueueWidget *queueWidget;
    void openFolderDialog();
    void updateSeekSlider(qint64 position) const;
    void updateSeekDuration(qint64 duration) const;
    void seekToReleasedPosition() const;
    static void updateVolume(int volume);
    void onMetadataChanged() const;
    void toggleLoop() const;
    void changePlayPauseIcon() const;
    void createPlaylistButtonClicked();
    void onPlaylistSelected(const QListWidgetItem *item) const;
    void onPlaylistTabCloseRequested(int index) const;

   public slots:
    void updatePlaylists();
    void updatePlaylist(const Playlist &playlist) const;
};
