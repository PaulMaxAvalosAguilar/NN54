#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVector>
#include <memory>

#include "Manager/databasemanager.h"
#include "album.h"

class AlbumModel : public QAbstractListModel
{

    Q_OBJECT
public:

    Q_PROPERTY(int count NOTIFY countChanged)
    Q_INVOKABLE void insert(int index, QString &name);
    Q_INVOKABLE void append(QString &name);
    //Q_INVOKABLE void remove(int index);
    //Q_INVOKABLE void clear();

signals:
    void countChanged(const int count);

public:
    enum Roles{
        IdRole = Qt::UserRole +1,
        NameRole,
    };

    AlbumModel(QObject *parent= 0);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;



private:
    DatabaseManager &mDb;
    std::unique_ptr<std::vector<std::unique_ptr<Album>>> mAlbums;
};

#endif // ALBUMMODEL_H
