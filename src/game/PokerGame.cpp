#include "PokerGame.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "Player.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

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
    while (true) {
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
    for (auto &playerPtr : players) {
        Player &player = *playerPtr;
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
    for (int i = 0; i < numCards; i++) {
        Card card = deck->popTop();
        communityCards.push_back(card);
        std::cout << "Community card dealt: " << card.toString() << std::endl;
    }
}

void PokerGame::executeBettingRound()
{
    std::cout << "=== Betting Round ===" << std::endl;

    int activePlayer = (dealerIndex + 1) % players.size();// Non-dealer acts first
    int lastToAct = dealerIndex;// Dealer acts last
    bool bettingComplete = false;

    int highestBet = 0;
    while (!bettingComplete) {
        Player &player = *players[activePlayer];

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
                if (amountToCall > player.getChips()) {
                    amountToCall = player.getChips();// Go all-in if not enough chips
                }
                player.deductChips(amountToCall);
                pot += amountToCall;
                player.setCurrentBet(highestBet);
                std::cout << player.getName() << " calls with " << amountToCall << " chips." << std::endl;
            } else if (action == "raise") {
                int raiseAmount;
                std::cout << "Enter raise amount: ";
                std::cin >> raiseAmount;

                int totalBet = highestBet + raiseAmount;
                if (totalBet > player.getChips()) {
                    totalBet = player.getChips();// Go all-in if not enough chips
                }
                highestBet = totalBet;
                player.deductChips(totalBet - player.getCurrentBet());
                pot += totalBet - player.getCurrentBet();
                player.setCurrentBet(totalBet);
                std::cout << player.getName() << " raises to " << highestBet << " chips." << std::endl;
            } else {
                std::cout << "Invalid action. Please try again." << std::endl;
                continue;
            }
        }

        activePlayer = (activePlayer + 1) % players.size();
        if (activePlayer == lastToAct) {
            bettingComplete = true;// End the betting round when all players have acted
        }
    }

    // Reset current bets for the next round
    for (auto &playerPtr : players) { playerPtr->setCurrentBet(0); }
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
        // hero wins
        std::cout << players[0]->getName() << " wins the pot of " << pot << " chips!" << std::endl;
        players[0]->addChips(pot);
    } else if (heroResult.rank < villainResult.rank) {
        // villain wins
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

// Display Utils
void PokerGame::clearConsole() const
{
    system("clear");
}