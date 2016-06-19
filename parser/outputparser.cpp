#include "outputparser.h"
#include <QRegularExpression>
#include <utility> // std::move

QList<Output> OutputParser::parse(QString data) {
    data = stripPrintHeader(data);
    return splitColors(data);
}

QString OutputParser::parseToHtml(QString data) {
    QList<Output> list = parse(data);
    QString html = "";
    QList<Output>::iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        html.append(i->toHtml());
    }

    return html;
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

QString OutputParser::removeColors(QString data) {
    return data.remove(QRegularExpression("\\^\\d"));
}

QString OutputParser::stripPrintHeader(QString data) {
    if (data.startsWith("\xFF\xFF\xFF\xFFprint\n")) {
        data = data.remove(0, 10);
    }

    return data;
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
