#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>

#include "Deck.hpp"

Deck::Deck()
{
    const std::vector<char> suits = { 'H', 'D', 'C', 'S' };
    const std::vector<char> ranks = { '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K', 'A' };
    for (const auto &suit : suits) {
        for (const auto &rank : ranks) { cards.emplace_back(Card(rank, suit)); }
    }
    activeSize = cards.size();
}

Card Deck::popTop()
{
    if (isEmpty()) { throw std::out_of_range("No cards left in deck"); }
    return cards[--activeSize];
}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    activeSize = cards.size();
}

bool Deck::isEmpty() const
{
    return activeSize == 0;
}

int Deck::getLength() const
{
    return static_cast<int>(activeSize);
}
