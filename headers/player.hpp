#pragma once
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

#include "playlist.hpp"
#include "queue_model.hpp"

class Player final : public QMediaPlayer {
    Q_OBJECT
   protected:
    Player();
    static Player *instance;

   private:
    QAudioOutput audioOutput;
    QVector<Playlist> playlists;
    QueueModel queue;
    QVector<Song> priorityQueue;

   public:
    void addPlaylist(const Playlist &playlist);
    void playPlaylist(const QString &name);                    // to queue
    Q_INVOKABLE void addFolderToQueue(const QUrl &directory);  // to
    Q_INVOKABLE void setMediaSource(const QUrl &url);          // delete queue not prio queue
    Q_INVOKABLE void next();
    Q_INVOKABLE void queueSong(const Song &song);  // to prio queue
    Q_INVOKABLE void setVolume(const float volume) { audioOutput.setVolume(volume); }

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] Q_INVOKABLE float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] Q_INVOKABLE QueueModel *getQueue() { return &queue; }

    static Player *getInstance();

   public slots:
    void songEnded();
};