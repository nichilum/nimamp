#pragma once

#include <QWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class TransportWidget;
}
QT_END_NAMESPACE

class TransportWidget : public QWidget {
    Q_OBJECT

   public:
    explicit TransportWidget(QWidget *parent = nullptr);
    ~TransportWidget() override;

   private:
    Ui::TransportWidget *ui;
    void updateSeekSlider(qint64 position) const;
    void updateSeekDuration(qint64 duration) const;
    void seekToReleasedPosition() const;
    void onMetadataChanged() const;
    void toggleLoop() const;
    void changePlayPauseIcon() const;
};