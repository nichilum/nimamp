#include "../include/player.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QRandomGenerator>
#include <QVariantList>
#include <iostream>

Player *Player::instance = nullptr;

Player::Player() {
    setAudioOutput(&audioOutput);

    shuffled = false;

    connect(this, &QMediaPlayer::mediaStatusChanged, this, &Player::songEnded);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Player::savePlayer);
    connect(this, &Player::queueChanged, this, [this]() {
        if (!shuffled) {
            originalQueue = queue;
        };
    });

    loadPlayer();
}

/**
 * Add a new playlist to the collection of playlists
 * @param playlist The playlist to add
 */
void Player::addPlaylist(const Playlist &playlist) {
    playlists.push_back(playlist);
    emit playlistsChanged();
}

/**
 * Replace the queue with a given playlist
 * @param playlist The playlist to queue
 */
void Player::playPlaylist(const Playlist &playlist) {
    auto p = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (p != playlists.end()) {
        clearQueue();

        queue.append(p->getSongs());
        emit queueChanged();

        setSource(queue.front().getUrl());
        play();
    }
}

/**
 * Add a playlist to the end of the queue
 * @param playlist The playlist to add
 */
void Player::queuePlaylist(const Playlist &playlist) {
    auto p = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (p != playlists.end()) {
        // append songs to front of queue
        auto playlistSongs = p->getSongs();
        std::reverse(playlistSongs.begin(), playlistSongs.end());
        for (const auto &song : playlistSongs) {
            queue.prepend(song);
        }
        emit queueChanged();
    }
}

/**
 * Remove a given playlist from the collection of playlists
 * @param playlist The playlist to remove
 */
void Player::removePlaylist(const Playlist &playlist) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        playlists.erase(it);
        emit playlistsChanged();
    }
}

/**
 * Add a folder to a playlist
 * @param directory The directory to add
 * @param playlist The playlist to add the folder to
 */
void Player::addFolderToPlaylist(const QString &directory, const Playlist &playlist) {
    const QDir dir(directory);
    QStringList files = dir.entryList(QStringList() << "*.wav"
                                                    << "*.mp3",
                                      QDir::Files);

    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        for (const auto &filename : files) {
            auto song = Song(QUrl::fromLocalFile(dir.absoluteFilePath(filename)));
            it->addSong(song);
        }

        emit playlistChanged(*it);
        qDebug() << "Added folder to playlist: " << playlist.getName();
    }
}

/**
 * Adds a list of files to a specified playlist.
 *
 * @param filePaths A list of file paths to be added to the playlist.
 * @param playlist The target playlist to which the files will be added.
 */
void Player::addFilesToPlaylist(const QStringList &filePaths, const Playlist &playlist) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        for (QString filePath : filePaths) {
            auto song = Song(QUrl::fromLocalFile(filePath));
            it->addSong(song);
        }

        emit playlistChanged(*it);
        qDebug() << "Added files to playlist: " << playlist.getName();
    }
}

/**
 * Add a folder to the queue
 * @param directory The directory to add
 */
void Player::addFolderToQueue(const QString &directory) {
    clearQueue();

    const QDir dir(directory);
    QStringList files = dir.entryList(QStringList() << "*.wav"
                                                    << "*.mp3",
                                      QDir::Files);
    for (const auto &filename : files) {
        auto song = Song(QUrl::fromLocalFile(dir.absoluteFilePath(filename)));
        queueSong(song);
    }
}

/**
 * Plays the next song in the queue.
 * If the queue is empty, the song is skipped to the end.
 */
void Player::next() {
    if (queue.isEmpty()) {
        setPosition(duration());  // skip song to end
        return;
    }
    addToHistory();
    auto song = queue.front();
    queue.pop_front();
    setSource(song.url);
    play();
    emit queueChanged();
}

/**
 * Plays the previous song in the history.
 * If the history is empty, the song is skipped to the beginning.
 */
void Player::previous() {
    if (history.isEmpty() || position() > duration() * 0.1) {
        setPosition(0);
        return;
    }
    auto song = history.back();
    history.pop_back();
    setSource(song.url);
    play();
}

/**
 * Clear the queue
 */
void Player::clearQueue() {
    queue.clear();
    emit queueChanged();
}

/**
 * Add a song to a playlist
 * @param song The song to add
 * @param playlist The playlist to add the song to
 */
void Player::addToPlaylist(const Song &song, const Playlist &playlist) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        it->addSong(song);
        emit playlistChanged(*it);
        qDebug() << "Added song:" << song.getFilename() << "to playlist:" << it->getName();
    } else {
        qDebug() << "Playlist not found!";
    }
}

/**
 * Remove a song from a playlist
 * @param song The song to remove
 * @param playlist The playlist to remove the song from
 */
void Player::removeSongFromPlaylist(const Song &song, const Playlist &playlist) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        it->removeSong(song);
        emit playlistChanged(*it);
    } else {
        qDebug() << "Playlist not found!";
    }
}

