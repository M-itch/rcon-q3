#include "player.h"
#include "outputparser.h"

Player::Player(QString name, int score, int ping)
    : fullName(name),
      score(score),
      ping(ping) {
    this->name = OutputParser::removeColors(name);
}

QString Player::getName() const {
    return name;
}

QString Player::getFullName() const {
    return fullName;
}

void Player::setName(const QString& value) {
    fullName = value;
    name = OutputParser::removeColors(value);
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
