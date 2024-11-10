#pragma once
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

#include "playlist.hpp"

class Player : public QMediaPlayer {
Q_OBJECT
private:
    QAudioOutput audioOutput;
    QVector<Playlist> playlists;
    std::optional<Playlist> currentPlaylist;
    QVector<QUrl> queue;
public:
    Player();
    void addPlaylist(const Playlist& playlist);
    void playPlaylist(const QString& name);
    Q_INVOKABLE void next();
    Q_INVOKABLE void setVolume(const float volume) { audioOutput.setVolume(volume); }

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] Q_INVOKABLE float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] std::optional<Playlist> getCurrentPlaylist() const { return currentPlaylist; }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
};
