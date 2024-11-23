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

    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());// Remove duplicates

    for (size_t i = 0; i <= ranks.size() - 5; ++i) {
        if (ranks[i + 4] - ranks[i] == 4) {
            highCard = ranks[i + 4];
            return true;
        }
    }

    // Special case: Ace-low straight (A, 2, 3, 4, 5)
    if (std::find(ranks.begin(), ranks.end(), 14) != ranks.end() && ranks[0] == 2 && ranks[1] == 3 && ranks[2] == 4
        && ranks[3] == 5) {
        highCard = 5;
        return true;
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
        result.rank = (highCard == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        result.highCards = { highCard };
    } else if (rankFrequency.size() == 2) {
        auto it = rankFrequency.begin();
        if (it->second == 4 || (++it)->second == 4) {
            result.rank = FOUR_OF_A_KIND;
        } else {
            result.rank = FULL_HOUSE;
        }
    } else if (isFlush) {
        result.rank = FLUSH;
    } else if (isStraight) {
        result.rank = STRAIGHT;
        result.highCards = { highCard };
    } else if (rankFrequency.size() == 3) {
        if (std::any_of(
              rankFrequency.begin(), rankFrequency.end(), [](const auto &pair) { return pair.second == 3; })) {
            result.rank = THREE_OF_A_KIND;
        } else {
            result.rank = TWO_PAIR;
        }
    } else if (rankFrequency.size() == 4) {
        result.rank = ONE_PAIR;
    } else {
        result.rank = HIGH_CARD;
        for (const auto &[rank, freq] : rankFrequency) { result.highCards.push_back(rank); }
        std::sort(result.highCards.rbegin(), result.highCards.rend());
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
