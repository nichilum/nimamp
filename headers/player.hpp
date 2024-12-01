#pragma once
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>
#include <QSettings>
#include <QStack>

#include "playlist.hpp"

class Player final : public QMediaPlayer {
    Q_OBJECT
   protected:
    Player();
    static Player *instance;

   private:
    QAudioOutput audioOutput;
    QVector<Playlist> playlists;
    QVector<Song> queue;
    QVector<Song> priorityQueue;
    QStack<Song> history;

    QSettings settings;

   public:
    void addPlaylist(const Playlist &playlist);
    void saveQueue();
    void playPlaylist(const QString &name);
    void addFolderToQueue(const QString &directory);
    void playSong(const Song &song);
    void next();
    void previous();
    void clearQueue();
    void clearPriorityQueue();
    void queueSong(const Song &song);
    void queuePrioritySong(const Song &song);
    void setVolume(const float volume) { audioOutput.setVolume(volume); }

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] QVector<Song> *getQueue() { return &queue; }
    [[nodiscard]] QVector<Song> *getPriorityQueue() { return &priorityQueue; }
    [[nodiscard]] bool isQueueEmpty() { return queue.isEmpty(); }
    [[nodiscard]] bool isPriorityQueueEmpty() { return priorityQueue.isEmpty(); }

    static Player *getInstance();

    void togglePlayPause();

   public slots:
    void songEnded();

   signals:
    void queueChanged();
};