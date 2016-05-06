#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include <QList>
#include "output.h"

class OutputParser
{
public:
    static QList<Output> parse(QString data);
    static QString stripPrintHeader(QString data);
    static QColor numberToColor(int number);
    static QString removeColors(QString data);
private:
    static QList<Output> splitColors(QString data);
};

#endif // OUTPUTPARSER_H
