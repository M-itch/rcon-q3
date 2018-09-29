#include "statusparser.h"
#include <QStringListIterator>
#include <utility> // std::move

const QString StatusParser::printStatusHeader = "\xFF\xFF\xFF\xFFstatusResponse\n";

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
            playerName.chop(1); // remove first "
            playerName.remove(0, 1); // remove last "
            status.players.append(std::move(Player(playerName,
                                                   playerData[0].toInt(),
                                                   playerData[1].toInt())));
        }
    }

    return status;
}

QString StatusParser::stripResponseHeader(QString data) {
    return data.remove(printStatusHeader, Qt::CaseSensitive);
}
