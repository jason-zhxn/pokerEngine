#pragma once

#include "Card.hpp"
#include <string>
#include <vector>

class Agent
{
  public:
    Agent(const std::string &name, double initialChips);

    virtual ~Agent() = default;

    std::string getName() const;
    double getChips() const;
    bool isActive() const;
    std::vector<Card> getHand() const;
    double getCurrentBet() const;

    void addChips(double amount);
    void deductChips(double amount);
    void fold();
    void reset();
    void setHand(const std::vector<Card> &newHand);
    void setCurrentBet(const double &amount);

  protected:
    std::string name;
    double chips;
    bool active;
    std::vector<Card> hand;
    double currentBet;
};
