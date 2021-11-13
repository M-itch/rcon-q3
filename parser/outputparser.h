#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include "output.h"
#include <vector>
#include <unordered_map>

class OutputParser
{
public:
    static std::vector<Output> parse(QString data, bool noDoubleColors = true);
    static QString parseToHtml(QString data);
    static QString removePrintHeader(QString data);
    static QColor numberToColor(int number);
    static QString removeColors(QString data);
    static QString removeDoubleColors(QString data);
private:
    const static QString printHeader;
    const static std::unordered_map<int, QColor> numberToColorTable;
    static std::vector<Output> splitColors(const QString& data);
};

#endif // OUTPUTPARSER_H
