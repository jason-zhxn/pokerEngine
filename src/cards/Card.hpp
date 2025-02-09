#pragma once

#include <string>

class Card
{
  public:
    Card(const std::string &rank, const std::string &suit);
    std::string getRank() const;
    std::string getSuit() const;
    int getValue() const;
    std::string toString() const;
    int getHash() const;

    bool operator==(const Card &other) const;
    bool operator!=(const Card &other) const;

  private:
    std::string rank;
    std::string suitStr;
};
