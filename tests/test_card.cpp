#include "Card.hpp"
#include <gtest/gtest.h>

TEST(CardTest, ValidCard)
{
    Card card("10", "Hearts");
    EXPECT_EQ(card.getRank(), "10");
    EXPECT_EQ(card.getSuit(), "Hearts");
    EXPECT_EQ(card.getValue(), 10);
}

TEST(CardTest, AceValue)
{
    Card card("A", "Spades");
    EXPECT_EQ(card.getValue(), 14);
}

TEST(CardTest, KingValue)
{
    Card card("K", "Diamonds");
    EXPECT_EQ(card.getValue(), 13);
}

TEST(CardTest, ToString)
{
    Card card("Q", "Clubs");
    EXPECT_EQ(card.toString(), "Q of Clubs");
}

TEST(CardTest, EqualityOperator)
{
    Card card1("10", "Hearts");
    Card card2("10", "Hearts");
    EXPECT_TRUE(card1 == card2);
}

TEST(CardTest, InequalityOperator)
{
    Card card1("10", "Hearts");
    Card card2("J", "Hearts");
    EXPECT_TRUE(card1 != card2);
}
