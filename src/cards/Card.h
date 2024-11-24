#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
private:
    std::string rank; 
    std::string suit; 

public:

    // Constructor
    Card(const std::string& rank, const std::string& suit);

    // Getters
    std::string getRank() const;
    std::string getSuit() const;
    int getValue() const;

    // To-string representation
    std::string toString() const; // Returns a string representation of the card (e.g., "Ace of Hearts")
    
};

#endif // CARD_H
