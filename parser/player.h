#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player(QString name = "", int score = 0, int ping = 0);
    QString getName() const;
    QString getFullName() const;
    QString getNameHtml() const;
    void setName(QString value);
    int getScore() const;
    void setScore(int value);
    int getPing() const;
    void setPing(int value);
private:
    QString fullName;
    QString name;
    QString nameHtml;
    int score;
    int ping;
};

#endif // PLAYER_H
