#include "albummodel.h"

void AlbumModel::insert(int index, QString &name)
{
    if(index < 0 || (uint) index > mAlbums->size()){
        return;
    }

    if(!name.isEmpty()){
        emit beginInsertRows(QModelIndex(), index, index);
        std::unique_ptr<Album> album;
        album->setName("Agregado");
        mAlbums->push_back(std::move(album));
        emit endInsertRows();
    }

    emit countChanged(mAlbums->size());

}


void AlbumModel::append(QString &name)
{
    insert(mAlbums->size(), name);
}

AlbumModel::AlbumModel(QObject *parent)
    :QAbstractListModel(parent),
      mDb(DatabaseManager::instance()),
    /*,
      mAlbums(mDb.mAlbumDao.getAllRecords())
          */
      mAlbums(new std::vector<std::unique_ptr<Album>>())
{
    std::unique_ptr<Album> album1(new Album);
    std::unique_ptr<Album> album2(new Album);
    std::unique_ptr<Album> album3(new Album);
    std::unique_ptr<Album> album4(new Album);
    std::unique_ptr<Album> album5(new Album);
    std::unique_ptr<Album> album6(new Album);

    album1->setId(1);
    album2->setId(2);
    album3->setId(3);
    album4->setId(4);
    album5->setId(5);
    album6->setId(6);

    album1->setName("Paul");
    album2->setName("Petolonio");
    album3->setName("Petronila");
    album4->setName("Paulina");
    album5->setName("Polo");
    album6->setName("Paula");

    mAlbums->push_back(std::move(album1));
    mAlbums->push_back(std::move(album2));
    mAlbums->push_back(std::move(album3));
    mAlbums->push_back(std::move(album4));
    mAlbums->push_back(std::move(album5));
    mAlbums->push_back(std::move(album6));
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAlbums->size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || (uint)row > mAlbums->size()){
        return QVariant();
    }

    const Album &album = *(mAlbums->at(index.row()));
    switch(role){
    case Roles::IdRole:
        return album.getId();
    case Roles::NameRole:
        return album.getName();
    default:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> AlbumModel::AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}
