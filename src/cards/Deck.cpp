#include "Deck.h"

Deck::Deck()
{
    const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
    const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    for (const auto &suit : suits) {
        for (const auto &rank : ranks) { cards.emplace_back(Card(rank, suit)); }
    }
}

Card Deck::popTop()
{
    if (cards.empty()) { throw std::out_of_range("No cards left in deck"); }
    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}

void Deck::shuffle()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

int Deck::getLength() const
{
    return cards.size();
}
