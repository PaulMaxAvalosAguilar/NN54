#include "albumdao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "album.h"
#include "Manager/databasemanager.h"

using namespace std;

AlbumDao::AlbumDao(QSqlDatabase& database) :
    QObject(),
    mDatabase(database)
{
}

void AlbumDao::init() const
{
    if (!mDatabase.tables().contains("Album")) {
        QSqlQuery query(mDatabase);
        query.exec("CREATE TABLE Album (id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT)");
        DatabaseManager::debugQuery(query);
    }
}

void AlbumDao::addRecord(Album& record)
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO Album (name)  VALUES  (:name)");
    query.bindValue(":name", record.getName());
    query.exec();
    record.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
    emit addedRecord();
}

void AlbumDao::updateRecord(Album& record)
{
    QSqlQuery query(mDatabase);
    query.prepare("UPDATE Album SET (id) = (:name) WHERE  id = (:id)");
    query.bindValue(":name", record.getName());
    query.bindValue(":id", record.getId());
    query.exec();
    DatabaseManager::debugQuery(query);
    emit updatedRecord();
}

void AlbumDao::removeRecord(int recordId)
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Album WHERE id = (:id)");
    query.bindValue(":id", recordId);
    query.exec();
    DatabaseManager::debugQuery(query);
    emit removedRecord();
}

unique_ptr<vector<unique_ptr<Album>>> AlbumDao::getRecord(int recordId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM Album WHERE id = (:id)");
    query.bindValue(":id", recordId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Album>>> list(new vector<unique_ptr<Album>>());
    while(query.next()) {
        std::unique_ptr<Album> album(new Album());
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());
        list->push_back(move(album));
    }
    return list;
}

void AlbumDao::removeAllRecords()
{
    QSqlQuery query("DELETE FROM Album", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
    emit removedAllRecords();
}

unique_ptr<vector<unique_ptr<Album>>> AlbumDao::getAllRecords() const
{
    QSqlQuery query("SELECT * FROM Album", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Album>>> list(new vector<unique_ptr<Album>>());
    while(query.next()) {
        unique_ptr<Album> album(new Album());
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());
        list->push_back(move(album));
    }
    return list;
}


void AlbumDao::createIndexonColumnName(){
    QSqlQuery query("create index name on Album(name)", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
}

