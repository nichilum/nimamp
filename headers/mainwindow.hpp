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
    static void setMediaPosition(int position);
    void updateSlider(qint64 position) const;
    void updateDuration(qint64 duration) const;
    void seekToReleasedPosition();

   public slots:
    void updateQueue();
};
