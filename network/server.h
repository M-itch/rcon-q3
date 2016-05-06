#ifndef SERVER_H
#define SERVER_H

#include <QJsonObject>
#include <QString>

class Server
{
public:
    Server(QString ip = "", int port = 28960, QByteArray rconPassword = "");
    QString getIp() const;
    void setIp(const QString &value);
    int getPort() const;
    void setPort(int value);
    QByteArray getRconPassword() const;
    void setRconPassword(const QByteArray &value);
    QJsonObject toJSON();
private:
    QString ip;
    int port;
    QByteArray rconPassword;
};

#endif // SERVER_H
