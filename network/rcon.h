#ifndef RCON_H
#define RCON_H

#include <QString>
#include "query.h"
#include "server.h"

class Rcon : public Query
{
    Q_OBJECT
public:
    Rcon(Server server, QObject* parent = 0);
    void setPassword(QByteArray password);
    Server* getServer();
    void send(QByteArray command);
private:
    Server server;
};

#endif // RCON_H
