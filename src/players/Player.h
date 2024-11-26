#pragma once

#include "Card.h"
#include <string>
#include <vector>

class Player
{
  public:
    Player(const std::string &name, int initialChips);

    int getChips() const;
    bool isActive() const;

    void addChips(int amount);
    void deductChips(int amount);
    void fold();
    void resetForNextRound();
    void setHand(const std::vector<Card> &newHand);

    void displayStatus() const;

  private:
    std::string name;
    bool bot;
    int chips;
    bool active;
    std::vector<std::Card> hand;
};
