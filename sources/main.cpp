#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QMetaType>
#include <QLoggingCategory>

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

    QLoggingCategory::setFilterRules("qt.gui.imageio=false");

    // key events
    auto *keyFilter = new GlobalKeyFilter();
    app.installEventFilter(keyFilter);

    // font
    auto fontId = QFontDatabase::addApplicationFont(":/resources/inter.ttf");
    auto fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);

    // style sheet
    QFile styleFile(":/qss/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    app.setStyleSheet(style);

    auto player = Player::getInstance();
    player->setVolume(0.);  // gehoerschutz

    MainWindow window;
    window.show();

    return QApplication::exec();
}
