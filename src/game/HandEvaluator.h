#pragma once
#include <vector>
#include <map>
#include "Card.h"

class HandEvaluator {
public:
    enum HandRank {
        HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND,
        STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND,
        STRAIGHT_FLUSH, ROYAL_FLUSH
    };

    // Struct to store evaluation results
    struct HandResult {
        HandRank rank;
        std::vector<int> highCards;
    };

    // Evaluate a hand and return its ranking
    HandResult evaluateHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);

private:
    // Utility methods for hand evaluation
    bool isFlush(const std::vector<Card> &hand);
    bool isStraight(const std::vector<Card> &hand, int &highCard);
    std::map<int, int> getRankFrequency(const std::vector<Card> &hand);
    HandResult determineBestHand(const std::map<int, int> &rankFrequency, bool flush, bool straight, int highCard);

    // Combine player's hole cards and community cards
    std::vector<Card> mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards);
};
