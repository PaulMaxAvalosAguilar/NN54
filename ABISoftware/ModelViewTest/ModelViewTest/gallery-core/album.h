#ifndef ALBUM_H
#define ALBUM_H

#include <QString>

class Album
{

public:
    Album();

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

private:
    int id;
    QString name;
};

#endif // ALBUM_H
