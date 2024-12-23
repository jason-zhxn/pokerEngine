#pragma once
#include "Card.hpp"
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

    struct HandResult
    {
        HandRank rank;
        std::vector<int> highCards;
        std::string toString() const;
    };

    HandResult evaluateHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);

  private:
    bool isFlush(const std::vector<Card> &hand);
    bool isStraight(const std::vector<Card> &hand, int &highCard);
    std::map<int, int> getRankFrequency(const std::vector<Card> &hand);
    HandResult determineBestHand(const std::map<int, int> &rankFrequency, bool flush, bool straight, int highCard);

    std::vector<Card> mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);
};
