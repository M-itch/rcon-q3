#include "rcon.h"

Rcon::Rcon(const Server server, QObject* parent)
    : Query(server.getIp(), server.getPort(), parent) {
    rconPassword = server.getRconPassword();
}

void Rcon::setPassword(QByteArray password) {
    rconPassword = password;
}

void Rcon::send(QByteArray command) {
    command.prepend(" ")
           .prepend(rconPassword)
           .prepend("rcon ");
    Query::send(command);
}
