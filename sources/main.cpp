#include <QApplication>
#include <QDir>

#include "../headers/key_events.hpp"
#include "../headers/mainwindow.hpp"
#include "../headers/player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<Song>("Song");
    qRegisterMetaType<Playlist>("Playlist");
    GlobalKeyFilter *keyFilter = new GlobalKeyFilter();
    app.installEventFilter(keyFilter);

    auto player = Player::getInstance();
    player->setVolume(0.);  // gehoerschutz

    MainWindow window;
    window.show();

    return QApplication::exec();
}
