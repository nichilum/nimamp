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

    Queue& operator=(const Queue &other) {
        if (this == &other) return *this; // Handle self-assignment

        // Copy all members from `other` to `this`
        this->queue = other.queue;
        this->priorityQueue = other.priorityQueue;
        this->layout = other.layout;
        // Add more member copying as needed

        return *this;
    }
};

