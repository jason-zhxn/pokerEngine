#include "Agent.hpp"

Agent::Agent(const std::string &name, double initialChips) : name(name), chips(initialChips), active(true) {}

std::string Agent::getName() const
{
    return name;
}
double Agent::getChips() const
{
    return chips;
}
bool Agent::isActive() const
{
    return active;
}
std::vector<Card> Agent::getHand() const
{
    return hand;
}
double Agent::getCurrentBet() const
{
    return currentBet;
}

void Agent::addChips(double amount)
{
    chips += amount;
}
void Agent::deductChips(double amount)
{
    chips -= amount;
}
void Agent::fold()
{
    active = false;
}
void Agent::reset()
{
    active = true;
    hand.clear();
}
void Agent::setHand(const std::vector<Card> &newHand)
{
    hand = newHand;
}
void Agent::setCurrentBet(const double &amount)
{
    currentBet = amount;
}
