#include "Deck.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>

// Constructor: Initialize the deck with all cards
Deck::Deck()
{
    const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
    const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    for (const auto &suit : suits) {
        for (const auto &rank : ranks) {
            cards.emplace_back(Card(rank, suit));// add all cards to the deck
        }
    }
    activeSize = cards.size();// activeSize as a pointer to ignore "dealt" cards
}

// Remove and return the top card from the deck
Card Deck::popTop()
{
    if (isEmpty()) {// this should never happen
        throw std::out_of_range("No cards left in deck");
    }
    return cards[--activeSize];
}

// Shuffle the deck (restoring the dealt cards)
void Deck::shuffle(bool includeRemoved)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.begin(), std::default_random_engine(seed));
}

// Return true if the deck is empty
bool Deck::isEmpty() const
{
    return activeSize == 0;
}

// Return the number of active cards in the deck
int Deck::getLength() const
{
    return static_cast<int>(activeSize);
}
