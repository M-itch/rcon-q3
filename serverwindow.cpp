#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <network/server.h>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardPaths>
#include <utility>
#ifdef Q_OS_WIN
#include <QWinJumpList>
#include <QWinJumpListCategory>
#include <QWinJumpListItem>
#endif

const QString ServerWindow::serverFileNameFormat = "%1/servers.dat";

ServerWindow::ServerWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

bool ServerWindow::connectToServer(QString serverName)
{
    if (!serverList.contains(serverName)) {
        QMessageBox::critical(this,
                              QApplication::applicationName(),
                              tr("The profile name does not exist in the server list."));
        return false;
    }

    const Server s = serverList.value(serverName);

#ifdef Q_OS_WIN
    QWinJumpList jumpList;
    QWinJumpListCategory* recent = jumpList.recent();

    QWinJumpListItem* jumpItem = new QWinJumpListItem(QWinJumpListItem::Link);
    jumpItem->setTitle(QString("%1 @ %2:%3").arg(serverName)
                                            .arg(s.getIp())
                                            .arg(s.getPort()));
    jumpItem->setFilePath(QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    jumpItem->setIcon(QIcon("://resources/codlogo.ico"));
    jumpItem->setArguments(QStringList({"--connect", serverName}));
    recent->addItem(jumpItem);
#endif

    CommandWindow* commandWindow = new CommandWindow(s, this);
    commandWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    commandWindow->raise();
    commandWindow->show();
    this->hide();
    return true;
}

void ServerWindow::on_connectButton_clicked()
{
    if (ui->profileBox->currentIndex() == -1
            || !serverList.contains(ui->profileBox->currentText())) {
        return;
    }

    connectToServer(ui->profileBox->currentText());
}

void ServerWindow::on_saveButton_clicked()
{
    QHostAddress serverIP;
    if (ui->ipEdit->text().isEmpty() || serverIP.setAddress(ui->ipEdit->text())) {
        QMessageBox::warning(this,
                             QApplication::applicationName(),
                             tr("The IP address is not valid."));
        return;
    } else if (ui->passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this,
                             QApplication::applicationName(),
                             tr("The password field cannot be empty."));
        return;
    }

    Server s(ui->ipEdit->text(),
             ui->portBox->text().toUShort(),
             ui->passwordEdit->text().toUtf8());
    serverList.insert(ui->profileBox->currentText(), std::move(s));
    updateComboBoxItems(ui->profileBox, ui->profileBox->currentText());

    if(ui->defaultCheck->isChecked()) {
        defaultServer = ui->profileBox->currentText();
    }

    writeServers();
}

void ServerWindow::on_deleteButton_clicked()
{
    int currentIndex = ui->profileBox->currentIndex();
    if (currentIndex == -1) {
        return;
    }

    serverList.remove(ui->profileBox->currentText());
    ui->profileBox->removeItem(currentIndex);
    writeServers();
}

void ServerWindow::on_defaultCheck_clicked()
{
    if (ui->defaultCheck->isChecked()) {
        defaultServer = ui->profileBox->currentText();
    } else if (defaultServer == ui->profileBox->currentText()) {
        defaultServer = "";
    }
}

void ServerWindow::on_profileBox_currentIndexChanged(const QString& currentText)
{
    if (currentText == defaultServer) {
        ui->defaultCheck->setChecked(true);
    } else {
        ui->defaultCheck->setChecked(false);
    }

    const Server s = serverList.value(ui->profileBox->currentText());
    ui->ipEdit->setText(s.getIp());
    ui->portBox->setValue(s.getPort());
    ui->passwordEdit->setText(s.getRconPassword());
}

void ServerWindow::updateComboBoxItems(QComboBox* combo, QString newItemText)
{
    bool foundItem = false;
    int insertIndex = 0;
    for (int i = 0; i < combo->count();  i++) {
        const QString currentText = combo->itemText(i);
        if (currentText == newItemText) {
            foundItem = true;
        } else if (newItemText > currentText) {
            insertIndex = i;
        }
    }

    if (!foundItem) {
        combo->insertItem(insertIndex, newItemText);
    }
}

void ServerWindow::readServers(bool oldFormat)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(QString(serverFileNameFormat).arg(path));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open servers file.");
        return;
    }

    QByteArray serverData = file.readAll();
    QJsonObject rootObject = QJsonDocument::fromJson(serverData).object();
    QJsonArray serversArray = rootObject["servers"].toArray();
    foreach (const QJsonValue& value, serversArray) {
        const QJsonObject& serverObj = value.toObject();
        serverList.insert(serverObj["profile"].toString(), Server::fromJson(serverObj, oldFormat));
    }

    defaultServer = rootObject["default"].toString();
    ui->profileBox->addItems(serverList.keys());
    ui->profileBox->setCurrentText(defaultServer);
    on_profileBox_currentIndexChanged(defaultServer);
}

void ServerWindow::writeServers()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }

    QFile file(QString(serverFileNameFormat).arg(path));
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write to servers file.");
        return;
    }

    QMapIterator<QString, Server> i(serverList);
    QJsonArray serversArray;
    while (i.hasNext()) {
        const Server s = i.next().value();
        QJsonObject serverObject = s.toJSON();
        serverObject["profile"] = i.key();
        serversArray.append(serverObject);
    }

    QJsonObject rootObject;
    rootObject["servers"] = serversArray;
    rootObject["default"] = defaultServer;

    QString data = QJsonDocument(rootObject).toJson(QJsonDocument::Compact);
    file.write(data.toUtf8());
}

bool ServerWindow::moveToNewAppFolderLocation(const QString& oldApplicationName)
{
    bool moved = false;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    dir.cdUp();
    if (dir.exists(oldApplicationName)) {
        moved = dir.rename(oldApplicationName, QApplication::applicationName());
    }

    return moved;
}
