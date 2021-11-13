#ifndef PLAYERTABLEMODEL_H
#define PLAYERTABLEMODEL_H

#include <QAbstractTableModel>
#include <parser/player.h>
#include <vector>

class PlayerTableModel : public QAbstractTableModel
{
public:
    PlayerTableModel(QObject* parent = nullptr);
    int getPlayerCount() const;
    std::vector<Player> getPlayers() const;
    void setPlayers(std::vector<Player> players);
    std::vector<QString> getHeaders() const;
    void setHeaders(std::vector<QString> headers);
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;
private:
    std::vector<Player> players;
    std::vector<QString> headers;
    QVariant getDataFromIndex(const QModelIndex& index) const;
};

#endif // PLAYERTABLEMODEL_H
