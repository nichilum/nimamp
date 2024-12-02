#pragma once

#include <QMainWindow>

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
    void openFolderDialog();
    void updateSeekSlider(qint64 position) const;
    void updateSeekDuration(qint64 duration) const;
    void seekToReleasedPosition();
    static void updateVolume(int volume);
    static void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void onMetadataChanged() const;
    void toggleLoop() const;
    void changePlayPauseIcon() const;
    void createPlaylistButtonClicked();

   public slots:
    void updateQueue();
    void updatePlaylists();
};
