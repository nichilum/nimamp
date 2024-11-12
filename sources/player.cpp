#include "../headers/player.hpp"

#include <QDebug>
#include <QDir>
#include <iostream>

Player *Player::instance = nullptr;

Player::Player() {
    setAudioOutput(&audioOutput);

    connect(this, &QMediaPlayer::mediaStatusChanged, this, &Player::songEnded);
}

void Player::addPlaylist(const Playlist &playlist) {
    playlists.push_back(playlist);
}

void Player::playPlaylist(const QString &name) {
    for (const auto &playlist : playlists) {
        if (playlist.getName() == name) {
            queue.clear();
            queue.setSongs(playlist.getSongs());

            std::cout << "Playing playlist: " << name.toStdString() << std::endl;
            std::cout << "Songs in queue: " << queue.getSongs().size() << std::endl;
            qDebug() << queue.getSongs();

            setSource(queue.getSongs().front().getUrl());
            play();
            return;
        }
    }
}

void Player::addFolderToQueue(const QUrl &directory) {
    const QDir dir(directory.toLocalFile());
    QStringList files = dir.entryList(QStringList() << "*.wav"
                                                    << "*.mp3",
                                      QDir::Files);
    for (const auto &filename : files) {
        auto song = Song(QUrl::fromLocalFile(dir.absoluteFilePath(filename)));
        queue.addSong(song);
    }

    qDebug() << "Added folder to queue: " << queue.getSongs();
}

void Player::next() {
    if (queue.isEmpty()) {
        return;
    }

    auto song = queue.dequeue();
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
    queue.addSong(song);
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
