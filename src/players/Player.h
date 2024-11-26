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
    std::vector<Card> getHand() const;
    std::string getName() const;

    void addChips(int amount);
    void deductChips(int amount);
    void fold();
    void reset();
    void setHand(const std::vector<Card> &newHand);

  private:
    std::string name;
    bool bot;
    int chips;
    bool active;
    std::vector<Card> hand;
};
