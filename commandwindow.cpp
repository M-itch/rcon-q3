#include "commandwindow.h"
#include "htmldelegate.h"
#include "ui_commandwindow.h"
#include <parser/outputparser.h>
#include <parser/statusparser.h>
#include <QLineEdit>
#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QCompleter>
#include <QMessageBox>
#include <utility>

QString CommandWindow::autoCompletionFileName = "commands.txt";

CommandWindow::CommandWindow(const Server& server, QMainWindow* mainWindow, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow),
    mainWindow(mainWindow),
    status(new Query(server.getIp(), server.getPort())),
    rcon(new Rcon(server)),
    playerModel(new PlayerTableModel(this)),
    proxyModel(new QSortFilterProxyModel()),
    lastCommand(QDateTime::currentDateTime().addDays(-1)),
    logFileName(QString("%1/log_%2_%3.log").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation), server.getIp())
                                           .arg(server.getPort())),
    preferences("preferences.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    proxyModel->setSourceModel(playerModel);
    ui->playerTableView->setModel(proxyModel);
    ui->playerTableView->setItemDelegate(new HtmlDelegate());
    ui->playerTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->playerTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->playerTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->playerTableView->sortByColumn(0, Qt::AscendingOrder);
    ui->splitter->setStretchFactor(0, 3);
    statusTimer.setInterval(preferences.value("getstatus_interval", 2000).toInt());
    connect(status, SIGNAL(receive(QString)), this, SLOT(onReceiveStatus(QString)));
    connect(rcon, SIGNAL(receive(QString)), this, SLOT(onReceiveRcon(QString)));
    connect(ui->commandBox->lineEdit(), SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));
    connect(&statusTimer, SIGNAL(timeout()), this, SLOT(requestServerStatus()));
    statusTimer.start();
    loadAutoCompletionCommands();
    QTimer::singleShot(250, this, SLOT(onFirstRun()));
}

CommandWindow::~CommandWindow()
{
    delete ui;
    delete playerModel;
    delete proxyModel;
    delete status;
    delete rcon;
}

void CommandWindow::closeEvent(QCloseEvent* event)
{
    if (mainWindow != nullptr) {
        if (disconnect)
            mainWindow->show();
        else
            mainWindow->close();
    }

    QMainWindow::closeEvent(event);
}

void CommandWindow::on_sendButton_clicked()
{
    sendCommand(ui->commandBox->currentText().toUtf8());
}

void CommandWindow::onReceiveStatus(QString output)
{
    Status statusData = StatusParser::parse(output);
    QMap<QString, QString> v = std::move(statusData.variables);
    playerModel->setPlayers(std::move(statusData.players));
    QString serverStatus = (status->getPing() > 0) ? QString(" ~ %1 ms").arg(status->getPing()) : "";
    setWindowTitle(QString("%1 - %2 %3 [%4/%5]%6")
                   .arg(baseWindowTitle, v.value("gamename"), v.value("shortversion"))
                   .arg(playerModel->getPlayerCount()).arg(v.value("sv_maxclients"), serverStatus));
    QString hostname = OutputParser::removeColors(v.value("sv_hostname"));
    QString statusMessage = QString("%1 (%2) - %3").arg(v.value("mapname"), v.value("g_gametype"), hostname);
    if (statusMessage != ui->statusbar->currentMessage()) {
        ui->statusbar->showMessage(statusMessage);
    }
}

void CommandWindow::onReceiveRcon(QString output)
{
    QTextCursor previousCursor = ui->commandOutput->textCursor();
    ui->commandOutput->moveCursor(QTextCursor::End);

    std::vector<Output> parsedOutput = OutputParser::parse(output, false);
    for (auto i = parsedOutput.cbegin(); i < parsedOutput.cend(); ++i) {
        writeToLog(i->getText());
        ui->commandOutput->insertHtml(i->toPreFormatHtml());
    }

    ui->commandOutput->setTextCursor(previousCursor);
}

void CommandWindow::requestServerStatus()
{
    status->send("getstatus");
}

void CommandWindow::onFirstRun()
{
    requestServerStatus();
    on_actionStatus_triggered();
}

void CommandWindow::on_actionStatus_triggered()
{
    sendCommand("status");
}

void CommandWindow::on_actionDisconnect_triggered()
{
    disconnect = true;
    close();
}

void CommandWindow::on_actionServer_info_triggered()
{
    sendCommand("serverinfo");
}

void CommandWindow::on_actionExit_triggered()
{
    close();
}

void CommandWindow::on_actionPreferences_triggered()
{
    openFileAsDefault(preferences.fileName());
}

void CommandWindow::on_actionView_log_triggered()
{
    openFileAsDefault(logFileName);
}

void CommandWindow::on_actionAuto_completion_commands_triggered()
{
    openFileAsDefault(autoCompletionFileName);
}

void CommandWindow::on_actionPlayer_list_triggered()
{
    ui->playerTableView->setVisible(!ui->playerTableView->isVisible());
}

void CommandWindow::sendCommand(QByteArray command)
{
    QDateTime now = QDateTime::currentDateTime();
    if (lastCommand.msecsTo(now) < 1000) {
        return; // Prevent from spamming the server
    }

    lastCommand = now;
    ui->commandOutput->moveCursor(QTextCursor::End);
    QString output = QDateTime::currentDateTime().toString()
                                                 .append(" > ")
                                                 .append(command);
    writeToLog(output + "\n\n");
    ui->commandOutput->insertHtml(output.append("<br /><br />"));
    rcon->send(command);
}

void CommandWindow::writeToLog(QString line)
{
    if (preferences.value("logging_enabled", 1) == "0") {
        return;
    }

    QFile log(logFileName);
    if (log.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream out(&log);
        out << line;
    }
}

void CommandWindow::loadAutoCompletionCommands()
{
    QFile file(autoCompletionFileName);
    file.open(QFile::ReadOnly);
    QTextStream in(&file);
    QList<QString> autoCompletionCommands;
    while (!in.atEnd()) {
        QString command = in.readLine();
        if (!command.isEmpty() && !autoCompletionCommands.contains(command)) {
            autoCompletionCommands.append(command);
        }
    }

    QCompleter* completer = new QCompleter(autoCompletionCommands, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->commandBox->setCompleter(completer);
}

void CommandWindow::openFileAsDefault(QString fileName)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        QDesktopServices::openUrl(QUrl(QFileInfo(fileName).absolutePath()));
    } else {
        if (QFile(fileName).exists()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        } else {
            QMessageBox::critical(this, QApplication::applicationName(), tr("'%1' does not exist.").arg(fileName));
        }
    }
}

void CommandWindow::on_actionAbout_triggered()
{
    QString text = tr("Version %1 - <a href='https://github.com/M-itch/qtercon'>Source</a><br />"
                      "This program uses Qt version %2.")
                   .arg(QApplication::applicationVersion(), QT_VERSION_STR);
    QMessageBox::about(this, QApplication::applicationName(), text);
}
