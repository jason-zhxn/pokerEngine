#include "HandEvaluator.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Combine hole cards and community cards
std::vector<Card> HandEvaluator::mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards)
{
    std::vector<Card> fullHand = hand;
    fullHand.insert(fullHand.end(), communityCards.begin(), communityCards.end());
    return fullHand;
}

// Check if the hand is a flush
bool HandEvaluator::isFlush(const std::vector<Card> &hand)
{
    std::map<std::string, int> suitCount;
    for (const auto &card : hand) {
        suitCount[card.getSuit()]++;
        if (suitCount[card.getSuit()] >= 5) { return true; }
    }
    return false;
}

// Check if the hand is a straight
bool HandEvaluator::isStraight(const std::vector<Card> &hand, int &highCard)
{
    std::vector<int> ranks;
    for (const auto &card : hand) { ranks.push_back(card.getValue()); }

    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());

    // Ace-low straight (A-2-3-4-5)
    if (std::find(ranks.begin(), ranks.end(), 14) != ranks.end()) {
        ranks.push_back(1);// Treat Ace as 1
    }
    std::sort(ranks.begin(), ranks.end());

    // Check consecutive sequences
    for (size_t i = 0; i <= ranks.size() - 5; ++i) {
        bool isSequence = true;
        for (size_t j = 0; j < 4; ++j) {
            if (ranks[i + j + 1] - ranks[i + j] != 1) {
                isSequence = false;
                break;
            }
        }
        if (isSequence) {
            highCard = ranks[i + 4];// Highest card in the sequence
            return true;
        }
    }
    return false;
}

// Get rank frequencies for the hand
std::map<int, int> HandEvaluator::getRankFrequency(const std::vector<Card> &hand)
{
    std::map<int, int> rankFrequency;
    for (const auto &card : hand) { rankFrequency[card.getValue()]++; }
    return rankFrequency;
}

// Determine the best hand
HandEvaluator::HandResult
  HandEvaluator::determineBestHand(const std::map<int, int> &rankFrequency, bool flush, bool straight, int highCard)
{

    HandResult result;

    if (flush && straight) {
        result.rank = (highCard == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        result.highCards = { highCard };
        return result;
    }

    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    int pairCount = 0;

    for (const auto &[rank, count] : rankFrequency) {
        if (count == 4) {
            hasFourOfAKind = true;
            result.highCards.push_back(rank);
        }
        if (count == 3) {
            hasThreeOfAKind = true;
            result.highCards.push_back(rank);
        }
        if (count == 2) {
            pairCount++;
            result.highCards.push_back(rank);
        }
    }

    if (hasFourOfAKind) {
        result.rank = FOUR_OF_A_KIND;
    } else if (hasThreeOfAKind && pairCount > 0) {
        result.rank = FULL_HOUSE;
        std::sort(result.highCards.rbegin(), result.highCards.rend());
    } else if (flush) {
        result.rank = FLUSH;
    } else if (straight) {
        result.rank = STRAIGHT;
        result.highCards = { highCard };
    } else if (hasThreeOfAKind) {
        result.rank = THREE_OF_A_KIND;
    } else if (pairCount == 2) {
        result.rank = TWO_PAIR;
    } else if (pairCount == 1) {
        result.rank = ONE_PAIR;
    } else {
        result.rank = HIGH_CARD;
    }

    for (const auto &[rank, count] : rankFrequency) {
        for (int i = 0; i < count; ++i) { result.highCards.push_back(rank); }
    }

    std::sort(result.highCards.rbegin(), result.highCards.rend());

    return result;
}

// Evaluate the full hand
HandEvaluator::HandResult HandEvaluator::evaluateHand(const std::vector<Card> &hand,
  const std::vector<Card> &communityCards)
{

    std::vector<Card> fullHand = mergeHand(hand, communityCards);
    bool flush = isFlush(fullHand);
    int highCard = 0;
    bool straight = isStraight(fullHand, highCard);
    auto rankFrequency = getRankFrequency(fullHand);

    return determineBestHand(rankFrequency, flush, straight, highCard);
}
