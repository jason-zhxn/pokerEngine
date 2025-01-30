#include "PokerGame.hpp"
#include "BettingRound.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

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
        collectBlinds();
        dealHoleCards();

        preflop();
        flop();
        turn();
        river();
        payout();

        if (player->getChips() == 0 || bot->getChips() == 0) {
            std::cout << "Game over!\n";
            if (bot->getChips() > 0)
                std::cout << bot->getName() << " wins the game!\n";
            else
                std::cout << player->getName() << " wins the game!\n";
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
    std::cout << "=== Starting a new round ===\n";
}

void PokerGame::collectBlinds()
{
    double smallBlind = 0.5;
    double bigBlind = 1;

    if (playerIsDealer) {
        player->deductChips(smallBlind);
        bot->deductChips(bigBlind);
        pot += smallBlind + bigBlind;
        currentBet = bigBlind;
        std::cout << player->getName() << " posts the small blind of " << smallBlind << " chips.\n";
        std::cout << bot->getName() << " posts the big blind of " << bigBlind << " chips.\n";
    } else {
        bot->deductChips(smallBlind);
        player->deductChips(bigBlind);
        pot += smallBlind + bigBlind;
        currentBet = bigBlind;
        std::cout << bot->getName() << " posts the small blind of " << smallBlind << " chips.\n";
        std::cout << player->getName() << " posts the big blind of " << bigBlind << " chips.\n";
    }
}

void PokerGame::shiftDealerButton()
{
    playerIsDealer = !playerIsDealer;
    std::cout << "Dealer button is now with " << (playerIsDealer ? player->getName() : bot->getName()) << ".\n";
}

void PokerGame::dealHoleCards()
{
    player->setHand({ deck->popTop(), deck->popTop() });
    bot->setHand({ deck->popTop(), deck->popTop() });

    std::cout << "Your hole cards: " << player->getHand()[0].toString() << " " << player->getHand()[1].toString()
              << "\n";
}

void PokerGame::dealCommunityCards(int numCards)
{
    for (int i = 0; i < numCards; ++i) {
        Card card = deck->popTop();
        communityCards.push_back(card);
        std::cout << "Community card dealt: " << card.toString() << "\n";
    }
}

void PokerGame::handlePhase(const std::string &phaseName, int numCommunityCards)
{
    std::cout << "=== " << phaseName << " ===\n";
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
        std::cout << "It's a tie! Pot is split.\n";
    } else if (heroResult > villainResult) {
        player->addChips(pot);
        std::cout << player->getName() << " wins the pot of " << pot << " chips!\n";
    } else {
        bot->addChips(pot);
        std::cout << bot->getName() << " wins the pot of " << pot << " chips!\n";
    }

    std::cout << "Villain's hole cards were: " << bot->getHand()[0].toString() << " " << bot->getHand()[1].toString()
              << "\n";

    std::cout << player->getName() << "'s hand: " << heroResult.toString() << "\n";
    std::cout << bot->getName() << "'s hand: " << villainResult.toString() << "\n";

    pot = 0;
}
