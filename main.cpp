#include <iostream>
#include "monomial.h"

using Monomial = Groebner::Monomial;

int main() {
    Monomial m1({2, 3, 1});
    Monomial m2({1, 2, 0});
    if (m1.DivisibleBy(m2)) {
        std::cout << "m1 / m2 = " << m1 / m2 << "\n";
    } else if (m2.DivisibleBy(m1)) {
        std::cout << "m2 / m1 = " << m2 / m1 << "\n";
    } else {
        std::cout << "Not divisible" << "\n";
    }
}