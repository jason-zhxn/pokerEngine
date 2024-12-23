#include "BettingRound.hpp"
#include <iostream>
#include <string>

void executeBettingRound(PokerGame &game)
{
    std::cout << "=== Betting Round ===" << std::endl;

    int activePlayer = (game.dealerIndex + 1) % game.players.size();
    int lastToAct = game.dealerIndex;
    bool bettingComplete = false;

    int highestBet = 0;
    while (!bettingComplete) {
        Player &player = *game.players[activePlayer];

        if (player.isActive()) {
            std::cout << player.getName() << "'s turn. Current bet: " << highestBet
                      << ", your chips: " << player.getChips() << std::endl;

            std::cout << "Enter your action (fold, call, raise): ";
            std::string action;
            std::cin >> action;

            if (action == "fold") {
                player.fold();
                std::cout << player.getName() << " folds." << std::endl;
            } else if (action == "call") {
                int amountToCall = highestBet - player.getCurrentBet();
                if (amountToCall > player.getChips()) { amountToCall = player.getChips(); }
                player.deductChips(amountToCall);
                game.pot += amountToCall;
                player.setCurrentBet(highestBet);
                std::cout << player.getName() << " calls with " << amountToCall << " chips." << std::endl;
            } else if (action == "raise") {
                int raiseAmount;
                std::cout << "Enter raise amount: ";
                std::cin >> raiseAmount;

                int totalBet = highestBet + raiseAmount;
                if (totalBet > player.getChips()) { totalBet = player.getChips(); }
                highestBet = totalBet;
                player.deductChips(totalBet - player.getCurrentBet());
                game.pot += totalBet - player.getCurrentBet();
                player.setCurrentBet(totalBet);
                std::cout << player.getName() << " raises to " << highestBet << " chips." << std::endl;
            } else {
                std::cout << "Invalid action. Please try again." << std::endl;
                continue;
            }
        }

        activePlayer = (activePlayer + 1) % game.players.size();
        if (activePlayer == lastToAct) { bettingComplete = true; }
    }

    for (auto &playerPtr : game.players) { playerPtr->setCurrentBet(0); }
}
