#include "Card.hpp"
#include "HandEvaluator.hpp"
#include <gtest/gtest.h>

using cardVec = std::vector<Card>;

//
// Basic Tests
//
TEST(HandEvaluatorTest, RoyalFlush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Hearts"), Card("J", "Hearts") };
    cardVec communityCards = {
        Card("Q", "Hearts"), Card("K", "Hearts"), Card("A", "Hearts"), Card("4", "Spades"), Card("9", "Hearts")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.identifier.empty()) << "Identifier should not be empty for Royal Flush";

    EXPECT_EQ(result.rank, HandEvaluator::ROYAL_FLUSH);
    EXPECT_EQ(result.identifier[0], 14);
}

TEST(HandEvaluatorTest, StraightFlush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("9", "Clubs"), Card("T", "Clubs") };
    cardVec communityCards = {
        Card("J", "Clubs"), Card("Q", "Clubs"), Card("K", "Clubs"), Card("2", "Hearts"), Card("3", "Hearts")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.identifier.empty()) << "Identifier should not be empty for Straight Flush";

    EXPECT_EQ(result.rank, HandEvaluator::STRAIGHT_FLUSH);
    EXPECT_EQ(result.identifier[0], 13);
}

TEST(HandEvaluatorTest, QuadsOverQuads)
{
    HandEvaluator evaluator;
    // Hand 1 is Quad 9's
    cardVec hand1 = { Card("9", "Clubs"), Card("9", "Diamonds") };
    // Hand 2 is Quad 10's
    cardVec hand2 = { Card("10", "Clubs"), Card("10", "Diamonds") };
    cardVec communityCards = {
        Card("9", "Hearts"), Card("9", "Spades"), Card("10", "Hearts"), Card("10", "Spades"), Card("2", "Hearts")
    };

    auto result1 = evaluator.evaluateHand(hand1, communityCards);
    auto result2 = evaluator.evaluateHand(hand2, communityCards);

    EXPECT_EQ(result1.highCards[0], 10);
    EXPECT_EQ(result2.highCards[0], 9);

    ASSERT_FALSE(result1.identifier.empty()) << "Identifier for result1 should not be empty";
    ASSERT_FALSE(result2.identifier.empty()) << "Identifier for result2 should not be empty";

    EXPECT_LT(result1, result2);
}

TEST(HandEvaluatorTest, Quads)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("9", "Clubs"), Card("9", "Diamonds") };
    cardVec communityCards = {
        Card("9", "Hearts"), Card("9", "Spades"), Card("K", "Clubs"), Card("4", "Hearts"), Card("6", "Diamonds")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.identifier.empty()) << "Identifier should not be empty for Quads";
    ASSERT_FALSE(result.highCards.empty()) << "HighCards should not be empty for Quads";

    EXPECT_EQ(result.rank, HandEvaluator::FOUR_OF_A_KIND);
    EXPECT_EQ(result.identifier[0], 9);
    EXPECT_EQ(result.highCards[0], 13);
}

TEST(HandEvaluatorTest, FullHouse)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Clubs"), Card("T", "Diamonds") };
    cardVec communityCards = {
        Card("T", "Hearts"), Card("J", "Spades"), Card("J", "Clubs"), Card("4", "Hearts"), Card("9", "Diamonds")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.identifier.empty()) << "Identifier should not be empty for Full House";

    EXPECT_EQ(result.rank, HandEvaluator::FULL_HOUSE);
    EXPECT_EQ(result.identifier[0], 10);
    EXPECT_EQ(result.identifier[1], 11);
    EXPECT_EQ(result.highCards.size(), 0);
}

TEST(HandEvaluatorTest, Flush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("2", "Hearts"), Card("5", "Hearts") };
    cardVec communityCards = {
        Card("9", "Hearts"), Card("J", "Hearts"), Card("K", "Hearts"), Card("3", "Diamonds"), Card("8", "Spades")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.highCards.empty()) << "HighCards should not be empty for Flush";

    EXPECT_EQ(result.rank, HandEvaluator::FLUSH);
    EXPECT_EQ(result.identifier[0], 13);
    EXPECT_EQ(result.highCards.size(), 5);
    EXPECT_EQ(result.highCards[0], 13);
    EXPECT_EQ(result.highCards[1], 11);
    EXPECT_EQ(result.highCards[2], 9);
    EXPECT_EQ(result.highCards[3], 5);
    EXPECT_EQ(result.highCards[4], 2);
}

