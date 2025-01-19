#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QLoggingCategory>
#include <QMetaType>
#include <QNetworkProxyFactory>

#include "../include/key_events.hpp"
#include "../include/mainwindow.hpp"
#include "../include/player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<Song>("Song");
    qRegisterMetaType<Playlist>("Playlist");
    qRegisterMetaType<QVector<Song>>("QVector<Song>");
    qRegisterMetaType<QVector<Playlist>>("QVector<Playlist>");
    qRegisterMetaType<Player>("Player");

    QNetworkProxyFactory::setUseSystemConfiguration(false);
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

    MainWindow window;
    window.show();

    return QApplication::exec();
}
