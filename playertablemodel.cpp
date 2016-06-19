#include "playertablemodel.h"
#include <parser/outputparser.h>
#include <utility> // std::move

PlayerTableModel::PlayerTableModel(QObject* parent)
    : QAbstractTableModel(parent),
      headers({"Name", "Score", "Ping"}) {
}

QList<Player> PlayerTableModel::getPlayers() const {
    return players;
}

void PlayerTableModel::setPlayers(QList<Player> players) {
    emit layoutAboutToBeChanged();
    this->players = std::move(players);
    emit layoutChanged();
}

QList<QString> PlayerTableModel::getHeaders() const {
    return headers;
}

void PlayerTableModel::setHeaders(QList<QString> headers) {
    this->headers = std::move(headers);
}

int PlayerTableModel::rowCount(const QModelIndex&) const {
    return players.size();
}

int PlayerTableModel::columnCount(const QModelIndex&) const {
    return headers.size();
}

QVariant PlayerTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    else if (role == Qt::DisplayRole) {
        return getDataFromIndex(index);
    } else {
        return QVariant();
    }
}

QVariant PlayerTableModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal
            && section < headers.size()) {
        return headers[section];
    } else {
        return QVariant();
    }
}

QVariant PlayerTableModel::getDataFromIndex(const QModelIndex& index) const {
    QVariant data;
    if (index.row() < players.size()) {
        if (index.column() == 0) {
            data = players[index.row()].getNameHtml();
        } else if (index.column() == 1) {
            data = players[index.row()].getScore();
        } else if (index.column() == 2) {
            data = players[index.row()].getPing();
        }
    }

    return data;
}
