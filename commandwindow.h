#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include <QDateTime>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <network/rcon.h>
#include <network/server.h>
#include "playertablemodel.h"

namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(const Server& server, QMainWindow* mainWindow, QWidget* parent = nullptr);
    ~CommandWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_sendButton_clicked();
    void onReceiveStatus(QString output);
    void onReceiveRcon(QString output);
    void requestServerStatus();
    void onFirstRun();
    void on_actionStatus_triggered();
    void on_actionDisconnect_triggered();
    void on_actionServer_info_triggered();
    void on_actionExit_triggered();
    void on_actionPreferences_triggered();
    void on_actionView_log_triggered();
    void on_actionAuto_completion_commands_triggered();
    void on_actionPlayer_list_triggered();
    void on_actionAbout_triggered();

private:
    Ui::CommandWindow *ui;
    QMainWindow* mainWindow;
    QTimer statusTimer;
    Query* status;
    Rcon* rcon;
    PlayerTableModel* playerModel;
    QSortFilterProxyModel* proxyModel;
    QDateTime lastCommand;
    QString baseWindowTitle;
    QString logFileName;
    QSettings preferences;
    bool disconnect;
    static QString logFileNameFormat;
    static QString autoCompletionFileName;
    static QString preferencesFileName;
    void sendCommand(QByteArray command);
    void writeToLog(QString line);
    void loadAutoCompletionCommands();
    void openFileAsDefault(QString fileName);
};

#endif // COMMANDWINDOW_H
