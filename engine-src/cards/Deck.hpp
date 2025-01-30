#pragma once

#include "Card.hpp"
#include <vector>

class Deck
{
  private:
    std::vector<Card> cards;
    size_t activeSize;

  public:
    Deck();
    Card popTop();
    void shuffle();
    bool isEmpty() const;
    int getLength() const;
};
