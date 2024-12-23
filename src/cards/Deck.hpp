#pragma once

#include "Card.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

class Deck
{
  private:
    std::vector<Card> cards;
    size_t activeSize;

  public:
    Deck()
    {
        const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
        const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
        for (const auto &suit : suits) {
            for (const auto &rank : ranks) { cards.emplace_back(Card(rank, suit)); }
        }
        activeSize = cards.size();
    }

    void shuffle()
    {
        reset();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card popTop()
    {
        if (activeSize == 0) { throw std::out_of_range("No more cards in the deck!"); }
        Card topCard = cards[cards.size() - activeSize];
        activeSize--;
        return topCard;
    }

    void reset()
    {
        activeSize = cards.size();
    }

    bool isEmpty() const
    {
        return activeSize == 0;
    }

    size_t getRemainingCards() const
    {
        return activeSize;
    }
};
