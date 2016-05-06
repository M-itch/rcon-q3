#include "query.h"

Query::Query(QString server, int port, QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost);
    socket->connectToHost(server, port, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(socket, SIGNAL(connected()), this, SIGNAL(connected()));
}

Query::~Query() {
    delete socket;
}

int Query::getPing() {
    return lastRequest.msecsTo(firstResponse);
}

void Query::send(QByteArray command) {
    lastRequest = QDateTime::currentDateTime();
    firstResponse = QDateTime();
    command.prepend(Q3_PREFIX);
    socket->write(command);
}

void Query::readPendingDatagrams() {
    QByteArray data;
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);
        data.append(datagram); // Combine message with multiple datagrams.
    }

    if (!data.isEmpty()) {
        if (firstResponse.isNull()) {
            firstResponse = QDateTime::currentDateTime();
        }

        emit receive(data);
    }
}
