#pragma once
#include <QAudioOutput>
#include <QListWidgetItem>
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
    QStack<Song> history;

    QSettings settings;

   public:
    void addPlaylist(const Playlist &playlist);
    void addToPlaylist(const Song &song, const Playlist &playlist);
    void savePlayer();
    void loadPlayer();
    void playPlaylist(const Playlist &playlist);
    void queuePlaylist(const Playlist &playlist);
    void removePlaylist(const Playlist &playlist);
    void addFolderToPlaylist(const QString &directory, const Playlist &playlist);
    void addFolderToQueue(const QString &directory);
    void playSong(const Song &song);
    void playSongFromQueue(const Song &song);
    void next();
    void previous();
    void clearQueue();
    void queueSong(const Song &song);
    void setVolume(const float volume) { audioOutput.setVolume(volume); }
    void setLoop(bool loop);
    void clearQueueUpToSong(const Song &song);
    void removeSongFromQueue(const Song &song);
    void addToHistory();

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] QVector<Song> *getQueue() { return &queue; }
    [[nodiscard]] bool isQueueEmpty() const { return queue.isEmpty(); }
    [[nodiscard]] bool isLooping() const { return loops() == Infinite; }
    [[nodiscard]] QVector<Song> getQueueCopy() const { return queue; }
    [[nodiscard]] QVector<Song> getHistory() const { return history.toVector(); }

    static Player *getInstance();

    void togglePlayPause();

    friend QDataStream &operator<<(QDataStream &out, const Player &player) {
        out << player.history << player.queue << player.playlists << player.getVolume() << player.isLooping() << player.source() << static_cast<float>(player.position());
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Player &player) {
        auto volume = 0.f;
        auto loop = false;
        auto position = 0.f;
        auto source = QUrl();

        in >> player.history >> player.queue >> player.playlists >> volume >> loop >> source >> position;

        player.setVolume(volume);
        player.setLoop(loop);
        player.setSource(source);

        connect(&player, &QMediaPlayer::mediaStatusChanged, [position](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::LoadedMedia) {
                getInstance()->setPosition(static_cast<qint64>(position));
            }
            disconnect(getInstance(), &QMediaPlayer::mediaStatusChanged, nullptr, nullptr);
        });

        return in;
    }

   public slots:
    void songEnded();

   signals:
    void queueChanged();
    void playlistsChanged();
    void playlistChanged(const Playlist &playlist);
};