#pragma once

#include <QListWidgetItem>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class EqualizerWidget;
}
QT_END_NAMESPACE

class EqualizerWidget : public QWidget {
    Q_OBJECT

   public:
    explicit EqualizerWidget(QWidget *parent = nullptr);
    ~EqualizerWidget() override;

   private:
    Ui::EqualizerWidget *ui;
    static void updateVolume(int volume);
};