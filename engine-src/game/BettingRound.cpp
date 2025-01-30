#include "BettingRound.hpp"
#include <iostream>
#include <string>

void executeBettingRound(PokerGame &game)
{
    std::cout << "=== Betting Round ===" << std::endl;

    Agent *currentPlayer =
      game.playerIsDealer ? static_cast<Agent *>(game.bot.get()) : static_cast<Agent *>(game.player.get());
    Agent *opponent =
      game.playerIsDealer ? static_cast<Agent *>(game.player.get()) : static_cast<Agent *>(game.bot.get());

    bool bettingComplete = false;
    int highestBet = game.currentBet;

    while (!bettingComplete) {
        if (currentPlayer->isActive()) {
            if (currentPlayer == game.player.get()) {
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
                game.bot->makeMove();
                bettingComplete = true;
            }
        }

        std::swap(currentPlayer, opponent);

        if (currentPlayer->getCurrentBet() == highestBet && opponent->getCurrentBet() == highestBet) {
            bettingComplete = true;
        }
    }

    game.player->setCurrentBet(0);
    game.bot->setCurrentBet(0);
}
