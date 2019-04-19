#ifndef PICTUREDAO_H
#define PICTUREDAO_H
#include <memory>
#include <vector>
#include <QObject>

class QSqlDatabase;
class Picture;

class PictureDao : public QObject
{

Q_OBJECT

public:
    explicit PictureDao(QSqlDatabase& database);
    void init() const;

    void addRecord(Picture& record);
    void updateRecord(Picture& record);
    void removeRecord(int recordId);
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> getRecord(int recordId) const;

    void removeAllRecords();
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> getAllRecords() const;
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> getAllRecordsByAlbumId(int albumId) const;

    void createIndexonColumnAlbumId();
    void createIndexonColumnString();

signals:
    void addedRecord();
    void updatedRecord();
    void removedRecord();

    void updatedManyRecords();
    void removedAllRecords();

private:
    QSqlDatabase& mDatabase;
};

#endif // PICTUREDAO_H
