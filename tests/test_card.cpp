#include "Card.h"
#include <gtest/gtest.h>

// Test Card creation
TEST(CardTest, ValidCard)
{
    Card card("10", "Hearts");
    EXPECT_EQ(card.getRank(), "10");
    EXPECT_EQ(card.getSuit(), "Hearts");
    EXPECT_EQ(card.getValue(), 10);
}

// Test Ace rank
TEST(CardTest, AceValue)
{
    Card card("A", "Spades");
    EXPECT_EQ(card.getValue(), 14);
}

// Test King rank
TEST(CardTest, KingValue)
{
    Card card("K", "Diamonds");
    EXPECT_EQ(card.getValue(), 13);
}

// Test string representation
TEST(CardTest, ToString)
{
    Card card("Q", "Clubs");
    EXPECT_EQ(card.toString(), "Q of Clubs");
}

// Test equality operator
TEST(CardTest, EqualityOperator)
{
    Card card1("10", "Hearts");
    Card card2("10", "Hearts");
    EXPECT_TRUE(card1 == card2);
}

// Test inequality operator
TEST(CardTest, InequalityOperator)
{
    Card card1("10", "Hearts");
    Card card2("J", "Hearts");
    EXPECT_TRUE(card1 != card2);
}
