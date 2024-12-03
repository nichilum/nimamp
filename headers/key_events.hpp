#pragma once

#include <QKeyEvent>

#include "player.hpp"

class GlobalKeyFilter : public QObject {
   protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Space) {
                Player::getInstance()->togglePlayPause();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }
};