#include "headers/queue_model.hpp"

#include <QAbstractListModel>
#include <QList>
#include <QObject>

#include "headers/song.hpp"

void QueueModel::addSong(const Song &song) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
    m_songs.push_back(song);
    endInsertRows();
}

void QueueModel::addSongs(QList<Song> &&songs) {
    if (songs.isEmpty())
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + songs.size() - 1);
    m_songs.append(songs);
    endInsertRows();
}

void QueueModel::setSongs(QList<Song> &&songs) {
    if (songs.isEmpty())
        return;

    beginInsertRows(QModelIndex(), 0, songs.size() - 1);
    m_songs = songs;
    endInsertRows();
}
void QueueModel::clear() {
    beginResetModel();
    m_songs.clear();
    endResetModel();
}

int QueueModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_songs.count();
}

QVariant QueueModel::data(const QModelIndex &index, const int role) const {
    if (index.row() < 0 || index.row() >= m_songs.count())
        return QVariant();

    const Song &song = m_songs[index.row()];
    if (role == UrlRole)
        return song.getUrl();
    if (role == FilenameRole)
        return song.getFilename();

    return {};
}

QHash<int, QByteArray> QueueModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[FilenameRole] = "filename";
    return roles;
}

QVector<Song> QueueModel::getSongs() const {
    return m_songs;
}
