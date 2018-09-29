#ifndef STATUSPARSER_H
#define STATUSPARSER_H

#include "outputparser.h"
#include "status.h"
#include <QMap>

class StatusParser: public OutputParser
{
public:
    static Status parse(QString data);
    static QString stripResponseHeader(QString data);
private:
    const static QString printStatusHeader;
};

#endif // STATUSPARSER_H
