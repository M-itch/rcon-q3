#include "query.h"

Query::Query(const QString& server, int port, QObject *parent) : QObject(parent) {
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(&socket, SIGNAL(connected()), this, SIGNAL(connected()));
    socket.connectToHost(server, port, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
}

int Query::getPing() {
    return lastRequest.msecsTo(firstResponse);
}

void Query::send(QByteArray command) {
    lastRequest = QDateTime::currentDateTime();
    firstResponse = QDateTime();
    command.prepend(Q3_PREFIX);
    socket.write(command);
}

void Query::readPendingDatagrams() {
    QByteArray data;
    while (socket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket.readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);
        data.append(datagram); // Combine message with multiple datagrams.
    }

    if (!data.isEmpty()) {
        if (firstResponse.isNull()) {
            firstResponse = QDateTime::currentDateTime();
        }

        emit receive(QString::fromLocal8Bit(data));
    }
}
