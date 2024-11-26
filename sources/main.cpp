#include <QApplication>
#include <QDir>

#include "../headers/mainwindow.hpp"
#include "../headers/player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<Song>("Song");

    auto player = Player::getInstance();
    player->setVolume(0.1);  // gehoerschutz

    MainWindow window;
    window.show();

    return QApplication::exec();
}
