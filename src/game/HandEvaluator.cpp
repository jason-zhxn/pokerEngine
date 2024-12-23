#include "HandEvaluator.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::string rankToString(HandEvaluator::HandRank rank)
{
    switch (rank) {
    case HandEvaluator::HIGH_CARD:
        return "High Card";
    case HandEvaluator::ONE_PAIR:
        return "One Pair";
    case HandEvaluator::TWO_PAIR:
        return "Two Pair";
    case HandEvaluator::THREE_OF_A_KIND:
        return "Three of a Kind";
    case HandEvaluator::STRAIGHT:
        return "Straight";
    case HandEvaluator::FLUSH:
        return "Flush";
    case HandEvaluator::FULL_HOUSE:
        return "Full House";
    case HandEvaluator::FOUR_OF_A_KIND:
        return "Four of a Kind";
    case HandEvaluator::STRAIGHT_FLUSH:
        return "Straight Flush";
    case HandEvaluator::ROYAL_FLUSH:
        return "Royal Flush";
    default:
        return "Unknown Hand";
    }
}

bool HandEvaluator::HandResult::operator==(const HandResult &other) const
{
    return (rank == other.rank && identifier == other.identifier);
}

bool HandEvaluator::HandResult::operator>(const HandResult &other) const
{
    return (rank > other.rank) || (rank == other.rank && identifier > other.identifier);
}

std::string HandEvaluator::HandResult::toString() const
{
    std::ostringstream oss;
    oss << rankToString(rank) << " with high cards: ";
    for (size_t i = 0; i < highCards.size(); ++i) {
        oss << highCards[i];
        if (i < highCards.size() - 1) { oss << ", "; }
    }
    return oss.str();
}

std::vector<Card> HandEvaluator::mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards)
{
    std::vector<Card> fullHand = hand;
    fullHand.insert(fullHand.end(), communityCards.begin(), communityCards.end());
    return fullHand;
}

bool HandEvaluator::isFlush(const std::vector<Card> &hand)
{
    std::map<std::string, int> suitCount;
    for (const auto &card : hand) {
        suitCount[card.getSuit()]++;
        if (suitCount[card.getSuit()] >= 5) { return true; }
    }
    return false;
}

bool HandEvaluator::isStraight(const std::vector<Card> &hand, int &highCard)
{
    std::vector<int> ranks;
    for (const auto &card : hand) {
        int value = card.getValue();
        if (value < 2 || value > 14) { throw std::invalid_argument("Invalid card value: " + std::to_string(value)); }
        ranks.push_back(value);
    }

    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());
    if (ranks.size() < 5) { return false; }


    if (std::find(ranks.begin(), ranks.end(), 14) != ranks.end()) {
        ranks.push_back(1);
        std::sort(ranks.begin(), ranks.end());
    }

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

    if (flush && straight) {//handle royal and straight flushes
        result.rank = (highCard == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
        result.identifier = { highCard };
        result.highCards = { };
        return result;
    }

    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    int threeOfAKindRank = 0;
    int fourOfAKindRank = 0;
    std::vector<int> pairs;
    std::vector<int> kickers;

    for (const auto &[rank, count] : rankFrequency) {
        if (count == 4) {
            hasFourOfAKind = true;
            fourOfAKindRank = rank;
        } else if (count == 3) {
            hasThreeOfAKind = true;
            threeOfAKindRank = rank;
        } else if (count == 2) {
            pairs.push_back(rank);
        } else {
            kickers.push_back(rank);
        }
    }

    std::sort(pairs.rbegin(), pairs.rend());
    std::sort(kickers.rbegin(), kickers.rend());

    if (hasFourOfAKind) {
        result.rank = FOUR_OF_A_KIND;
        result.identifier = { fourOfAKindRank };
        result.highCards = { kickers[0] };
        return result;
    }

    if (hasThreeOfAKind && !pairs.empty()) {
        result.rank = FULL_HOUSE;
        result.identifier = { threeOfAKindRank, pairs[0] };
        result.highCards = { };
        return result;
    }

    if (flush) {
        result.rank = FLUSH;
        result.highCards.clear();
        for (const auto &[rank, count] : rankFrequency) {
            result.highCards.push_back(rank);
            if (result.highCards.size() == 5) break;//Only top 5 cards
        }
        std::sort(result.highCards.rbegin(), result.highCards.rend());
        return result;
    }

    if (straight) {
        result.rank = STRAIGHT;
        result.highCards = { highCard };
        return result;
    }

    if (hasThreeOfAKind) {
        result.rank = THREE_OF_A_KIND;
        result.identifier = { threeOfAKindRank };
        result.highCards = { kickers[0], kickers[1] };
        return result;
    }

    if (pairs.size() >= 2) {
        result.rank = TWO_PAIR;
        result.identifier = { pairs[0], pairs[1] };
        result.highCards.push_back(kickers[0]);//need to address counterfeit here i believe
        return result;
    }

    if (pairs.size() == 1) {
        result.rank = ONE_PAIR;
        result.identifier = { pairs[0] };
        result.highCards = { kickers[0], kickers[1], kickers[2] };
        return result;
    }

    result.rank = HIGH_CARD;
    result.highCards = { kickers[0], kickers[1], kickers[2], kickers[3], kickers[4] };
    return result;
}

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
