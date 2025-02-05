#pragma once

#include <string>

class Card
{
  public:
    Card(const char &rank, const char &suit);
    char getRank() const;
    char getSuit() const;
    int getValue() const;
    std::string toString() const;

    bool operator==(const Card &other) const;
    bool operator!=(const Card &other) const;

  private:
    char rank;
    char suit;
};
