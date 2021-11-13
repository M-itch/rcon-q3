#include "serverwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("rcon-q3");
    a.setApplicationVersion(APP_VERSION);
    QDir::setCurrent(QApplication::applicationDirPath());

    ServerWindow w;
    bool moved = w.moveToNewAppFolderLocation("qtercon");
    w.readServers(moved);
    if (moved) w.writeServers();

    bool connectedToServer = false;
    QStringList arguments = a.arguments();
    if (arguments.size() >= 3 && arguments.at(1) == "--connect") {
        connectedToServer = w.connectToServer(arguments.at(2));
    }

    if (!connectedToServer) {
        w.show();
    }

    return a.exec();
}
