#pragma once

#include "Bot.hpp"
#include "Deck.hpp"
#include "HandEvaluator.hpp"
#include "Player.hpp"
#include <memory>
#include <string>
#include <vector>

class PokerGame
{
  private:
    std::unique_ptr<Deck> deck;
    std::unique_ptr<Player> player;
    std::unique_ptr<Bot> bot;
    std::vector<Card> communityCards;
    double pot;
    double currentBet;
    bool playerIsDealer;

  public:
    PokerGame();
    void playGame();

  private:
    friend void executeBettingRound(PokerGame &game);
    void setCurrentBet(double newBet);
    double getCurrentBet();
    void resetGameState();
    void collectBlinds();
    void shiftDealerButton();
    void dealHoleCards();
    void dealCommunityCards(int numCards);
    void handlePhase(const std::string &phaseName, int numCommunityCards = 0);
    void preflop();
    void flop();
    void turn();
    void river();
    void payout();
};
