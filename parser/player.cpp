#include "player.h"
#include "outputparser.h"

Player::Player(QString name, int score, int ping)
    : fullName(name),
      name(OutputParser::removeColors(name)),
      nameHtml(OutputParser::parseToHtml(name)),
      score(score),
      ping(ping) {
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

void Player::setName(QString value) {
    fullName.swap(value);
    name = OutputParser::removeColors(fullName);
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
