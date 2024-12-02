#include <QApplication>
#include <QDir>

#include "../headers/mainwindow.hpp"
#include "../headers/player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<Song>("Song");
    qRegisterMetaType<Playlist>("Playlist");

    auto player = Player::getInstance();
    player->setVolume(0.);  // gehoerschutz

    MainWindow window;
    window.show();

    return QApplication::exec();
}
