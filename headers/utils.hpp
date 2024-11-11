#pragma once

#include <QVector>
#include <QUrl>
#include <QStringList>
#include <algorithm>

inline QStringList convertToQStringList(const QVector<QUrl>& urlVector) {
    QStringList stringList;
    std::transform(urlVector.begin(), urlVector.end(), std::back_inserter(stringList),
                   [](const QUrl &url) { return url.toString(); });
    return stringList;
}