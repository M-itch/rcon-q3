#ifndef PLAYERTABLEMODEL_H
#define PLAYERTABLEMODEL_H

#include <QAbstractTableModel>
#include <parser/player.h>

class PlayerTableModel : public QAbstractTableModel
{
public:
    PlayerTableModel(QObject* parent = 0);
    QList<Player> getPlayers() const;
    void setPlayers(QList<Player> players);
    QList<QString> getHeaders() const;
    void setHeaders(QList<QString> headers);
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;
private:
    QList<Player> players;
    QList<QString> headers;
    QVariant getDataFromIndex(const QModelIndex& index) const;
};

#endif // PLAYERTABLEMODEL_H
