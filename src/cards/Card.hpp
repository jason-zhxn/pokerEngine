#pragma once

#include <stdexcept>
#include <string>

class Card
{
  public:
    Card(const std::string &rank, const std::string &suit) : rank(rank), suit(suit) {}

    std::string getRank() const
    {
        return rank;
    }

    std::string getSuit() const
    {
        return suit;
    }

    int getValue() const
    {
        if (rank == "T") return 10;
        if (rank == "J") return 11;
        if (rank == "Q") return 12;
        if (rank == "K") return 13;
        if (rank == "A") return 14;
        try {
            return std::stoi(rank);
        } catch (const std::invalid_argument &) {
            throw std::invalid_argument("Invalid card rank: " + rank);
        }
    }

    std::string toString() const
    {
        return rank + " of " + suit;
    }

    bool operator==(const Card &other) const
    {
        return rank == other.rank && suit == other.suit;
    }

    bool operator!=(const Card &other) const
    {
        return !(*this == other);
    }

  private:
    std::string rank;
    std::string suit;
};
