#include "BettingRound.hpp"
#include <iostream>
#include <limits>
#include <string>

void handlePlayerAction(PokerGame &game, Agent *currentPlayer)
{
    double amountToCall = game.currentBet - currentPlayer->getCurrentBet();

    if (amountToCall == 0) {
        std::string action;
        std::cin >> action;

        if (action == "check" || action == "ch") {
        } else if (action == "bet" || action == "b") {
            double raiseAmount = 0;
            std::cin >> raiseAmount;

            while (std::cin.fail() || raiseAmount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> raiseAmount;
            }

            double totalBet = game.currentBet + raiseAmount;
            if (totalBet > currentPlayer->getChips()) { totalBet = currentPlayer->getChips(); }

            double addedAmount = totalBet - currentPlayer->getCurrentBet();
            currentPlayer->deductChips(addedAmount);
            game.pot += addedAmount;
            currentPlayer->setCurrentBet(totalBet);
            game.currentBet = totalBet;

        } else {
            handlePlayerAction(game, currentPlayer);
        }

    } else {
        std::string action;
        std::cin >> action;

        if (action == "fold" || action == "f") {
            currentPlayer->fold();
            return;
        } else if (action == "call" || action == "c") {
            if (amountToCall > currentPlayer->getChips()) { amountToCall = currentPlayer->getChips(); }
            currentPlayer->deductChips(amountToCall);
            game.pot += amountToCall;
            currentPlayer->setCurrentBet(game.currentBet);
        } else if (action == "raise" || action == "r") {
            double raiseAmount = 0;
            std::cin >> raiseAmount;

            while (std::cin.fail() || raiseAmount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> raiseAmount;
            }

            double totalBet = game.currentBet + raiseAmount;
            if (totalBet > currentPlayer->getChips()) { totalBet = currentPlayer->getChips(); }

            double addedAmount = totalBet - currentPlayer->getCurrentBet();
            currentPlayer->deductChips(addedAmount);
            game.pot += addedAmount;
            currentPlayer->setCurrentBet(totalBet);
            game.currentBet = totalBet;

        } else {
            handlePlayerAction(game, currentPlayer);
        }
    }
}

void executeBettingRound(PokerGame &game)
{

    Agent *currentPlayer =
      game.playerIsDealer ? static_cast<Agent *>(game.player.get()) : static_cast<Agent *>(game.bot.get());
    Agent *opponent =
      game.playerIsDealer ? static_cast<Agent *>(game.bot.get()) : static_cast<Agent *>(game.player.get());

    bool bettingComplete = false;
    bool firstRound = true;

    while (!bettingComplete) {
        if (currentPlayer->isActive()) {
            if (currentPlayer == game.player.get()) {

                handlePlayerAction(game, currentPlayer);
            } else {
                game.bot->makeMove(game.pot, game.currentBet);
            }
        }

        std::swap(currentPlayer, opponent);

        if ((!opponent->isActive() || !currentPlayer->isActive())
            || (currentPlayer->getCurrentBet() == game.currentBet && opponent->getCurrentBet() == game.currentBet
                && !firstRound)) {
            bettingComplete = true;
        }
        firstRound = false;
    }

    game.setCurrentBet(0);
    game.player->setCurrentBet(0);
    game.bot->setCurrentBet(0);
}
