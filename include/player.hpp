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
    QVector<Song> originalQueue;
    QStack<Song> history;

    QSettings settings;

    bool shuffled;

    /**
     * True if the player is dirty (set on first load)
     */
    bool dirty;

   public:
    void addPlaylist(const Playlist &playlist);
    void addToPlaylist(const Song &song, const Playlist &playlist);
    void removeSongFromPlaylist(const Song &song, const Playlist &playlist);
    void sortPlaylist(const QString sortingType, const Playlist &playlist);
    void moveSongInPlaylist(const Playlist &playlist, int from, int to);
    void savePlayer();
    void loadPlayer();
    void playPlaylist(const Playlist &playlist);
    void queuePlaylist(const Playlist &playlist);
    void removePlaylist(const Playlist &playlist);
    void addFolderToPlaylist(const QString &directory, const Playlist &playlist);
    void addFilesToPlaylist(const QStringList &filePaths, const Playlist &playlist);
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
    void toggleShuffleQueue();

    [[nodiscard]] QAudioOutput *getAudioOutput() { return &audioOutput; }
    [[nodiscard]] float getVolume() const { return audioOutput.volume(); }
    [[nodiscard]] QVector<Playlist> getPlaylists() const { return playlists; }
    [[nodiscard]] QVector<Song> *getQueue() { return &queue; }
    [[nodiscard]] bool isQueueEmpty() const { return queue.isEmpty(); }
    [[nodiscard]] bool isLooping() const { return loops() == Infinite; }
    [[nodiscard]] bool isShuffled() const { return shuffled; }
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
            auto player = getInstance();
            if (!player->dirty && status == QMediaPlayer::LoadedMedia) {
                player->setPosition(static_cast<qint64>(position));
                player->dirty = true;
            }
        });

        return in;
    }

   public slots:
    void songEnded();

   signals:
    /**
     * Emitted when the queue changes
     */
    void queueChanged();

    /**
     * Emitted when the playlists (in the player) change
     */
    void playlistsChanged();

    /**
     * Emitted when the content of a playlist changes
     * @param playlist The playlist that changed
     */
    void playlistChanged(const Playlist &playlist);
};