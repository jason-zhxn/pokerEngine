#include "PokerGame.h"
#include <iostream>

int main()
{
    Deck deck;
    deck.shuffle();
    Card card = deck.popTop();
    std::cout << card.toString() << std::endl;
    PokerGame pokergame;
    pokergame.playGame();
    return 0;
}
