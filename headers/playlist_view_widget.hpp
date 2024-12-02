#pragma once

#include <QWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class PlaylistViewWidget;
}
QT_END_NAMESPACE

class PlaylistViewWidget : public QWidget {
    Q_OBJECT

   public:
    explicit PlaylistViewWidget(QWidget *parent = nullptr);
    ~PlaylistViewWidget() override;

    [[nodiscard]] Ui::PlaylistViewWidget *getUi() const { return ui; }

   private:
    Ui::PlaylistViewWidget *ui;
    void createPlaylistButtonClicked();

   public slots:
    void updatePlaylists();
};