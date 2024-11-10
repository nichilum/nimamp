#include "../headers/player.hpp"

#include <iostream>
#include <QDebug>

Player::Player() {
    setAudioOutput(&audioOutput);
}

void Player::addPlaylist(const Playlist& playlist) {
    playlists.push_back(playlist);
}

void Player::playPlaylist(const QString &name) {
    for (const auto& playlist : playlists) {
        if (playlist.getName() == name) {
            currentPlaylist = playlist;

            queue.clear();
            queue = playlist.getSongs();

            std::cout << "Playing playlist: " << name.toStdString() << std::endl;
            std::cout << "Songs in queue: " << queue.size() << std::endl;
            qDebug() << queue;

            setSource(queue.front());
            play();
            return;
        }
    }
}

void Player::next() {
    if (!currentPlaylist.has_value()) {
        return;
    }

    auto songs = currentPlaylist->getSongs();
    if (songs.empty()) {
        return;
    }

    queue.pop_front();
    auto song = queue.front();
    setSource(song);
    play();
}

