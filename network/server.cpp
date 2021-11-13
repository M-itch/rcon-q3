#include "server.h"

Server::Server(QString ip, quint16 port, QByteArray rconPassword)
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

quint16 Server::getPort() const {
    return port;
}

void Server::setPort(quint16 value) {
    port = value;
}

QByteArray Server::getRconPassword() const {
    return rconPassword;
}

void Server::setRconPassword(const QByteArray &value) {
    rconPassword = value;
}

QJsonObject Server::toJSON() const {
    QJsonObject json;
    json["ip"] = ip;
    json["port"] = port;
    json["password"] = QString(rconPassword.toBase64());
    return json;
}

Server Server::fromJson(const QJsonObject& object, bool oldFormat) {
    return Server(object["ip"].toString(),
                  static_cast<quint16>(object["port"].toInt()),
                  oldFormat ? object["password"].toString().toUtf8() : QByteArray::fromBase64(object["password"].toString().toUtf8()));
}
