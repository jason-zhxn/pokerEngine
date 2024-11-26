#include "HandEvaluator.h"
#include <gtest/gtest.h>

TEST(HandEvaluatorTest, RoyalFlush)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("10", "Hearts"), Card("J", "Hearts"), Card("Q", "Hearts"), Card("K", "Hearts"), Card("A", "Hearts")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::ROYAL_FLUSH);
    EXPECT_EQ(result.highCards[0], 14);
}

TEST(HandEvaluatorTest, StraightFlush)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("9", "Clubs"), Card("10", "Clubs"), Card("J", "Clubs"), Card("Q", "Clubs"), Card("K", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::STRAIGHT_FLUSH);
    EXPECT_EQ(result.highCards[0], 13);
}

TEST(HandEvaluatorTest, FourOfAKind)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("9", "Clubs"), Card("9", "Diamonds"), Card("9", "Hearts"), Card("9", "Spades"), Card("K", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::FOUR_OF_A_KIND);
}

TEST(HandEvaluatorTest, FullHouse)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("10", "Clubs"), Card("10", "Diamonds"), Card("10", "Hearts"), Card("J", "Spades"), Card("J", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::FULL_HOUSE);
}

TEST(HandEvaluatorTest, Flush)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("2", "Hearts"), Card("5", "Hearts"), Card("9", "Hearts"), Card("J", "Hearts"), Card("K", "Hearts")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::FLUSH);
}

TEST(HandEvaluatorTest, Straight)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("2", "Diamonds"), Card("3", "Clubs"), Card("4", "Hearts"), Card("5", "Spades"), Card("6", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::STRAIGHT);
    EXPECT_EQ(result.highCards[0], 6);
}

TEST(HandEvaluatorTest, ThreeOfAKind)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("10", "Clubs"), Card("10", "Diamonds"), Card("10", "Hearts"), Card("J", "Spades"), Card("K", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::THREE_OF_A_KIND);
}

TEST(HandEvaluatorTest, TwoPair)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("10", "Clubs"), Card("10", "Diamonds"), Card("J", "Hearts"), Card("J", "Spades"), Card("K", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::TWO_PAIR);
}

TEST(HandEvaluatorTest, OnePair)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("10", "Clubs"), Card("10", "Diamonds"), Card("J", "Hearts"), Card("Q", "Spades"), Card("K", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::ONE_PAIR);
}

TEST(HandEvaluatorTest, HighCard)
{
    HandEvaluator evaluator;
    std::vector<Card> hand = {
        Card("2", "Diamonds"), Card("5", "Clubs"), Card("7", "Hearts"), Card("9", "Spades"), Card("J", "Clubs")
    };

    auto result = evaluator.evaluateHand(hand);
    EXPECT_EQ(result.rank, HandEvaluator::HIGH_CARD);
    EXPECT_EQ(result.highCards[0], 11);
}
