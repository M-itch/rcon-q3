#ifndef SERVER_H
#define SERVER_H

#include <QJsonObject>
#include <QString>

class Server
{
public:
    Server(QString ip = "", quint16 port = 28960, QByteArray rconPassword = "");
    QString getIp() const;
    void setIp(const QString &value);
    quint16 getPort() const;
    void setPort(quint16 value);
    QByteArray getRconPassword() const;
    void setRconPassword(const QByteArray &value);
    QJsonObject toJSON() const;
    static Server fromJson(const QJsonObject& object, bool oldFormat = false);
private:
    QString ip;
    quint16 port;
    QByteArray rconPassword;
};

#endif // SERVER_H
