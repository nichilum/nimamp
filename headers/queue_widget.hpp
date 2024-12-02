#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class QueueWidget;
}
QT_END_NAMESPACE

class QueueWidget : public QWidget {
    Q_OBJECT

   public:
    explicit QueueWidget(QWidget *parent = nullptr);
    ~QueueWidget() override;

   private:
    Ui::QueueWidget *ui;
    static void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void onQueueItemRightClicked(const QPoint &pos);

   public slots:
    void updateQueue();
};