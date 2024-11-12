#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>

#include "song.hpp"

class QueueModel final : public QAbstractListModel {
    Q_OBJECT
   public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        FilenameRole
    };

    QueueModel(QObject *parent = nullptr) : QAbstractListModel(parent){};
    void addSong(const Song &song);
    void addSongs(QList<Song> &&songs);
    void setSongs(QList<Song> &&songs);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] QVector<Song> getSongs() const;

    [[nodiscard]] bool isEmpty() const {
        return m_songs.isEmpty();
    }

    /**
     * Pop first element of the queue and return the element after.
     * @return First Song after pop
     */
    Song dequeue() {
        m_songs.pop_front();
        return m_songs.front();
    }

   private:
    QVector<Song> m_songs;
};
