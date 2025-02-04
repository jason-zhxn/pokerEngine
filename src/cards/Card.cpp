#include "Card.hpp"
#include <stdexcept>

Card::Card(const char &rank, const char &suit) : rank(rank), suit(suit) {}

char Card::getRank() const
{
    return rank;
}
char Card::getSuit() const
{
    return suit;
}

int Card::getValue() const
{

    switch (rank){
        case 'A':
            return 14;
        case 'K':
            return 13;
        case 'Q':
            return 12;
        case 'J':
            return 11;
        case 'T':
            return 10;
        default:
            break;
    }

    return rank - '0';
}

std::string Card::toString() const
{
    std::string rank_str = "a";
    rank_str[0] = rank;
    switch (suit){
        case 'H':
            return rank_str + " of Hearts";
        case 'D':
            return rank_str + " of Diamonds";
        case 'S':
            return rank_str + " of Spades";
        case 'C':
            return rank_str + " of Clubs";
    }

    return "Error. Invalid suit.\n";
}

bool Card::operator==(const Card &other) const
{
    return rank == other.rank && suit == other.suit;
}

bool Card::operator!=(const Card &other) const
{
    return !(*this == other);
}
