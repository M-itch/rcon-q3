#include "playertablemodel.h"
#include <parser/outputparser.h>
#include <utility>

PlayerTableModel::PlayerTableModel(QObject* parent)
    : QAbstractTableModel(parent),
      headers({tr("Name"), tr("Score"), tr("Ping")}) {
}

int PlayerTableModel::getPlayerCount() const {
    return static_cast<int>(players.size());
}

std::vector<Player> PlayerTableModel::getPlayers() const {
    return players;
}

void PlayerTableModel::setPlayers(std::vector<Player> players) {
    emit layoutAboutToBeChanged();
    this->players = std::move(players);
    emit layoutChanged();
}

std::vector<QString> PlayerTableModel::getHeaders() const {
    return headers;
}

void PlayerTableModel::setHeaders(std::vector<QString> headers) {
    this->headers = std::move(headers);
}

int PlayerTableModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(players.size());
}

int PlayerTableModel::columnCount(const QModelIndex&) const {
    return static_cast<int>(headers.size());
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
            && section < static_cast<int>(headers.size())) {
        return headers[static_cast<unsigned int>(section)];
    } else {
        return QVariant();
    }
}

QVariant PlayerTableModel::getDataFromIndex(const QModelIndex& index) const {
    QVariant data;
    if (index.row() < static_cast<int>(players.size())) {
        auto row = static_cast<unsigned int>(index.row());
        if (index.column() == 0) {
            data = players[row].getNameHtml();
        } else if (index.column() == 1) {
            data = players[row].getScore();
        } else if (index.column() == 2) {
            data = players[row].getPing();
        }
    }

    return data;
}
