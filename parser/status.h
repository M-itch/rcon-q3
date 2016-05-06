#ifndef STATUS_H
#define STATUS_H

#include <QList>
#include <QMap>
#include <QString>
#include "player.h"

class Status
{
public:
    Status();
    QMap<QString, QString> variables;
    QList<Player> players;
};

#endif // STATUS_H
