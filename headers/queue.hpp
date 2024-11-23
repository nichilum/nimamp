#pragma once

#include <QObject>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "song.hpp"

class Queue final : public QWidget {
    Q_OBJECT

   private:
    QVBoxLayout *layout;
    QVector<Song> queue;
    QVector<Song> priorityQueue;

   public:
    explicit Queue(QWidget *parent = nullptr);
    void queueSong(const Song &song);
    void queueSongs(const QVector<Song> &songs);
    void queuePrioritySong(const Song &song);
    void clearQueue();
    void removeSong(int index) { queue.remove(index); }

    void renderQueue();

    [[nodiscard]] QVector<Song> *getQueue() { return &queue; }
    [[nodiscard]] QVector<Song> *getPriorityQueue() { return &priorityQueue; }
};