#pragma once

#include <QListWidgetItem>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MetadataWidget;
}
QT_END_NAMESPACE

class MetadataWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MetadataWidget(QWidget *parent = nullptr);
    ~MetadataWidget() override;

   private:
    Ui::MetadataWidget *ui;
    void onMetadataChanged() const;
};