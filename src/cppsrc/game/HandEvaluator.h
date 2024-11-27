#pragma once

#include "Card.h"
#include <algorithm>
#include <map>
#include <vector>

class HandEvaluator
{
  public:
    enum HandRank {
        HIGH_CARD,
        ONE_PAIR,
        TWO_PAIR,
        THREE_OF_A_KIND,
        STRAIGHT,
        FLUSH,
        FULL_HOUSE,
        FOUR_OF_A_KIND,
        STRAIGHT_FLUSH,
        ROYAL_FLUSH
    };

    // Struct to hold the evaluation result
    struct HandResult
    {
        HandRank rank;
        std::vector<int> highCards;// High card values to resolve ties
    };

    // Evaluate a hand of 5-7 cards and return the result
    static HandResult evaluateHand(const std::vector<Card> &hand);

  private:
    static bool isFlush(const std::vector<Card> &hand);
    static bool isStraight(const std::vector<Card> &hand, int &highCard);
    static std::map<int, int> getRankFrequency(const std::vector<Card> &hand);
    static HandResult
      getHighestRank(const std::map<int, int> &rankFrequency, bool isFlush, bool isStraight, int highCard);
};
