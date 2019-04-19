#ifndef ALBUMDAO_H
#define ALBUMDAO_H
#include <memory>
#include <vector>
#include <QObject>

class QSqlDatabase;
class Album;

class AlbumDao : public QObject
{

Q_OBJECT

public:
    explicit AlbumDao(QSqlDatabase& database);
    void init() const;

    void addRecord(Album& record);
    void updateRecord(Album& record);
    void removeRecord(int recordId);
    std::unique_ptr<std::vector<std::unique_ptr<Album>>> getRecord(int recordId) const;

    void removeAllRecords();
    std::unique_ptr<std::vector<std::unique_ptr<Album>>> getAllRecords() const;

    void createIndexonColumnName();

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
