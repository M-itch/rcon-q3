#include "outputparser.h"
#include <QRegularExpression>

const QString OutputParser::printHeader = "\xff\xff\xff\xffprint\n";
const std::unordered_map<int, QColor> OutputParser::numberToColorTable = {{1, Qt::red},
                                                                          {2, Qt::green},
                                                                          {3, Qt::darkYellow},
                                                                          {4, Qt::blue},
                                                                          {5, Qt::cyan},
                                                                          {6, QColor(255, 105, 180)}, // pink
                                                                          {8, Qt::darkGreen},
                                                                          {9, Qt::gray}};

std::vector<Output> OutputParser::parse(QString data, bool noDoubleColors) {
    data = removePrintHeader(data);
    if (noDoubleColors) {
        data = removeDoubleColors(data);
    }

    return splitColors(data);
}

QString OutputParser::parseToHtml(QString data) {
    std::vector<Output> list = parse(data);
    QString output = "";
    for (auto i = list.cbegin(); i != list.cend(); ++i) {
        output.append(i->toHtml());
    }

    return output;
}

std::vector<Output> OutputParser::splitColors(const QString& data) {
    int startIndex = 0;
    QColor color = Qt::black;
    std::vector<Output> output;
    for (int i = 0 ; i < data.length() ; i++) {
        if (data[i] == '^' && data.length() > i+1
                && data[i+1].isDigit()) {
            output.emplace_back(Output(data.mid(startIndex, i - startIndex), color));
            color = numberToColor(data[i+1].digitValue());
            startIndex = i+2; // skip ^x
        }
    }

    output.emplace_back(Output(data.right(data.length() - startIndex), color)); // last text

    return output;
}

/// Removes ^x and ^^xx.
QString OutputParser::removeColors(QString data) {
    static QRegularExpression singleExpression("\\^\\d");
    static QRegularExpression doubleExpression("\\^\\^\\d\\d");
    data.remove(doubleExpression);
    return data.remove(singleExpression);
}

/// Replaces ^^x with ^.
/// Before: ^^xy
/// After: ^y
QString OutputParser::removeDoubleColors(QString data) {
    static QRegularExpression expression("\\^\\^\\d");
    return data.replace(expression, "^");
}

QString OutputParser::removePrintHeader(QString data) {
    return data.remove(printHeader, Qt::CaseSensitive);
}

QColor OutputParser::numberToColor(int number) {
    auto color = numberToColorTable.find(number);
    return color != numberToColorTable.end() ? color->second : Qt::black;
}
