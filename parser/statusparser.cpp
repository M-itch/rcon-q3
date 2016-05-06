#include "statusparser.h"
#include <QStringListIterator>
#include <utility> // std::move

Status StatusParser::parse(QString data) {
    data = stripResponseHeader(data);
    QStringList playerList = data.split("\n");

    QString variableData = playerList.first();
    playerList.removeFirst();
    variableData.remove(0, 1); // remove first slash
    QStringList variableList = variableData.split("\\");
    QStringListIterator it(variableList);
    Status status;
    while (it.hasNext()) {
        QString key = it.next();
        if(it.hasNext()) {
            QString value = it.next();
            status.variables.insert(key, value);
        }
    }

    QStringListIterator itP(playerList);
    while (itP.hasNext()) {
        QString line = itP.next();
        QStringList playerData = line.split(" ");
        if(playerData.size() >= 3) {
            QString playerName = QStringList(playerData.mid(2)).join(" ");
            playerName.chop(1);
            playerName.remove(0, 1);
            status.players.append(std::move(Player(playerName,
                                                   playerData[0].toInt(),
                                                   playerData[1].toInt())));
        }
    }

    return status;
}

QString StatusParser::stripResponseHeader(QString data) {
    if (data.startsWith("\xFF\xFF\xFF\xFFstatusResponse\n")) {
        data = data.remove(0, 19);
    }

    return data;
}
