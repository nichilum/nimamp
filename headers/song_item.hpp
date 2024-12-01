#pragma once

#include <QObject>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SongItem;
}
QT_END_NAMESPACE

class SongItem : public QWidget {
    Q_OBJECT
   public:
    QString name;
    explicit SongItem(const Song &song, QWidget *parent = nullptr);

    ~SongItem() override;

   signals:
    void removeRequested(const QString &songName);

   private:
    Ui::SongItem *ui;
};