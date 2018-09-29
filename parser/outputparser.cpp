#include "outputparser.h"
#include <QRegularExpression>
#include <utility> // std::move

const QString OutputParser::printHeader = "\xFF\xFF\xFF\xFFprint\n";

QList<Output> OutputParser::parse(QString data, bool noDoubleColors) {
    data = removePrintHeader(data);
    if (noDoubleColors) {
        data = removeDoubleColors(data);
    }

    return splitColors(data);
}

QString OutputParser::parseToHtml(QString data) {
    QList<Output> list = parse(data);
    QString output = "";
    QList<Output>::iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        output.append(i->toHtml());
    }

    return output;
}

QList<Output> OutputParser::splitColors(QString data) {
    int startIndex = 0;
    QColor color = Qt::black;
    QList<Output> output;
    for (int i = 0 ; i < data.length() ; i++) {
        if (data[i] == '^' && data.length() > i+1
                && data[i+1].isDigit()) {
            output.append(std::move(Output(data.mid(startIndex, i - startIndex),
                                           color)));
            color = numberToColor(data[i+1].digitValue());
            startIndex = i+2; // skip ^x
        }
    }

    output.append(std::move(Output(data.right(data.length() - startIndex),
                                   color))); // last text

    return output;
}

/// Removes ^x and ^^xx.
QString OutputParser::removeColors(QString data) {
    data.remove(QRegularExpression("\\^\\^\\d\\d"));
    return data.remove(QRegularExpression("\\^\\d"));
}

/// Replaces ^^x with ^.
/// Before: ^^xy
/// After: ^y
QString OutputParser::removeDoubleColors(QString data) {
    return data.replace(QRegularExpression("\\^\\^\\d"), "^");
}

QString OutputParser::removePrintHeader(QString data) {
    return data.remove(printHeader, Qt::CaseSensitive);
}

QColor OutputParser::numberToColor(int number) {
    QColor color = Qt::black; // Also 7 (white)
    switch (number) {
        case 1:
            color = Qt::red;
            break;
        case 2:
            color = Qt::green;
            break;
        case 3:
            color = Qt::darkYellow;
            break;
        case 4:
            color = Qt::blue;
            break;
        case 5:
            color = Qt::cyan;
            break;
        case 6: // pink
            color = QColor(255, 105, 180);
            break;
        case 8:
            color = Qt::darkGreen;
            break;
        case 9:
            color = Qt::gray;
            break;

        default:
            break;
    }

    return color;
}
