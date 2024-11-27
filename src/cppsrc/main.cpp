#include "cards/Card.h"
#include "game/HandEvaluator.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

int main()
{
    Card card1("10", "Hearts");
    Card card2("J", "Hearts");
    Card card3("Q", "Hearts");
    Card card4("K", "Hearts");
    Card card5("A", "Hearts");

    HandEvaluator evaluator;

    try {
        HandEvaluator::HandResult result = evaluator.evaluateHand({ card1, card2, card3, card4, card5 });

        // Print the result
        std::cout << "Hand rank: " << result.rank << std::endl;
        std::cout << "High cards: ";
        for (int highCard : result.highCards) { std::cout << highCard << " "; }
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

int add(int a, int b)
{
    return a + b;
}

PYBIND11_MODULE(poker, m)
{
    m.def("add", &add, "Adds two numbers");
}