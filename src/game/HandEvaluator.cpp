#include "HandEvaluator.hpp"

// Simple rank-to-string helper for debugging
static std::string rankToString(HandEvaluator::HandRank rank)
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
    return rank == other.rank && identifier == other.identifier && highCards == other.highCards;
}

bool HandEvaluator::HandResult::operator>(const HandResult &other) const
{
    if (rank > other.rank) { return true; }
    if (rank < other.rank) { return false; }

    // Apparently > operation on vectors compare lexicographically
    if (identifier != other.identifier) { return identifier > other.identifier; }
    return highCards > other.highCards;
}

bool HandEvaluator::HandResult::operator<(const HandResult &other) const
{
    return !(*this > other || *this == other);
}

std::string HandEvaluator::HandResult::toString() const
{
    std::ostringstream oss;
    oss << rankToString(rank) << " with identifiers: ";
    for (size_t i = 0; i < identifier.size(); ++i) {
        oss << identifier[i];
        if (i < identifier.size() - 1) { oss << ", "; }
    }
    return oss.str();
}

std::vector<Card> HandEvaluator::mergeHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards)
{
    if (hand.size() > 2 || communityCards.size() > 5) {
        throw std::logic_error("Maximum of 2 hole cards and 5 community cards allowed.");
    }
    std::vector<Card> fullHand = hand;
    fullHand.insert(fullHand.end(), communityCards.begin(), communityCards.end());
    return fullHand;
}

bool HandEvaluator::isFlush(const std::vector<Card> &allCards, std::string &flushSuit)
{
    std::map<std::string, int> suitCount;
    for (const auto &card : allCards) {
        suitCount[card.getSuit()]++;
        if (suitCount[card.getSuit()] >= 5) {
            flushSuit = card.getSuit();
            return true;
        }
    }
    return false;
}

bool HandEvaluator::isStraight(const std::vector<int> &ranks, int &highCard)
{
    if (ranks.size() < 5) { return false; }

    std::vector<int> sorted(ranks.begin(), ranks.end());

    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    if (sorted[0] == 14) { sorted.push_back(1); }

    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    for (size_t i = 0; i + 4 < sorted.size(); ++i) {
        if (sorted[i] - sorted[i + 4] == 4) {
            highCard = sorted[i];
            return true;
        }
    }

    return false;
}

