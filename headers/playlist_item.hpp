#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PlaylistItem;
}
QT_END_NAMESPACE

class PlaylistItem : public QWidget {
    Q_OBJECT
   public:
    QString name;
    explicit PlaylistItem(const Playlist &playlist, QWidget *parent = nullptr);

    ~PlaylistItem() override;

   signals:
    void removeRequested(const QString &songName);

   private:
    Ui::PlaylistItem *ui;
};