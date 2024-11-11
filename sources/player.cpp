#include "../headers/player.hpp"

#include <iostream>
#include <QDebug>
#include <QDir>

Player::Player() {
    setAudioOutput(&audioOutput);

    connect(this, &QMediaPlayer::mediaStatusChanged, this, &Player::songEnded);
}

void Player::addPlaylist(const Playlist& playlist) {
    playlists.push_back(playlist);
}

void Player::playPlaylist(const QString &name) {
    for (const auto& playlist : playlists) {
        if (playlist.getName() == name) {
            queue.clear();
            queue = playlist.getSongs();

            std::cout << "Playing playlist: " << name.toStdString() << std::endl;
            std::cout << "Songs in queue: " << queue.size() << std::endl;
            qDebug() << queue;

            setSource(queue.front().getUrl());
            play();
            return;
        }
    }
}

void Player::addFolderToQueue(const QUrl &directory) {
    const QDir dir(directory.toLocalFile());
    QStringList files = dir.entryList(QStringList() << "*.wav" << "*.mp3", QDir::Files);
    for (const auto& filename : files) {
        auto song = Song(QUrl::fromLocalFile(dir.absoluteFilePath(filename)), filename);
        queue.push_back(song);
    }

    qDebug () << "Added folder to queue: " << queue;
}

void Player::next() {
    if (queue.isEmpty()) {
        return;
    }

    queue.pop_front();
    auto song = queue.front();
    setSource(song.getUrl());
    play();
}

/**
 * Set the media source of the player and immediately start playing it.
 * This overwrites the queue and the current song.
 * @param url The URL of the media source
 */
void Player::setMediaSource(const QUrl &url) {
    setSource(url);
    play();
}

void Player::queueSong(const Song &song) {
    queue.push_back(song);
}

void Player::songEnded() {
    if (mediaStatus() == EndOfMedia) {
        next();
    }
}
