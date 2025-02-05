#include "PokerGame.hpp"
#include "BettingRound.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

PokerGame::PokerGame() : pot(0), currentBet(0), playerIsDealer(true)
{
    deck = std::make_unique<Deck>();
    player = std::make_unique<Player>("Hero", 100.0);
    bot = std::make_unique<Bot>("Villain", 100.0);
}

void PokerGame::playGame()
{
    while (true) {
        resetGameState();

        std::string play_again;
        std::cin >> play_again;

        if (play_again == "n" || play_again == "no" || play_again == "exit" || play_again == "q") {
            double pnl = player->getChips() - 100.0;
            if (pnl < 0) {
            } else if (pnl == 0) {
            } else {
            }
            return;
        }

        collectBlinds();
        dealHoleCards();

        preflop();
        flop();
        turn();
        river();

        if (player->isActive() && bot->isActive()) {
            payout();
        } else if (!player->isActive()) {
            bot->addChips(pot);
        } else if (!bot->isActive()) {
            player->addChips(pot);
        } else {
            throw std::runtime_error("Invalid folding logic. Time to debug.");
        }

        if (player->getChips() == 0 || bot->getChips() == 0) {
            if (bot->getChips() > 0)
                continue;
            else
                break;
        }

        shiftDealerButton();
    }
}

void PokerGame::resetGameState()
{
    pot = 0;
    currentBet = 0;
    communityCards.clear();
    deck->shuffle();
    player->reset();
    bot->reset();
}

void PokerGame::collectBlinds()
{
    double smallBlind = 0.5;
    double bigBlind = 1;

    if (playerIsDealer) {
        player->deductChips(smallBlind);
        player->setCurrentBet(smallBlind);
        bot->deductChips(bigBlind);
        bot->setCurrentBet(bigBlind);
        pot += smallBlind + bigBlind;
    } else {
        bot->deductChips(smallBlind);
        bot->setCurrentBet(smallBlind);
        player->deductChips(bigBlind);
        player->setCurrentBet(bigBlind);
        pot += smallBlind + bigBlind;
    }

    currentBet = bigBlind;
}

void PokerGame::shiftDealerButton()
{
    playerIsDealer = !playerIsDealer;
}

void PokerGame::dealHoleCards()
{
    player->setHand(std::move(std::vector<Card>{ deck->popTop(), deck->popTop() }));
    bot->setHand(std::move(std::vector<Card>{ deck->popTop(), deck->popTop() }));
}

void PokerGame::dealCommunityCards(int numCards)
{
    for (int i = 0; i < numCards; ++i) { communityCards.push_back(std::move(deck->popTop())); }
}

double PokerGame::getCurrentBet()
{
    return currentBet;
}

void PokerGame::setCurrentBet(double newBet)
{
    currentBet = newBet;
}

double PokerGame::getPot()
{
    return pot;
}

void PokerGame::addToPot(double value)
{
    pot += value;
}

void PokerGame::handlePhase(const std::string &phaseName, int numCommunityCards)
{
    if (numCommunityCards > 0) dealCommunityCards(numCommunityCards);
    executeBettingRound(*this);
}

void PokerGame::preflop()
{
    handlePhase("Preflop", 0);
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

void PokerGame::payout()
{
    HandEvaluator evaluator;

    auto heroResult = evaluator.evaluateHand(player->getHand(), communityCards);
    auto villainResult = evaluator.evaluateHand(bot->getHand(), communityCards);

    if (heroResult == villainResult) {
        player->addChips(pot / 2);
        bot->addChips(pot / 2);
    } else if (heroResult > villainResult) {
        player->addChips(pot);
    } else {
        bot->addChips(pot);
    }

    pot = 0;
}
