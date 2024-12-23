#pragma once

#include "Deck.hpp"
#include "HandEvaluator.hpp"
#include "Player.hpp"
#include <memory>
#include <string>
#include <vector>

class PokerGame
{
  public:
    PokerGame();
    void playGame();

  private:
    void handlePhase(const std::string &phaseName, int numCommunityCards = 0);
    void preflop();
    void flop();
    void turn();
    void river();
    friend void executeBettingRound(PokerGame &game);

    void handlePlayerDecision(Player &player, int currentBet, int &pot, int &playerBet);

    void dealHoleCards();
    void dealCommunityCards(int numCards);
    void payout();
    void resetGameState();

    void clearConsole() const;

    std::unique_ptr<Deck> deck;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<Card> communityCards;
    int pot;
    int currentBet;
    int dealerIndex;
};
