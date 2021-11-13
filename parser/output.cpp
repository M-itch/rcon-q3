#include "output.h"

Output::Output(QString text, QColor color)
    : text(text),
      color(color) {
}

QString Output::getText() const {
    return text;
}

void Output::setText(QString value) {
    text.swap(value);
}

QColor Output::getColor() const {
    return color;
}

void Output::setColor(QColor value) {
    color = value;
}

QString Output::toHtml() const {
    return QString("<font color='%1'>%2</font>").arg(color.name())
                                                .arg(text)
           .replace("\n", "<br />");
}

QString Output::toPreFormatHtml() const {
    return QString("<font color='%1'><pre>%2</pre></font>").arg(color.name())
                                                           .arg(text)
           .replace("\n", "<br />");
}
