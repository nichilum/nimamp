#pragma once

#include <QMainWindow>

#include "metadata_widget.hpp"
#include "playlist.hpp"
#include "playlist_tabs_widget.hpp"
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

   private:
    Ui::MainWindow *ui;
    QueueWidget *queueWidget;
    PlaylistViewWidget *playlistViewWidget;
    TransportWidget *transportWidget;
    PlaylistTabsWidget *playlistTabsWidget;
    MetadataWidget *metadataWidget;

    void openFolderDialog();
    void openFileDialog();
    static void updateVolume(int volume);
};
