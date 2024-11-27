#pragma once

#include "Card.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

class Deck
{
  public:
    Deck();
    Card popTop();
    void shuffle();
    int getLength() const;

  private:
    std::vector<Card> cards;
};
