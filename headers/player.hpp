#pragma once
#include <QMediaPlayer>

class Player : QMediaPlayer {
private:
    QVector<QUrl> queue;
    
};
