#pragma once

#include "Deck.h"
#include "Player.h"
#include <memory>
#include <string>
#include <vector>

class PokerGame
{
  public:
    PokerGame(int initialChips, int numPlayers);

    void playGame();

  private:
    void dealCards();
    void executeBettingRound();
    void handlePlayerDecision(Player &player, int currentBet, int &pot, int &playerBet);
    void determineWinner();
    void resetGameState();

    void displayAsciiArt(const std::string &message) const;
    void clearConsole() const;

    std::unique_ptr<Deck> deck;
    std::vector<std::unique_ptr<Player>> players;
    int pot;
    int currentBet;
};
