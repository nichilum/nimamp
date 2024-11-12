#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>

#include "song.hpp"

class SongModel final : public QAbstractListModel {
    Q_OBJECT
   public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        FilenameRole
    };

    SongModel(QObject *parent = nullptr) : QAbstractListModel(parent){};

    void addSongs(QList<Song> &&songs) {
        if (songs.isEmpty())
            return;

        beginInsertRows(QModelIndex(), rowCount(), rowCount() + songs.size() - 1);
        m_songs.append(songs);
        endInsertRows();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_songs.count();
    }

    QVariant data(const QModelIndex &index, const int role) const override {
        if (index.row() < 0 || index.row() >= m_songs.count())
            return QVariant();

        const Song &song = m_songs[index.row()];
        if (role == UrlRole)
            return song.getUrl();
        if (role == FilenameRole)
            return song.getFilename();

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[UrlRole] = "url";
        roles[FilenameRole] = "filename";
        return roles;
    }

   private:
    QList<Song> m_songs;
};
