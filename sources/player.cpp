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

    auto songList = settings.value("queue").toList();
    QVector<Song> loadedQueue;
    for (const QVariant &songVariant : songList) {
        if (songVariant.canConvert<Song>()) {
            loadedQueue.append(songVariant.value<Song>());
        }
    }
    qDebug() << "Loaded songs: " << loadedQueue;
}

void Player::addPlaylist(const Playlist &playlist) {
    playlists.push_back(playlist);
}

void Player::playPlaylist(const QString &name) {
    for (const auto &playlist : playlists) {
        if (playlist.getName() == name) {
            queue.clear();
            queue.append(playlist.getSongs());

            std::cout << "Playing playlist: " << name.toStdString() << std::endl;
            std::cout << "Songs in queue: " << queue.size() << std::endl;
            qDebug() << queue;

            setSource(queue.front().getUrl());
            play();
            return;
        }
    }

    emit queueChanged();
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
        return;
    }
    queue.pop_front();
    auto song = queue.front();
    qDebug() << "Current queue: " << queue;
    setSource(song.getUrl());
    play();

    emit queueChanged();
}

void Player::clearQueue() {
    queue.clear();
    emit queueChanged();
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
    emit queueChanged();
}

Player *Player::getInstance() {
    if (instance == nullptr) {
        instance = new Player();
    }
    return instance;
}

void Player::saveQueue() {
    QVariantList songList;
    for (const Song &song : queue) {
        songList.append(QVariant::fromValue(song));
    }
    settings.setValue("queue", songList);
}

void Player::songEnded() {
    if (mediaStatus() == EndOfMedia) {
        next();
    }
}

void Player::togglePlayPause() {
    auto player = Player::getInstance();
    if (player->isPlaying() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}