HandEvaluator::HandResult HandEvaluator::determineBestHand(const std::vector<Card> &allCards)
{
    HandResult result;

    std::string flushSuit;
    bool hasFlush = isFlush(allCards, flushSuit);

    std::vector<int> ranks;
    ranks.reserve(allCards.size());
    for (auto &c : allCards) { ranks.push_back(c.getValue()); }

    int topStraightRank = 0;
    bool hasStraight = isStraight(ranks, topStraightRank);

    std::map<int, int> freq;
    for (int r : ranks) { freq[r]++; }
    // Sort them by (count DESC, rank DESC)
    std::vector<std::pair<int, int>> freqVec;
    freqVec.reserve(freq.size());
    for (auto &[val, count] : freq) { freqVec.push_back({ count, val }); }
    std::sort(freqVec.begin(), freqVec.end(), [](auto &a, auto &b) {
        if (a.first == b.first) { return a.second > b.second; }
        return a.first > b.first;
    });

    std::vector<int> ranksDesc;
    for (auto &[val, count] : freq) {
        for (int i = 0; i < count; i++) { ranksDesc.push_back(val); }
    }
    std::sort(ranksDesc.begin(), ranksDesc.end(), std::greater<int>());

    auto pickKickers = [&](const std::initializer_list<int> &used, int howMany) {
        std::vector<int> picked;
        for (int r : ranksDesc) {
            if (std::find(used.begin(), used.end(), r) != used.end()) {
                continue;// skip ranks we've used in the main combo
            }
            picked.push_back(r);
            if ((int)picked.size() == howMany) { break; }
        }
        return picked;
    };

    auto getFlushCards = [&]() {
        std::vector<int> fCards;
        for (auto &c : allCards) {
            if (c.getSuit() == flushSuit) { fCards.push_back(c.getValue()); }
        }
        std::sort(fCards.begin(), fCards.end(), std::greater<int>());
        if (fCards.size() > 5) { fCards.resize(5); }
        return fCards;
    };

    // c0/r0 is the top frequency + rank, c1/r1 the second
    int c0 = freqVec[0].first;
    int r0 = freqVec[0].second;
    int c1 = (freqVec.size() > 1) ? freqVec[1].first : 0;
    int r1 = (freqVec.size() > 1) ? freqVec[1].second : 0;

    // Royal / Straight Flush
    if (hasFlush) {
        auto flushCards = getFlushCards();
        int flushStraightHighCard = 0;
        bool flushHasStraight = isStraight(flushCards, flushStraightHighCard);

        if (flushHasStraight) {
            result.rank = (flushStraightHighCard == 14) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
            result.identifier = { flushStraightHighCard };
            return result;
        }
    }


    // Quads
    if (c0 == 4) {
        result.rank = FOUR_OF_A_KIND;
        result.identifier = { r0 };
        result.highCards = pickKickers({ r0 }, 1);
        return result;
    }

    // Full house
    if (c0 == 3 && c1 >= 2) {
        result.rank = FULL_HOUSE;
        result.identifier = { r0, r1 };
        return result;
    }

    // Flush
    if (hasFlush) {
        result.rank = FLUSH;
        auto fCards = getFlushCards();
        result.identifier = { fCards[0] };
        result.highCards.insert(result.highCards.end(), fCards.begin(), fCards.end());
        return result;
    }

    // Straight
    if (hasStraight) {
        result.rank = STRAIGHT;
        result.identifier = { topStraightRank };
        return result;
    }

    // Trips
    if (c0 == 3) {
        result.rank = THREE_OF_A_KIND;
        result.identifier = { r0 };
        // pick 2 kickers
        result.highCards = pickKickers({ r0 }, 2);
        return result;
    }

    // Two pair
    if (c0 == 2 && c1 == 2) {
        result.rank = TWO_PAIR;
        result.identifier = { r0, r1 };
        result.highCards = pickKickers({ r0, r1 }, 1);
        return result;
    }

    // Pair
    if (c0 == 2) {
        result.rank = ONE_PAIR;
        result.identifier = { r0 };
        result.highCards = pickKickers({ r0 }, 3);
        return result;
    }

    // High card
    result.rank = HIGH_CARD;
    if (!ranksDesc.empty()) { result.identifier.push_back(ranksDesc[0]); }
    for (size_t i = 1; i < ranksDesc.size() && result.highCards.size() < 4; ++i) {
        result.highCards.push_back(ranksDesc[i]);
    }
    return result;
}

std::string HandEvaluator::fastEvaluateHand(const std::vector<Card> &hand, const std::vector<Card> &communityCards)
{
    std::vector<Card> fullHand = mergeHand(hand, communityCards);
    // return determineBestHand(fullHand);
    int cards[7] = { 0 };
    int suit_hash = 0;
    for (int i = 0; i < 7; i++) {
        cards[i] = fullHand[i].getHash();
        suit_hash += (1 << ((cards[i] % 4) * 3));
    }

    if (SUITS_TABLE[suit_hash]) {
        int suit_binary[4] = { 0 };

        for (int i = 0; i < 7; i++) { suit_binary[cards[i] & 0x3] |= (1 << (cards[i] / 4)); }
        return RANK_DESCRIPTION[FLUSH_TABLE[suit_binary[SUITS_TABLE[suit_hash] - 1]]][0];
    }

    unsigned char quinary[13] = { 0 };

    for (int i = 0; i < 7; i++) { quinary[cards[i] / 4]++; }

    const int hash = hashQuinaryResult(quinary);

    return RANK_DESCRIPTION[NOFLUSH_TABLE[hash]][0];
}

int HandEvaluator::hashQuinaryResult(const unsigned char quinary[])
{
    int sum = 0;
    const int len = 13;
    int i;
    int k = 7;

    for (i = 0; i < len; i++) {
        sum += DP_TABLE[quinary[i]][len - i - 1][k];

        k -= quinary[i];

        if (k <= 0) { break; }
    }

    return sum;
}

HandEvaluator::HandResult HandEvaluator::evaluateHand(const std::vector<Card> &hand,
  const std::vector<Card> &communityCards)
{
    std::vector<Card> fullHand = mergeHand(hand, communityCards);
    return determineBestHand(fullHand);
}
