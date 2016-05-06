#include "server.h"

Server::Server(QString ip, int port, QByteArray rconPassword)
    : ip(ip),
      port(port),
      rconPassword(rconPassword) {
}

QString Server::getIp() const {
    return ip;
}

void Server::setIp(const QString &value) {
    ip = value;
}

int Server::getPort() const {
    return port;
}

void Server::setPort(int value) {
    port = value;
}

QByteArray Server::getRconPassword() const {
    return rconPassword;
}

void Server::setRconPassword(const QByteArray &value) {
    rconPassword = value;
}

QJsonObject Server::toJSON() {
    QJsonObject json;
    json["ip"] = ip;
    json["port"] = port;
    json["password"] = QString(rconPassword);
    return json;
}
