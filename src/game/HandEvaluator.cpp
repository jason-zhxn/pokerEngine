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

// Check for Straight
bool HandEvaluator::isStraight(const std::vector<Card> &hand, int &highCard)
{
    // Extract card values
    std::vector<int> ranks;
    for (const auto &card : hand) {
        int value = card.getValue();
        if (value < 2 || value > 14) { throw std::invalid_argument("Invalid card value: " + std::to_string(value)); }
        ranks.push_back(value);
    }

    // Sort and deduplicate
    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());
    // Early exit if fewer than 5 cards
    if (ranks.size() < 5) { return false; }


    // Handle Ace-low straight (A-2-3-4-5)
    if (std::find(ranks.begin(), ranks.end(), 14) != ranks.end()) {
        ranks.push_back(1);
        std::sort(ranks.begin(), ranks.end());
    }

    // Check for 5 consecutive cards
    for (size_t i = 0; i <= ranks.size() - 5; ++i) {
        if (ranks[i + 4] - ranks[i] == 4
            && std::adjacent_find(ranks.begin() + i, ranks.begin() + i + 5, [](int a, int b) { return b - a != 1; })
                 == ranks.begin() + i + 5) {
            highCard = ranks[i + 4];
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

HandEvaluator::HandResult
  HandEvaluator::determineBestHand(const std::map<int, int> &rankFrequency, bool flush, bool straight, int highCard)
{
    HandResult result;

    // Flush and straight hands
    if (flush && straight) {
        result.rank = (highCard == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        result.highCards = { highCard };
        return result;
    }

    // Variables to track best hand components
    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    int threeOfAKindRank = 0;
    std::vector<int> pairs;
    std::vector<int> kickers;

    // Analyze rank frequencies
    for (const auto &[rank, count] : rankFrequency) {
        if (count == 4) {
            hasFourOfAKind = true;
            result.highCards.push_back(rank);
        } else if (count == 3) {
            hasThreeOfAKind = true;
            threeOfAKindRank = rank;
        } else if (count == 2) {
            pairs.push_back(rank);
        } else {
            kickers.push_back(rank);
        }
    }

    // Sort pairs and kickers in descending order
    std::sort(pairs.rbegin(), pairs.rend());
    std::sort(kickers.rbegin(), kickers.rend());

    // Handle Four of a Kind
    if (hasFourOfAKind) {
        result.rank = FOUR_OF_A_KIND;
        result.highCards = { result.highCards[0], kickers[0] };
        return result;
    }

    // Handle Full House
    if (hasThreeOfAKind && !pairs.empty()) {
        result.rank = FULL_HOUSE;
        result.highCards = { threeOfAKindRank, pairs[0] };
        return result;
    }

    // Handle Flush
    if (flush) {
        result.rank = FLUSH;
        result.highCards.clear();
        for (const auto &[rank, count] : rankFrequency) {
            result.highCards.push_back(rank);
            if (result.highCards.size() == 5) break;// Only top 5 cards
        }
        std::sort(result.highCards.rbegin(), result.highCards.rend());
        return result;
    }

    // Handle Straight
    if (straight) {
        result.rank = STRAIGHT;
        result.highCards = { highCard };
        return result;
    }

    // Handle Three of a Kind
    if (hasThreeOfAKind) {
        result.rank = THREE_OF_A_KIND;
        result.highCards = { threeOfAKindRank };
        result.highCards.insert(result.highCards.end(), kickers.begin(), kickers.begin() + 2);
        return result;
    }

    // Handle Two Pair
    if (pairs.size() >= 2) {
        result.rank = TWO_PAIR;
        result.highCards = { pairs[0], pairs[1] };
        result.highCards.push_back(kickers[0]);// Add highest kicker
        return result;
    }

    // Handle One Pair
    if (pairs.size() == 1) {
        result.rank = ONE_PAIR;
        result.highCards = { pairs[0] };
        result.highCards.insert(result.highCards.end(), kickers.begin(), kickers.begin() + 3);
        return result;
    }

    // Handle High Card
    result.rank = HIGH_CARD;
    result.highCards.insert(result.highCards.end(), kickers.begin(), kickers.begin() + 5);
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
