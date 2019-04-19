#include "picture.h"

Picture::Picture()
{

}

int Picture::getId() const
{
    return id;
}

void Picture::setId(int value)
{
    id = value;
}

int Picture::getAlbumId() const
{
    return albumId;
}

void Picture::setAlbumId(int value)
{
    albumId = value;
}

QString Picture::getString() const
{
    return string;
}

void Picture::setString(const QString &value)
{
    string = value;
}
