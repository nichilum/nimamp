#include "../headers/player.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QVariantList>
#include <iostream>

Player *Player::instance = nullptr;

Player::Player() {
    setAudioOutput(&audioOutput);

    connect(this, &QMediaPlayer::positionChanged, this, &Player::songEnded);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Player::savePlayer);

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
 * Set the media source of the player and immediately start playing it.
 * This also adds the song to the history.
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
    if (auto it = std::find(queue.begin(), queue.end(), song); it != queue.end()) {
        queue.erase(queue.begin(), ++it);
    }

    emit queueChanged();
}

void Player::removeSongFromQueue(const Song &song) {
    if (auto it = std::find(queue.begin(), queue.end(), song); it != queue.end()) {
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
    if (position() >= duration()) {
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

void Player::savePlayer() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << *this;
    settings.setValue("player", data);
    settings.sync();
}

void Player::loadPlayer() {
    QByteArray data = settings.value("player").toByteArray();

    if (!data.isEmpty()) {
        QDataStream in(&data, QIODevice::ReadOnly);
        in >> *this;
    }
}