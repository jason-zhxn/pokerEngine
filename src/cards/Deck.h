#pragma once

#include <vector>
#include "Card.h"

class Deck
{
private:
    std::vector<Card> cards;
    size_t activeSize;

public:
    Deck();
    Card popTop();
    void shuffle(bool includeRemoved = true);
    bool isEmpty() const;
    int getLength() const;
};
