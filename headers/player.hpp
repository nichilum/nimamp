#pragma once
#include <QMediaPlayer>
#include <QObject>

class Player : public QMediaPlayer {
Q_OBJECT
public:
    Q_INVOKABLE void print(QString message);
};
