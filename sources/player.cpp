#include "../headers/player.hpp"

#include <iostream>

void Player::print(QString message) {
    std::cout << message.toStdString() << std::endl;
}