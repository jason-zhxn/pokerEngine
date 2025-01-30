#pragma once

#include "Card.hpp"
#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
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

    class HandResult
    {
      public:
        HandRank rank;
        std::vector<int> identifier;
        std::vector<int> highCards;

        std::string toString() const;
        bool operator==(const HandResult &other) const;
        bool operator>(const HandResult &other) const;
        bool operator<(const HandResult &other) const;
    };

    HandResult evaluateHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);

  private:
    std::vector<Card> mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);

    bool isFlush(const std::vector<Card> &allCards, std::string &flushSuit);

    bool isStraight(const std::vector<int> &ranks, int &highCard);

    HandResult determineBestHand(const std::vector<Card> &allCards);
};
