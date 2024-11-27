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

        if (players[0]->getChips() == 0 || players[1]->getChips() == 0)
        {
            std::cout << "Game over!";
            if (players[0]->getChips() > 0)
                std::cout << players[0]->getName() << " wins the game!" << std::endl;
            else
                std::cout << players[1]->getName() << " wins the game!" << std::endl;
            break;
        }
    }
}

// Game Stages
void PokerGame::preflop() 
{
    std::cout << "=== Preflop ===" << std::endl;
}

void PokerGame::flop() 
{
    std::cout << "=== Flop ===" << std::endl;
    dealCommunityCards(3);
}

void PokerGame::turn() 
{
    std::cout << "=== Turn ===" << std::endl;
    dealCommunityCards(1);
}

void PokerGame::river() 
{
    std::cout << "=== River ===" << std::endl;
    dealCommunityCards(1);
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

        // Display hero hand
        if (player.getName() == "Hero") {
            std::cout << "Your hole cards: ";
            std::vector<Card> hero_hand = player.getHand();
            std::cout << hero_hand[0].toString() << hero_hand[1].toString() << std::endl;
        }
    }


}

void PokerGame::dealCommunityCards(int numCards) 
{
    for (int i = 0; i < numCards; i++)
    {
        Card card = deck->popTop();
        communityCards.push_back(card);
        std::cout << "Community card dealt: " << card.toString() << std::endl;
    }
}

Player& PokerGame::determineWinner()
{

}

void PokerGame::payout(Player &player)
{
    player.addChips(pot);
}

void PokerGame::resetGameState()
{
    pot = 0;
}

// Display
void PokerGame::displayAsciiArt(const std::string &message) const
{

}

void PokerGame::clearConsole() const
{

}