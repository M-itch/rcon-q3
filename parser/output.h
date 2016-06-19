#ifndef OUTPUT_H
#define OUTPUT_H

#include <QColor>
#include <QString>

class Output
{
public:
    Output(QString text = "", QColor color = QColor());
    QString getText() const;
    void setText(const QString &value);
    QColor getColor() const;
    void setColor(const QColor &value);
    QString toHtml();
    QString toPreFormatHtml();
private:
    QString text;
    QColor color;
};

#endif // OUTPUT_H
