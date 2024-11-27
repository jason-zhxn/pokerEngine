#include "PokerGame.h"
#include "Deck.h"
#include "Player.h"
#include "HandEvaluator.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// Constructor
PokerGame::PokerGame() : pot(0), currentBet(0)
{
    // NOTE: this defaults to two players
    deck = std::make_unique<Deck>();
    players.push_back(std::make_unique<Player>("Hero", 100));
    players.push_back(std::make_unique<Player>("Villain", 100));
}

// Game Loop
void PokerGame::playGame() 
{
    while (true)
    {
        resetGameState();
        dealHoleCards();
        preflop();
        executeBettingRound();
        flop();
        executeBettingRound();
        turn();
        executeBettingRound();
        river();
        executeBettingRound();
        payout(determineWinner());
    }

}

// Game Stages
void PokerGame::preflop() 
{

}

void PokerGame::flop() 
{

}

void PokerGame::turn() 
{

}

void PokerGame::river() 
{

}

// Game Utilities
void PokerGame::dealHoleCards()
{
    for (auto& playerPtr : players)
    {
        Player& player = *playerPtr;
        std::vector<Card> hole_cards;
        hole_cards.push_back(deck->popTop());
        hole_cards.push_back(deck->popTop());
        player.setHand(hole_cards);
    }
}

void PokerGame::dealCommunityCards(int numCards) 
{
    for (int i = 0; i < numCards; i++)
    {
        communityCards.push_back(deck->popTop());
    }
}

Player& PokerGame::determineWinner()
{

}

void PokerGame::payout(Player &player)
{

}

void PokerGame::resetGameState()
{

}

// Display
void PokerGame::displayAsciiArt(const std::string &message) const
{

}

void PokerGame::clearConsole() const
{

}