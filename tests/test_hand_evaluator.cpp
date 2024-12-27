#include "Card.hpp"
#include "HandEvaluator.hpp"
#include <gtest/gtest.h>

using cardVec = std::vector<Card>;

TEST(HandEvaluatorTest, RoyalFlush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Hearts"), Card("J", "Hearts") };
    cardVec communityCards = { Card("Q", "Hearts"), Card("K", "Hearts"), Card("A", "Hearts") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::ROYAL_FLUSH);
    EXPECT_EQ(result.highCards[0], 14);
}

TEST(HandEvaluatorTest, StraightFlush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("9", "Clubs"), Card("T", "Clubs") };
    cardVec communityCards = { Card("J", "Clubs"), Card("Q", "Clubs"), Card("K", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::STRAIGHT_FLUSH);
    EXPECT_EQ(result.highCards[0], 13);
}

TEST(HandEvaluatorTest, QuadsOverQuads)
{
    HandEvaluator evaluator;
    cardVec hand1 = { Card("9", "Clubs"), Card("9", "Diamonds") };
    cardVec hand2 = { Card("10", "Clubs"), Card("10", "Diamonds") };
    cardVec communityCards = { Card("9", "Hearts"), Card("9", "Spades"), Card("10", "Hearts"), Card("10", "Spades") };

    auto result1 = evaluator.evaluateHand(hand1, communityCards);
    auto result2 = evaluator.evaluateHand(hand2, communityCards);

    EXPECT_EQ(result1 == result2, false);
    EXPECT_EQ(result1 > result2, false);
}

TEST(HandEvaluatorTest, FourOfAKind)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("9", "Clubs"), Card("9", "Diamonds") };
    cardVec communityCards = { Card("9", "Hearts"), Card("9", "Spades"), Card("K", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);

    EXPECT_EQ(result.rank, HandEvaluator::FOUR_OF_A_KIND);
    EXPECT_EQ(result.identifier[0], 9);
    EXPECT_EQ(result.highCards[0], 13);
}

TEST(HandEvaluatorTest, FullHouse)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("T", "Clubs"), Card("T", "Diamonds") };
    cardVec communityCards = { Card("T", "Hearts"), Card("J", "Spades"), Card("J", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
    EXPECT_EQ(result.rank, HandEvaluator::FULL_HOUSE);
    EXPECT_EQ(result.identifier[0], 10);
    EXPECT_EQ(result.identifier[1], 11);
    EXPECT_EQ(result.highCards.size(), 0);
}

TEST(HandEvaluatorTest, Flush)
{
    HandEvaluator evaluator;
    cardVec hand = { Card("2", "Hearts"), Card("5", "Hearts") };
    cardVec communityCards = { Card("9", "Hearts"), Card("J", "Hearts"), Card("K", "Hearts") };

    auto result = evaluator.evaluateHand(hand, communityCards);
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
    cardVec communityCards = { Card("4", "Hearts"), Card("5", "Spades"), Card("6", "Clubs") };

    auto result = evaluator.evaluateHand(hand, communityCards);
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
