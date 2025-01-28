#include "BettingRound.hpp"
#include <iostream>
#include <string>

void executeBettingRound(PokerGame &game)
{
    std::cout << "=== Betting Round ===" << std::endl;

    Agent *currentPlayer =
      game.playerIsDealer ? static_cast<Agent *>(game.player.get()) : static_cast<Agent *>(game.bot.get());
    Agent *opponent =
      game.playerIsDealer ? static_cast<Agent *>(game.bot.get()) : static_cast<Agent *>(game.player.get());

    bool bettingComplete = false;
    int highestBet = game.currentBet;

    while (!bettingComplete) {
        std::cout << "Eureka! 6" << std::endl;
        if (currentPlayer->isActive()) {
            std::cout << "Eureka! 5" << std::endl;
            if (currentPlayer == game.player.get()) {
                std::cout << "Eureka! 4" << std::endl;
                std::cout << currentPlayer->getName() << "'s turn. Current bet: " << highestBet
                          << ", your chips: " << currentPlayer->getChips() << std::endl;

                std::cout << "Enter your action (fold, call, raise): ";
                std::string action;
                std::cin >> action;

                if (action == "fold") {
                    currentPlayer->fold();
                    std::cout << currentPlayer->getName() << " folds." << std::endl;
                    break;
                } else if (action == "call") {
                    int amountToCall = highestBet - currentPlayer->getCurrentBet();
                    if (amountToCall > currentPlayer->getChips()) { amountToCall = currentPlayer->getChips(); }
                    currentPlayer->deductChips(amountToCall);
                    game.pot += amountToCall;
                    currentPlayer->setCurrentBet(highestBet);
                    std::cout << currentPlayer->getName() << " calls with " << amountToCall << " chips." << std::endl;
                } else if (action == "raise") {
                    int raiseAmount;
                    std::cout << "Enter raise amount: ";
                    std::cin >> raiseAmount;

                    int totalBet = highestBet + raiseAmount;
                    if (totalBet > currentPlayer->getChips()) { totalBet = currentPlayer->getChips(); }
                    highestBet = totalBet;
                    currentPlayer->deductChips(totalBet - currentPlayer->getCurrentBet());
                    game.pot += totalBet - currentPlayer->getCurrentBet();
                    currentPlayer->setCurrentBet(totalBet);
                    std::cout << currentPlayer->getName() << " raises to " << highestBet << " chips." << std::endl;
                } else {
                    std::cout << "Invalid action. Please try again." << std::endl;
                    continue;
                }
            } else {
                std::cout << "Eureka! 3" << std::endl;
                game.bot->makeMove(game.pot, highestBet);
                //bettingComplete = true;
            }
        }
        std::cout << "Eureka! 2" << std::endl;
        std::swap(currentPlayer, opponent);

        if (currentPlayer->getCurrentBet() == highestBet && opponent->getCurrentBet() == highestBet) {
            //if (currentPlayer->getCurrentBet() == 0.5 || opponent->getCurrentBet() == 0.5){
                std::cout << "Eureka! 1" << std::endl;
            //}
            bettingComplete = true;
        }
    }

    game.player->setCurrentBet(0);
    game.bot->setCurrentBet(0);
}
