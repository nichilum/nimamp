#include "../headers/queue.hpp"
#include "../headers/song_item.hpp"

Queue::Queue(QWidget* parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    this->setLayout(layout);

    renderQueue();
}

void Queue::queueSong(const Song& song) {
    queue.push_back(song);
    renderQueue();
}

void Queue::queueSongs(const QVector<Song>& songs) {
    queue.append(songs);
    renderQueue();
}

void Queue::queuePrioritySong(const Song& song) {
    priorityQueue.push_back(song);
    renderQueue();
}
void Queue::clearQueue() {
    queue.clear();
    renderQueue();
}

void Queue::renderQueue() {
    // Clear the layout
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget(); // Delete the widget
        delete child;           // Delete the layout item
    }

    // Add updated items from the queue
    for (int i = 0; i < queue.size(); ++i) {
        SongItem *songItem = new SongItem(queue[i].filename, i, this);

        // Handle remove requests
        connect(songItem, &SongItem::removeRequested, this, &Queue::removeSong);

        layout->addWidget(songItem);
    }

    layout->addStretch();
}
