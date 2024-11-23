#include "Card.h"
#include <stdexcept>

// Constructor
Card::Card(const std::string& rank, const std::string& suit)
    : rank(rank), suit(suit) {}

// Getters
std::string Card::getRank() const {
    return rank;
}

std::string Card::getSuit() const {
    return suit;
}

int Card::getValue() const {
    if (rank == "A") return 14;
    if (rank == "K") return 13;
    if (rank == "Q") return 12;
    if (rank == "J") return 11;
    
    // Try converting rank to integer
    try {
        return std::stoi(rank);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid rank: " + rank);
    }
}

// Returns a string representation of the card
std::string Card::toString() const {
    return rank + " of " + suit;
}