/**
 * Move a song in a playlist
 * @param playlist The playlist to move the song in
 * @param from The index of the song to move
 * @param to The index to move the song to
 */
void Player::moveSongInPlaylist(const Playlist &playlist, const int from, const int to) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });

    if (it != playlists.end()) {
        it->moveSong(from, to);
        emit playlistChanged(*it);
    } else {
        qDebug() << "Playlist not found!";
    }
}

/**
 * Set the media source of the player and immediately start playing it.
 * This also adds the song to the history.
 * @param song The Song to play
 */
void Player::playSong(const Song &song) {
    addToHistory();
    setSource(song.url);
    play();
}

/**
 * Play a song from the queue.
 * This also clears the queue up to the song.
 * @param song The song to play
 */
void Player::playSongFromQueue(const Song &song) {
    clearQueueUpToSong(song);
    addToHistory();
    setSource(song.url);
    play();
}

/**
 * Add a song to the end of the queue
 * @param song The song to add
 */
void Player::queueSong(const Song &song) {
    queue.push_back(song);
    emit queueChanged();
}

/**
 * Sets the loop mode of the player
 * @param loop Set to true to loop the current song indefinitely
 */
void Player::setLoop(const bool loop) {
    qInfo() << "Setting loop to " << loop;
    if (loop) {
        setLoops(Infinite);
    } else {
        setLoops(Once);
    }
}

/**
 * Clear the queue up to a given song.
 * If the song is not in the queue, nothing happens.
 * @param song The song to clear up to
 */
void Player::clearQueueUpToSong(const Song &song) {
    if (auto it = std::find(queue.begin(), queue.end(), song); it != queue.end()) {
        queue.erase(queue.begin(), ++it);
    }

    emit queueChanged();
}

/**
 * Remove a song from the queue
 * @param song The song to remove
 */
void Player::removeSongFromQueue(const Song &song) {
    if (auto it = std::find(queue.begin(), queue.end(), song); it != queue.end()) {
        queue.erase(it);
    }

    emit queueChanged();
}

/**
 * Get the instance of the player
 * @return Singleton instance of the player
 */
Player *Player::getInstance() {
    if (instance == nullptr) {
        instance = new Player();
    }
    return instance;
}

/**
 * Check if the current song has ended and play the next song if it has
 */
void Player::songEnded() {
    if (mediaStatus() == EndOfMedia) {
        next();
    }
}

/**
 * Toggle play/pause or play the next song if the player is stopped
 */
void Player::togglePlayPause() {
    if (source().isEmpty()) {
        next();
        return;
    }

    if (isPlaying() == PlayingState) {
        pause();
    } else {
        play();
    }
}

/**
 * Add current song to the history
 */
void Player::addToHistory() {
    if (!source().isEmpty()) {
        history.append(Song(source()));
    }
}

/**
 * Save the player state to the settings
 */
void Player::savePlayer() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << *this;
    settings.setValue("player", data);
    settings.sync();
}

/**
 * Load the player state from the settings
 */
void Player::loadPlayer() {
    QByteArray data = settings.value("player").toByteArray();

    if (!data.isEmpty()) {
        QDataStream in(&data, QIODevice::ReadOnly);
        in >> *this;
    }
}

/**
 * Toggles the shuffle mode for the playback queue.
 *
 * @note The function assumes that `originalQueue` holds the original
 * order of the playback queue and that `queue` is a modifiable container.
 *
 * @warning The shuffle operation uses a default random engine, which may
 * produce non-deterministic results on different runs.
 */
void Player::toggleShuffleQueue() {
    shuffled = !shuffled;

    if (shuffled) {
        auto rng = std::default_random_engine{};
        std::shuffle(std::begin(queue), std::end(queue), rng);
    } else {
        queue = originalQueue;
    }
    emit queueChanged();
}

/**
 * @brief Sorts a specified playlist based on the given sorting type.
 * @param sortingType A string representing the desired sorting criterion
 * (e.g., "title", "artist", "duration").
 * @param playlist The target playlist to be sorted.
 *
 * @note The function assumes that the `Playlist` class provides a method
 * to set the sorting type (`setSortingType`) and that `playlists` contains
 * valid `Playlist` objects.
 *
 * @warning The sorting behavior depends on the implementation of
 * `Playlist::setSortingType`. Ensure the sorting type string is valid
 * and supported.
 */
void Player::sortPlaylist(const QString sortingType, const Playlist &playlist) {
    auto it = std::find_if(playlists.begin(), playlists.end(), [&playlist](const Playlist &p) {
        return p.getUuid() == playlist.getUuid();
    });
    if (it != playlists.end()) {
        it->setSortingType(sortingType);
        emit playlistChanged(*it);
    } else {
        qDebug() << "Playlist not found!";
    }
}