TEST(HandEvaluatorTest, Straight)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("2", "Diamonds"), Card("3", "Clubs") };
    cardVec communityCards = {
        Card("4", "Hearts"), Card("5", "Spades"), Card("6", "Clubs"), Card("K", "Hearts"), Card("7", "Diamonds")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    ASSERT_FALSE(result.identifier.empty()) << "Identifier should not be empty for Straight";

    EXPECT_EQ(result.rank, HandEvaluator::STRAIGHT);
    EXPECT_EQ(result.identifier[0], 6);
}

TEST(HandEvaluatorTest, ThreeOfAKind)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Clubs"), Card("T", "Diamonds") };
    cardVec communityCards = { Card("T", "Hearts"), Card("J", "Spades"), Card("K", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::THREE_OF_A_KIND);
    EXPECT_EQ(result.identifier[0], 10);
    EXPECT_EQ(result.highCards[0], 13);
    EXPECT_EQ(result.highCards[1], 11);
}

TEST(HandEvaluatorTest, TwoPair)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Clubs"), Card("T", "Diamonds") };
    cardVec communityCards = { Card("J", "Hearts"), Card("J", "Spades"), Card("K", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::TWO_PAIR);
    EXPECT_EQ(result.identifier[0], 11);
    EXPECT_EQ(result.identifier[1], 10);
    EXPECT_EQ(result.highCards[0], 13);
}

TEST(HandEvaluatorTest, TwoPairCounterfeited)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("10", "Clubs"), Card("10", "Diamonds") };
    cardVec communityCards = {
        Card("J", "Hearts"), Card("J", "Spades"), Card("K", "Clubs"), Card("K", "Hearts"), Card("8", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::TWO_PAIR);
    EXPECT_EQ(result.identifier[0], 13);
    EXPECT_EQ(result.identifier[1], 11);
    EXPECT_EQ(result.highCards[0], 10);
}

TEST(HandEvaluatorTest, OnePair)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("10", "Clubs"), Card("10", "Diamonds") };
    cardVec communityCards = { Card("J", "Hearts"), Card("Q", "Spades"), Card("K", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::ONE_PAIR);
    EXPECT_EQ(result.identifier[0], 10);
    EXPECT_EQ(result.highCards[0], 13);
    EXPECT_EQ(result.highCards[1], 12);
    EXPECT_EQ(result.highCards[2], 11);
}

TEST(HandEvaluatorTest, HighCard)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("2", "Diamonds"), Card("5", "Clubs") };
    cardVec communityCards = { Card("7", "Hearts"), Card("9", "Spades"), Card("J", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::HIGH_CARD);
    EXPECT_EQ(result.identifier[0], 11);
    EXPECT_EQ(result.highCards[0], 9);
    EXPECT_EQ(result.highCards[1], 7);
    EXPECT_EQ(result.highCards[2], 5);
    EXPECT_EQ(result.highCards[3], 2);
}

//
// Different Kicker Tests
//
TEST(HandEvaluatorTest, TwoPairsWithDifferentKickers)
{
    HandEvaluator evaluator;

    cardVec communityCards = {
        Card("K", "Hearts"), Card("7", "Diamonds"), Card("3", "Clubs"), Card("3", "Spades"), Card("2", "Hearts")
    };

    // KK77A
    cardVec hand1 = { Card("K", "Diamonds"), Card("A", "Clubs") };

    // KK77Q
    cardVec hand2 = { Card("K", "Clubs"), Card("Q", "Spades") };

    auto result1 = evaluator.evaluateHand(hand1, communityCards);
    auto result2 = evaluator.evaluateHand(hand2, communityCards);

    EXPECT_EQ(result1.rank, HandEvaluator::TWO_PAIR);
    EXPECT_EQ(result2.rank, HandEvaluator::TWO_PAIR);

    EXPECT_GT(result1, result2);
}
