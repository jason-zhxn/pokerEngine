#pragma once

#include "Deck.h"
#include "Player.h"
#include "HandEvaluator.h"
#include <memory>
#include <string>
#include <vector>

class PokerGame
{
public:
    // Constructor
    PokerGame();

    // Game Loop
    void playGame();

private:
    // Game Stages
    void preflop();
    void flop();
    void turn();
    void river();

    // Betting
    void executeBettingRound();
    void handlePlayerDecision(Player &player, int currentBet, int &pot, int &playerBet);

    // Game Utilities
    void dealHoleCards();
    void dealCommunityCards(int numCards);
    void payout();
    void resetGameState();

    // Display Utils
    void clearConsole() const;

    // Members
    std::unique_ptr<Deck> deck;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<Card> communityCards;
    int pot;
    int currentBet;
    int dealerIndex;
};
