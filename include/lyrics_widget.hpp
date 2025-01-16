#pragma once

#include <QLabel>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LyricsWidget;
}
QT_END_NAMESPACE

class LyricsWidget : public QWidget {
    Q_OBJECT

   public:
    explicit LyricsWidget(QWidget *parent = nullptr);
    ~LyricsWidget() override;

   private:
    Ui::LyricsWidget *ui;
    QNetworkAccessManager *manager;
    QLabel *label;
    void loadLyrics();
};