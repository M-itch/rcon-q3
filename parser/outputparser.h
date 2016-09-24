#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include <QList>
#include "output.h"

class OutputParser
{
public:
    static QList<Output> parse(QString data);
    static QString parseToHtml(QString data);
    static QString removePrintHeader(QString data);
    static QColor numberToColor(int number);
    static QString removeColors(QString data);
private:
    static QList<Output> splitColors(QString data);
    const static QString printHeader;
};

#endif // OUTPUTPARSER_H
