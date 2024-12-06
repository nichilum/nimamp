#include <QApplication>
#include <QDir>
#include <QMetaType>

#include "../headers/key_events.hpp"
#include "../headers/mainwindow.hpp"
#include "../headers/player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<Song>("Song");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<QVector<Song>>("QVector<Song>");
    qRegisterMetaType<QVector<Playlist>>("QVector<Playlist>");
    qRegisterMetaType<Player>("Player");

    // key events
    GlobalKeyFilter *keyFilter = new GlobalKeyFilter();
    app.installEventFilter(keyFilter);

    // style sheet
    // QFile styleFile(":/qss/style.qss");
    // styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    // QString style(styleFile.readAll());
    // app.setStyleSheet(style);

    auto player = Player::getInstance();
    player->setVolume(0.);  // gehoerschutz

    MainWindow window;
    window.show();

    return QApplication::exec();
}
