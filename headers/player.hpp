#pragma once
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>
#include <QSettings>

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
    QSettings settings;

   public:
    void addPlaylist(const Playlist &playlist);
    void saveQueue();
    void playPlaylist(const QString &name);                       // to queue
    Q_INVOKABLE void addFolderToQueue(const QString &directory);  // to
    Q_INVOKABLE void setMediaSource(const QUrl &url);             // delete queue not prio queue
    Q_INVOKABLE void next();
    Q_INVOKABLE void clearQueue();
    Q_INVOKABLE void queueSong(const Song &song);  // to prio queue
    Q_INVOKABLE void setVolume(const float volume) { audioOutput.setVolume(volume); }

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] Q_INVOKABLE float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] Q_INVOKABLE QVector<Song> *getQueue() { return &queue; }
    void setQueue(const QVector<Song> &queue) { this->queue = queue; }

    static Player *getInstance();


    static void togglePlayPause();

   public slots:
    void songEnded();

   signals:
    void queueChanged();
};