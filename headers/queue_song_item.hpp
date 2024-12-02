#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class QueueSongItem;
}
QT_END_NAMESPACE

class QueueSongItem : public QWidget {
    Q_OBJECT
   public:
    QString name;
    explicit QueueSongItem(const Song &song, QWidget *parent = nullptr);

    ~QueueSongItem() override;

   signals:
    void removeRequested(const QString &songName);

   private:
    Ui::QueueSongItem *ui;
};