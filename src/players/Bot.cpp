#include "Bot.hpp"
#include <iostream>

Bot::Bot(const std::string &name, double initialChips) : Agent(name, initialChips) {}

void Bot::makeMove(double &pot, double highestBet)
{
    double amountToCall = highestBet - this->getCurrentBet();

    if (amountToCall == 0) {
        std::cout << this->getName() << " checks with " << highestBet << " chips\n"; 
        return;
    }
    else if (amountToCall > this->getChips()) { amountToCall = this->getChips(); }
    
    this->deductChips(amountToCall);
    pot += amountToCall;
    this->setCurrentBet(highestBet);
    std::cout << this->getName() << " calls with " << amountToCall << " chips.\n";
}
