#include "album.h"


Album::Album()
{
}

int Album::getId() const
{
    return id;
}

void Album::setId(int value)
{
    id = value;
}

QString Album::getName() const
{
    return name;
}

void Album::setName(const QString &value)
{
    name = value;
}

