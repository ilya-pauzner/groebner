#include "test.h"

namespace Groebner {
    void test_monomials() {
        Monomial m1;
        Monomial m2;
        if (m1 != m2) {
            throw std::runtime_error("Empty monomials should be equal.");
        }

        Monomial m3({2, 3, 4, 0});
        Monomial m4({2, 3, 4});
        if (m3 != m4) {
            throw std::runtime_error("Zeroes at the end should be trimmed.");
        }

        Monomial m5 = std::move(m4);
        Monomial m6 = m3;
        if (m5 != m6 || m3 != m6) {
            throw std::runtime_error("Something was bad with copy/move constructors.");
        }

        Monomial m7({1, 2, 3, 4});
        Monomial m8 = m7 * m7;
        Monomial m9 = m8 / m7;
        if (m7 != m9) {
            throw std::runtime_error("Multiplication and division should give identity.");
        }

        Monomial m10({2, 1, 2});
        Monomial m11({2, 3, 2});
        Monomial m12({1, 0, 3});
        if (m11.isDivisibleBy(m10) && !m10.isDivisibleBy(m11)
            && !m12.isDivisibleBy(m11) && !m11.isDivisibleBy(m12)
            && !m12.isDivisibleBy(m10) && !m10.isDivisibleBy(m12)) {
            // Everything OK
        } else {
            throw std::runtime_error("Division test failed.");
        }

        Monomial m13({1, 2, 3});
        Monomial m14({2, 3});
        Monomial m15({3, 5, 3});
        if (m13 * m14 != m15) {
            throw std::runtime_error("Multiplication test failed.");
        }

        if (m13 * m14 != m14 * m13) {
            throw std::runtime_error("Multiplication should be commutative");
        }
    }

    void test_all() {
        test_monomials();
    }
}