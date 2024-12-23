#pragma once

#include "Agent.hpp"

class Bot : public Agent
{
  public:
    Bot(const std::string &name, double initialChips);

    void makeMove();
};
