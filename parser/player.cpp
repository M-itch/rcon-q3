#include "player.h"
#include "outputparser.h"

Player::Player(QString name, int score, int ping)
    : fullName(name),
      score(score),
      ping(ping) {
    this->name = OutputParser::removeColors(name);
    this->nameHtml = OutputParser::parseToHtml(fullName);
}

QString Player::getName() const {
    return name;
}

QString Player::getFullName() const {
    return fullName;
}

QString Player::getNameHtml() const {
    return nameHtml;
}

void Player::setName(const QString& value) {
    fullName = value;
    name = OutputParser::removeColors(value);
    nameHtml = OutputParser::parseToHtml(fullName);
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int value) {
    score = value;
}

int Player::getPing() const {
    return ping;
}

void Player::setPing(int value) {
    ping = value;
}
