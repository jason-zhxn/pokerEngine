#pragma once

#include "Agent.hpp"

class Player : public Agent
{
  public:
    Player(const std::string &name, double initialChips);
};
