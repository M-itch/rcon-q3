#include "output.h"

Output::Output(QString text, QColor color)
    : text(text),
      color(color) {
}

QString Output::getText() const {
    return text;
}

void Output::setText(const QString &value) {
    text = value;
}

QColor Output::getColor() const {
    return color;
}

void Output::setColor(const QColor &value) {
    color = value;
}

QString Output::toHtml() {
    return QString("<font color='%1'>%2</font>").arg(color.name())
                                                .arg(text)
           .replace("\n", "<br />");
}

QString Output::toPreFormatHtml() {
    return QString("<font color='%1'><pre>%2</pre></font>").arg(color.name())
                                                           .arg(text)
           .replace("\n", "<br />");
}
