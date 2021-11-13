#ifndef STATUS_H
#define STATUS_H

#include <QMap>
#include <QString>
#include "player.h"

struct Status
{
    QMap<QString, QString> variables;
    std::vector<Player> players;
};

#endif // STATUS_H
