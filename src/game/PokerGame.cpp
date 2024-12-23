#include "PokerGame.hpp"
#include "BettingRound.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

PokerGame::PokerGame() : pot(0), currentBet(0)
{
    deck = std::make_unique<Deck>();
    players.push_back(std::make_unique<Player>("Hero", 100, false));
    players.push_back(std::make_unique<Player>("Villain", 100, true));
}

void PokerGame::playGame()
{
    while (true) {
        resetGameState();
        dealHoleCards();
        preflop();
        executeBettingRound(*this);
        flop();
        executeBettingRound(*this);
        turn();
        executeBettingRound(*this);
        river();
        executeBettingRound(*this);
        payout();

        if (players[0]->getChips() == 0 || players[1]->getChips() == 0) {
            std::cout << "Game over!";
            if (players[0]->getChips() > 0)
                std::cout << players[0]->getName() << " wins the game!" << std::endl;
            else
                std::cout << players[1]->getName() << " wins the game!" << std::endl;
            break;
        }
    }
}

void PokerGame::handlePhase(const std::string &phaseName, int numCommunityCards)
{
    std::cout << "=== " << phaseName << " ===" << std::endl;
    if (numCommunityCards > 0) { dealCommunityCards(numCommunityCards); }
}

void PokerGame::preflop()
{
    handlePhase("Preflop");
}

void PokerGame::flop()
{
    handlePhase("Flop", 3);
}

void PokerGame::turn()
{
    handlePhase("Turn", 1);
}

void PokerGame::river()
{
    handlePhase("River", 1);
}

void PokerGame::dealHoleCards()
{
    for (auto &playerPtr : players) {
        Player &player = *playerPtr;
        std::vector<Card> hole_cards;
        hole_cards.push_back(deck->popTop());
        hole_cards.push_back(deck->popTop());
        player.setHand(hole_cards);

        if (player.getName() == "Hero") {
            std::cout << "Your hole cards: ";
            std::vector<Card> hero_hand = player.getHand();
            std::cout << hero_hand[0].toString() << " " << hero_hand[1].toString() << std::endl;
        }
    }
}

void PokerGame::dealCommunityCards(int numCards)
{
    for (int i = 0; i < numCards; i++) {
        Card card = deck->popTop();
        communityCards.push_back(card);
        std::cout << "Community card dealt: " << card.toString() << std::endl;
    }
}

void PokerGame::payout()
{
    HandEvaluator evaluator;

    HandEvaluator::HandResult heroResult = evaluator.evaluateHand(players[0]->getHand(), communityCards);
    HandEvaluator::HandResult villainResult = evaluator.evaluateHand(players[1]->getHand(), communityCards);

    std::cout << players[0]->getName() << "'s hand: " << heroResult.rank << " with high cards: ";
    for (const auto &card : heroResult.highCards) { std::cout << card << " "; }
    std::cout << std::endl;

    std::cout << players[1]->getName() << "'s hand: " << villainResult.rank << " with high cards: ";
    for (const auto &card : villainResult.highCards) { std::cout << card << " "; }
    std::cout << std::endl;

    if (heroResult.rank > villainResult.rank) {
        std::cout << players[0]->getName() << " wins the pot of " << pot << " chips!" << std::endl;
        players[0]->addChips(pot);
    } else if (heroResult.rank < villainResult.rank) {
        std::cout << players[1]->getName() << " wins the pot of " << pot << " chips!" << std::endl;
        players[1]->addChips(pot);
    } else {
        std::cout << "Both players have the same hand rank. Checking high cards..." << std::endl;
        if (heroResult.highCards > villainResult.highCards) {
            std::cout << players[0]->getName() << " wins the pot of " << pot << " chips!" << std::endl;
            players[0]->addChips(pot);
        } else if (heroResult.highCards < villainResult.highCards) {
            std::cout << players[1]->getName() << " wins the pot of " << pot << " chips!" << std::endl;
            players[1]->addChips(pot);
        } else {
            std::cout << "It's a tie! The pot of " << pot << " chips is split." << std::endl;
            players[0]->addChips(pot / 2);
            players[1]->addChips(pot / 2);
        }
    }
    pot = 0;
}

void PokerGame::resetGameState()
{
    dealerIndex = (dealerIndex + 1) % players.size();
    pot = 0;
    currentBet = 0;
    communityCards.clear();
    deck->shuffle();
    for (auto &playerPtr : players) { playerPtr->reset(); }
    std::cout << "=== Starting a new round ===" << std::endl;
    dealerIndex = (dealerIndex + 1) % players.size();
    std::cout << "Dealer button is now with " << players[dealerIndex]->getName() << std::endl;
}

void PokerGame::clearConsole() const
{
    system("clear");
}
