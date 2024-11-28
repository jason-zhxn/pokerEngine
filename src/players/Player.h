#pragma once

#include "Card.h"
#include <string>
#include <vector>

class Player
{
  public:
    Player(const std::string &name, int initialChips, bool bot);

    int getChips() const;
    bool isActive() const;
    std::vector<Card> getHand() const;
    std::string getName() const;
    int getCurrentBet() const;
    bool isBot() const;

    void addChips(int amount);
    void deductChips(int amount);
    void fold();
    void reset();
    void setHand(const std::vector<Card> &newHand);
    void setCurrentBet(const int &amount);

  private:
    std::string name;
    bool bot;
    int chips;
    bool active;
    std::vector<Card> hand;
    int currentBet;
};
