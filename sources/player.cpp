#include "../headers/player.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QVariantList>
#include <iostream>

Player *Player::instance = nullptr;

Player::Player() {
    setAudioOutput(&audioOutput);

    connect(this, &QMediaPlayer::mediaStatusChanged, this, &Player::songEnded);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Player::saveQueue);

    // auto songList = settings.value("queue").toList();
    // QVector<Song> loadedQueue;
    // for (const QVariant &songVariant : songList) {
    //     if (songVariant.canConvert<Song>()) {
    //         loadedQueue.append(songVariant.value<Song>());
    //     }
    // }
    // qDebug() << "Loaded songs: " << loadedQueue;
}

/*
 * Add Playlist to playlist vector
 */
void Player::addPlaylist(const Playlist &playlist) {
    playlists.push_back(playlist);
    emit playlistsChanged();
}

/*
 * replace queue with playlist
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

/*
 * prepend queue with playlist
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

        setSource(queue.front().getUrl());
        play();
    }
}

/*
 * remove playlist from playlists
 */
void Player::removePlaylist(const Playlist &playlist) {
    auto it = std::ranges::find(playlists, playlist.getUuid(), &Playlist::getUuid);

    if (it != playlists.end()) {
        playlists.erase(it);
        emit playlistsChanged();
    }
}

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

    qDebug() << "Added folder to queue: " << queue;
}

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

void Player::clearQueue() {
    queue.clear();
    emit queueChanged();
}

void Player::addToPlaylist(const Song &song, const Playlist &playlist) {
    auto it = std::ranges::find(playlists, playlist.getUuid(), &Playlist::getUuid);

    if (it != playlists.end()) {
        it->addSong(song);
        emit playlistChanged(*it);
        qDebug() << "Added song:" << song.getFilename() << "to playlist:" << it->getName();
    } else {
        qDebug() << "Playlist not found!";
    }
}

/**
 * Set the media source of the player and immediately start playing it.
 * This overwrites the queue and the current song but not the priority queue.
 * @param song The Song to play
 */
void Player::playSong(const Song &song) {
    addToHistory();
    setSource(song.url);
    play();
}

void Player::playSongFromQueue(const Song &song) {
    clearQueueUpToSong(song);
    addToHistory();
    setSource(song.url);
    play();
}

void Player::queueSong(const Song &song) {
    queue.push_back(song);
    emit queueChanged();
}

void Player::setLoop(const bool loop) {
    qInfo() << "Setting loop to " << loop;
    if (loop) {
        setLoops(Infinite);
    } else {
        setLoops(Once);
    }
}

void Player::clearQueueUpToSong(const Song &song) {
    if (auto it = std::ranges::find(queue, song); it != queue.end()) {
        queue.erase(queue.begin(), ++it);
    }

    emit queueChanged();
}

void Player::removeSongFromQueue(const Song &song) {
    if (auto it = std::ranges::find(queue, song); it != queue.end()) {
        queue.erase(it);
    }

    emit queueChanged();
}

Player *Player::getInstance() {
    if (instance == nullptr) {
        instance = new Player();
    }
    return instance;
}

void Player::songEnded() {
    if (mediaStatus() == EndOfMedia) {
        next();
    }
}

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

void Player::addToHistory() {
    if (!source().isEmpty()) {
        history.append(Song(source()));
    }
}

// Saving and Loading

void Player::saveQueue() {
    QVariantList songList;
    for (const Song &song : queue) {
        songList.append(QVariant::fromValue(song));
    }
    settings.setValue("queue", songList);
}