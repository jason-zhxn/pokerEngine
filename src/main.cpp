#include "cards/Card.h"
#include "game/HandEvaluation.h"
#include <iostream>

int main()
{
    // Create some sample cards
    Card card1("Ace", "Hearts");
    Card card2("10", "Spades");

    // Print card information
    std::cout << "Card 1: " << card1.toString() << std::endl;
    std::cout << "Card 2: " << card2.toString() << std::endl;

    // Example usage of HandEvaluation (placeholder)
    HandEvaluation evaluator;
    // Assume evaluateHand() is a method in HandEvaluation to evaluate cards
    std::string result = evaluator.evaluateHand({ card1, card2 });

    // Print hand evaluation result
    std::cout << "Hand Evaluation Result: " << result << std::endl;

    return 0;
}
