#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>

#include "Deck.hpp"

Deck::Deck()
{
    const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
    const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
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

void Deck::shuffle(bool includeRemoved)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.begin(), std::default_random_engine(seed));
}

bool Deck::isEmpty() const
{
    return activeSize == 0;
}

int Deck::getLength() const
{
    return static_cast<int>(activeSize);
}
