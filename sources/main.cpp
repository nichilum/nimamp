#include <QDir>
#include <QGuiApplication>
#include <QMediaMetaData>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringListModel>
#include <iostream>

#include "headers/player.hpp"
#include "headers/queue_model.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    auto player = Player::getInstance();
    player->setVolume(0.1);  // gehoerschutz

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("player", player);
    engine.rootContext()->setContextProperty("queueModel", player->getQueue());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("nimamp", "Main");

    return app.exec();
}
