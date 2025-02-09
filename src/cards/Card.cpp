#include "Card.hpp"
#include <stdexcept>
#include <string>

Card::Card(const std::string &rank, const std::string &suit) : rank(rank), suitStr(suit) {}

std::string Card::getRank() const
{
    return rank;
}
std::string Card::getSuit() const
{
    return suitStr;
}

int Card::getValue() const
{
    if (rank == "A") return 14;
    if (rank == "K") return 13;
    if (rank == "Q") return 12;
    if (rank == "J") return 11;
    if (rank == "T") return 10;

    try {
        return std::stoi(rank);
    } catch (const std::invalid_argument &) {
        throw std::runtime_error("Invalid rank: " + rank);
    }
}

std::string Card::toString() const
{
    return rank + " of " + suitStr;
}

int Card::getHash() const
{
    int rankValue = getValue();
    return ((rankValue - 2) << 2) + (suitStr == "H" ? 0 : suitStr == "D" ? 1 : suitStr == "C" ? 2 : 3);
}

bool Card::operator==(const Card &other) const
{
    return rank == other.rank && suitStr == other.suitStr;
}

bool Card::operator!=(const Card &other) const
{
    return !(*this == other);
}
