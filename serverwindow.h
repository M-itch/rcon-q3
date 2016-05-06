#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include "commandwindow.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget* parent = 0);
    ~ServerWindow();
    void connectToServer(QString serverName);

private slots:
    void on_connectButton_clicked();
    void on_saveButton_clicked();
    void on_deleteButton_clicked();
    void on_defaultCheck_clicked();
    void on_profileBox_currentIndexChanged(const QString& currentText);

private:
    Ui::ServerWindow *ui;
    QMap<QString, Server> serverList;
    QString defaultServer;
    void updateComboBoxItems(QComboBox* combo, QString newItemText);
    void readServers();
    void writeServers();
};

#endif // SERVERWINDOW_H
