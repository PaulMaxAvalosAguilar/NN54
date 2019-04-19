#include "picturedao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "picture.h"
#include "Manager/databasemanager.h"

using namespace std;

PictureDao::PictureDao(QSqlDatabase& database) :
    QObject(),
    mDatabase(database)
{
}

void PictureDao::init() const
{
    if (!mDatabase.tables().contains("Picture")) {
        QSqlQuery query(mDatabase);
        query.exec("CREATE TABLE Picture (id INTEGER PRIMARY KEY AUTOINCREMENT, albumId INTEGER,string TEXT)");
        DatabaseManager::debugQuery(query);
    }
}

void PictureDao::addRecord(Picture& record)
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO Picture (albumId,string)  VALUES  (:albumId,:string)");
    query.bindValue(":albumId", record.getAlbumId());
    query.bindValue(":string", record.getString());
    query.exec();
    record.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
    emit addedRecord();
}

void PictureDao::updateRecord(Picture& record)
{
    QSqlQuery query(mDatabase);
    query.prepare("UPDATE Picture SET (albumId,string) = (:albumId,:string) WHERE  id = (:id)");
    query.bindValue(":albumId", record.getAlbumId());
    query.bindValue(":string", record.getString());
    query.bindValue(":id", record.getId());
    query.exec();
    DatabaseManager::debugQuery(query);
    emit updatedRecord();
}

void PictureDao::removeRecord(int recordId)
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Picture WHERE id = (:id)");
    query.bindValue(":id", recordId);
    query.exec();
    DatabaseManager::debugQuery(query);
    emit removedRecord();
}

unique_ptr<vector<unique_ptr<Picture>>> PictureDao::getRecord(int recordId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM Picture WHERE id = (:id)");
    query.bindValue(":id", recordId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> list(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        std::unique_ptr<Picture> picture(new Picture());
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("albumId").toInt());
        picture->setString(query.value("string").toString());
        list->push_back(move(picture));
    }
    return list;
}

void PictureDao::removeAllRecords()
{
    QSqlQuery query("DELETE FROM Picture", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
    emit removedAllRecords();
}

unique_ptr<vector<unique_ptr<Picture>>> PictureDao::getAllRecords() const
{
    QSqlQuery query("SELECT * FROM Picture", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> list(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        unique_ptr<Picture> picture(new Picture());
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("albumId").toInt());
        picture->setString(query.value("string").toString());
        list->push_back(move(picture));
    }
    return list;
}

std::unique_ptr<std::vector<std::unique_ptr<Picture> > > PictureDao::getAllRecordsByAlbumId(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM Picture WHERE albumId = (:albumId)");
    query.bindValue(":albumId", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> list(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        std::unique_ptr<Picture> picture(new Picture());
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("albumId").toInt());
        picture->setString(query.value("string").toString());
        list->push_back(move(picture));
    }
    return list;
}

void PictureDao::createIndexonColumnAlbumId(){
    QSqlQuery query("create index albumId on Picture(albumId)", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
}

void PictureDao::createIndexonColumnString(){
    QSqlQuery query("create index string on Picture(string)", mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
}
