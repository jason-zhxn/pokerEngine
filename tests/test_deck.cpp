#include "Deck.h"
#include <gtest/gtest.h>
#include <set>

// Test Deck creation
TEST(DeckTest, CreateDeck)
{
    Deck deck;
    EXPECT_EQ(deck.getLength(), 52);
}

// Test popTop()
TEST(DeckTest, PopTop)
{
    Deck deck;
    Card card = deck.popTop();
    EXPECT_EQ(typeid(card), typeid(card));
    EXPECT_EQ(deck.getLength(), 51);
}
