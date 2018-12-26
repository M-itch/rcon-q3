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

QString CommandWindow::logFileNameFormat = "%1/log_%2_%3.log";
QString CommandWindow::autoCompletionFileName = "commands.txt";
QString CommandWindow::preferencesFileName = "preferences.ini";

CommandWindow::CommandWindow(const Server server, QMainWindow* mainWindow, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow),
    mainWindow(mainWindow),
    status(new Query(server.getIp(), server.getPort())),
    rcon(new Rcon(server)),
    playerModel(new PlayerTableModel(this)),
    proxyModel(new QSortFilterProxyModel()),
    preferences(preferencesFileName, QSettings::IniFormat),
    disconnect(false)
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
    lastCommand = QDateTime::currentDateTime().addDays(-1);
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    logFileName = QString(logFileNameFormat).arg(dataPath)
                                            .arg(server.getIp())
                                            .arg(server.getPort());
    baseWindowTitle = windowTitle();
    statusTimer.setInterval(preferences.value("getstatus_interval", 2000).toInt());
    connect(status, SIGNAL(receive(QByteArray)), this, SLOT(onReceiveStatus(QByteArray)));
    connect(rcon, SIGNAL(receive(QByteArray)), this, SLOT(onReceiveRcon(QByteArray)));
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

void CommandWindow::onReceiveStatus(QByteArray output)
{
    Status status = StatusParser::parse(output);
    QMap<QString, QString> v = std::move(status.variables);
    QList<Player> p = std::move(status.players);
    playerModel->setPlayers(p);
    int ping = this->status->getPing();
    QString serverStatus = (ping > 0) ? QString(" ~ %1 ms").arg(ping) : "";
    setWindowTitle(QString("%1 - %2 %3 [%4/%5]%6").arg(baseWindowTitle)
                                                  .arg(v.value("gamename"))
                                                  .arg(v.value("shortversion"))
                                                  .arg(p.size())
                                                  .arg(v.value("sv_maxclients"))
                                                  .arg(serverStatus));

    QString hostname = OutputParser::removeColors(v.value("sv_hostname"));
    QString statusMessage = QString("%1 (%2) - %3").arg(v.value("mapname"))
                            .arg(v.value("g_gametype"))
                            .arg(hostname);
    if (statusMessage != ui->statusbar->currentMessage()) {
        ui->statusbar->showMessage(statusMessage);
    }
}

void CommandWindow::onReceiveRcon(QByteArray output)
{
    QList<Output> parsedOutput = OutputParser::parse(output, false);
    QListIterator<Output> i(parsedOutput);
    QTextCursor prevCursor = ui->commandOutput->textCursor();
    ui->commandOutput->moveCursor(QTextCursor::End);
    while (i.hasNext()) {
        Output line = i.next();
        writeToLog(line.getText());
        ui->commandOutput->insertHtml(line.toPreFormatHtml());
    }

    ui->commandOutput->setTextCursor(prevCursor);
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
    openFileAsDefault(preferencesFileName);
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
            QMessageBox::critical(this,
                                  QApplication::applicationName(),
                                  QString("'%1' does not exist.").arg(fileName));
        }
    }
}

void CommandWindow::on_actionAbout_triggered()
{
    QString text = QString("%1 - <a href='https://github.com/M-itch/qtercon'>%2</a><br />This program uses Qt version %3.")
                   .arg(QApplication::applicationVersion())
                   .arg(GIT_VERSION)
                   .arg(QT_VERSION_STR);
    QMessageBox::about(this,
                       QApplication::applicationName(),
                       text);
}
