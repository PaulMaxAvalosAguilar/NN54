#ifndef PICTURE_H
#define PICTURE_H

#include <QString>

class Picture
{
public:
    Picture();

    int getId() const;
    void setId(int value);

    int getAlbumId() const;
    void setAlbumId(int value);

    QString getString() const;
    void setString(const QString &value);

private:
    int id;
    int albumId;
    QString string;
};

#endif // PICTURE_H
