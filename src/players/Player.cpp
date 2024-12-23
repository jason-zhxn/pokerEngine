#include "Player.hpp"

Player::Player(const std::string &name, int initialChips, bool bot)
  : name(name), chips(initialChips), active(true), bot(bot)
{}

std::string Player::getName() const
{
    return name;
}

int Player::getChips() const
{
    return chips;
}

bool Player::isActive() const
{
    return active;
}

bool Player::isBot() const
{
    return bot;
}

std::vector<Card> Player::getHand() const
{
    return hand;
}

int Player::getCurrentBet() const
{
    return currentBet;
}

void Player::addChips(int amount)
{
    chips += amount;
}

void Player::deductChips(int amount)
{
    chips -= amount;
}

void Player::fold()
{
    active = false;
}

void Player::reset()
{
    active = true;
    hand.clear();
}

void Player::setHand(const std::vector<Card> &newHand)
{
    hand = newHand;
}

void Player::setCurrentBet(const int &amount)
{
    currentBet = amount;
}
