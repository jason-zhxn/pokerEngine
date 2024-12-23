#include "Player.hpp"
#include <gtest/gtest.h>

TEST(PlayerTest, PlayerInitiation)
{
    Player player("Harry", 100);
    EXPECT_EQ(player.getChips(), 100);
    EXPECT_EQ(player.getName(), "Harry");
    EXPECT_EQ(player.isActive(), true);
}

TEST(PlayerTest, PlayerModifiers)
{
    Player player("Harry", 100);
    player.addChips(10);
    EXPECT_EQ(player.getChips(), 110);
    player.deductChips(100);
    EXPECT_EQ(player.getChips(), 10);
    player.fold();
    EXPECT_EQ(player.isActive(), false);
}

TEST(PlayerTest, PlayerHand)
{
    Player player("Harry", 100);

    std::vector<Card> hand = { Card("J", "Spades"), Card("K", "Hearts") };

    player.setHand(hand);

    ASSERT_EQ(player.getHand().size(), 2);
    EXPECT_EQ(player.getHand()[0].toString(), "J of Spades");
    EXPECT_EQ(player.getHand()[1].toString(), "K of Hearts");
}
