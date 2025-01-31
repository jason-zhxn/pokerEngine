#include "Bot.hpp"
#include <iostream>

Bot::Bot(const std::string &name, double initialChips) : Agent(name, initialChips) {}

void Bot::makeMove(double &pot, double highestBet)
{
    double amountToCall = highestBet - this->getCurrentBet();

    if (amountToCall == 0) { return; }

    if (amountToCall > this->getChips()) {
        amountToCall = this->getChips();
        if (amountToCall == 0) { return; }
    } else {
        this->deductChips(amountToCall);
        pot += amountToCall;
        this->setCurrentBet(highestBet);
    }
}
