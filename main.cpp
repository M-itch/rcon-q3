#include "serverwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWindow w;
    w.show();

    QStringList arguments = a.arguments();
    if (arguments.size() >= 2 && arguments.first() == "--connect") {
        w.connectToServer(arguments.at(1));
    }

    return a.exec();
}
