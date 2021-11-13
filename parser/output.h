#ifndef OUTPUT_H
#define OUTPUT_H

#include <QColor>
#include <QString>

class Output
{
public:
    Output(QString text = "", QColor color = QColor());
    QString getText() const;
    void setText(QString value);
    QColor getColor() const;
    void setColor(QColor value);
    QString toHtml() const;
    QString toPreFormatHtml() const;
private:
    QString text;
    QColor color;
};

#endif // OUTPUT_H
