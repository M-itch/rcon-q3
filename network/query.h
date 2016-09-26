#ifndef QUERY_H
#define QUERY_H

#include <QDateTime>
#include <QUdpSocket>

class Query : public QObject
{
    Q_OBJECT
public:
    Query(const QString& server,
          int port,
          QObject* parent = 0);
    int getPing();
    void send(QByteArray command);
signals:
    void receive(QByteArray output);
    void connected();
private slots:
    void readPendingDatagrams();
private:
    QDateTime lastRequest;
    QDateTime firstResponse;
    QUdpSocket socket;
    const QByteArray Q3_PREFIX = "\xFF\xFF\xFF\xFF";
};

#endif // QUERY_H
