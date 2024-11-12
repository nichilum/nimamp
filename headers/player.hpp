#pragma once
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

#include "playlist.hpp"

class Player final : public QMediaPlayer {
    Q_OBJECT
   private:
    QAudioOutput audioOutput;
    QVector<Playlist> playlists;
    QVector<Song> queue;

   public:
    Player();
    void addPlaylist(const Playlist &playlist);
    void playPlaylist(const QString &name);
    Q_INVOKABLE void addFolderToQueue(const QUrl &directory);
    Q_INVOKABLE void setMediaSource(const QUrl &url);
    Q_INVOKABLE void next();
    Q_INVOKABLE void queueSong(const Song &song);
    Q_INVOKABLE void setVolume(const float volume) { audioOutput.setVolume(volume); }

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] Q_INVOKABLE float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] QVector<Song> getQueue() const { return queue; }

   public slots:
    void songEnded();
};
