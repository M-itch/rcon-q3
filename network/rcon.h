#ifndef RCON_H
#define RCON_H

#include <QString>
#include "query.h"
#include "server.h"

class Rcon : public Query
{
    Q_OBJECT
public:
    Rcon(const Server server, QObject* parent = 0);
    void setPassword(QByteArray password);
    void send(QByteArray command);
private:
    QByteArray rconPassword;
};

#endif // RCON_H
