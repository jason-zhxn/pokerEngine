#include "HandEvaluator.h"

// Check if the hand is a flush (all cards of the same suit)
bool HandEvaluator::isFlush(const std::vector<Card> &hand)
{
    if (hand.size() < 5) return false;

    std::string suit = hand[0].getSuit();
    for (const auto &card : hand) {
        if (card.getSuit() != suit) { return false; }
    }
    return true;
}

// Check if the hand is a straight (consecutive ranks)
bool HandEvaluator::isStraight(const std::vector<Card> &hand, int &highCard)
{
    if (hand.size() < 5) return false;

    std::vector<int> ranks;
    for (const auto &card : hand) { ranks.push_back(card.getValue()); }

    // Remove duplicates
    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());

    // For Ace-low straight, add Ace as 1 if Ace is present
    bool hasAce = (std::find(ranks.begin(), ranks.end(), 14) != ranks.end());
    if (hasAce) {
        ranks.push_back(1);// Consider Ace as low
    }

    // Sort ranks again
    std::sort(ranks.begin(), ranks.end());

    // Ensure there are at least 5 unique ranks to form a straight
    if (ranks.size() < 5) return false;

    // Now check for straights
    for (size_t i = 0; i <= ranks.size() - 5; ++i) {
        bool isSequence = true;
        for (size_t j = 0; j < 4; ++j) {
            if (ranks[i + j + 1] - ranks[i + j] != 1) {
                isSequence = false;
                break;
            }
        }
        if (isSequence) {
            highCard = ranks[i + 4] == 1 ? 14 : ranks[i + 4];// Adjust highCard for Ace
            return true;
        }
    }

    return false;
}

// Get a map of rank frequencies
std::map<int, int> HandEvaluator::getRankFrequency(const std::vector<Card> &hand)
{
    std::map<int, int> rankFrequency;
    for (const auto &card : hand) { rankFrequency[card.getValue()]++; }
    return rankFrequency;
}

// Determine the highest rank in the hand
HandEvaluator::HandResult
  HandEvaluator::getHighestRank(const std::map<int, int> &rankFrequency, bool isFlush, bool isStraight, int highCard)
{
    HandResult result;

    if (isStraight && isFlush) {
        // For Royal Flush, check if the straight starts at 10 and ends with Ace
        if (highCard == 14 && rankFrequency.count(10) > 0) {
            result.rank = ROYAL_FLUSH;
        } else {
            result.rank = STRAIGHT_FLUSH;
        }
        result.highCards = { highCard };
    } else {
        bool hasFourOfAKind = false;
        bool hasThreeOfAKind = false;
        int pairCount = 0;

        for (const auto &pair : rankFrequency) {
            if (pair.second == 4) {
                hasFourOfAKind = true;
            } else if (pair.second == 3) {
                hasThreeOfAKind = true;
            } else if (pair.second == 2) {
                pairCount++;
            }
        }

        if (hasFourOfAKind) {
            result.rank = FOUR_OF_A_KIND;
        } else if (hasThreeOfAKind && pairCount > 0) {
            result.rank = FULL_HOUSE;
        } else if (isFlush) {
            result.rank = FLUSH;
        } else if (isStraight) {
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
            for (const auto &[rank, freq] : rankFrequency) {
                for (int i = 0; i < freq; ++i) { result.highCards.push_back(rank); }
            }
            std::sort(result.highCards.rbegin(), result.highCards.rend());
        }
    }

    return result;
}

// Evaluate the hand
HandEvaluator::HandResult HandEvaluator::evaluateHand(const std::vector<Card> &hand)
{
    if (hand.size() < 5) { throw std::invalid_argument("Hand must have at least 5 cards."); }

    bool flush = isFlush(hand);
    int highCard = 0;
    bool straight = isStraight(hand, highCard);
    auto rankFrequency = getRankFrequency(hand);

    return getHighestRank(rankFrequency, flush, straight, highCard);
}
