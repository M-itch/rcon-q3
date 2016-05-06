#include "rcon.h"
#include <utility> // std::move

Rcon::Rcon(Server server, QObject* parent)
    : Query(server.getIp(), server.getPort(), parent),
      server(std::move(server)) {
}

void Rcon::setPassword(QByteArray password) {
    server.setRconPassword(password);
}

Server* Rcon::getServer() {
    return &server;
}

void Rcon::send(QByteArray command) {
    command.prepend(" ")
           .prepend(server.getRconPassword())
           .prepend("rcon ");
    Query::send(command);
}
