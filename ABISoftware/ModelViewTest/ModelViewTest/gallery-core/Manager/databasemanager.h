#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <memory>

#include <QString>

#include <Daos/albumdao.h>
#include <Daos/picturedao.h>

class QSqlQuery;
class QSqlDatabase;

const QString DATABASE_FILENAME = "gallery.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

    static void debugQuery(const QSqlQuery& query);

protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);
private:
    std::unique_ptr<QSqlDatabase> mDatabase;

public://Add your DAO's as public members
const AlbumDao mAlbumDao;
const PictureDao mPictureDao;

};

#endif // DATABASEMANAGER_H
