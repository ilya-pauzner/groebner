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

        Monomial m16({2, 0, 7});
        if (m16.degree(0) != 2 || m16.degree(1) != 0 || m16.degree(2) != 7 || m16.degree(1000) != 0) {
            throw std::runtime_error("Degree function works incorrectly");
        }
        if (m16.totalDegree() != 9) {
            throw std::runtime_error("Wrong total degree calculation");
        }
    }

    void test_monomial_order() {
        Monomial A({2, 3, 4});
        Monomial B({2, 3, 4, 5});
        Monomial C({5});
        Monomial D({3, 6});
        // A < B
        // D < C
        if (!LexOrder::isLess(A, B) || !LexOrder::isLess(D, C)) {
            throw std::runtime_error("Wrong lexicographical compare");
        }

        // A < B
        // A = D
        if (!DegreeOrder::isLess(A, B) || DegreeOrder::isLess(A, D) || DegreeOrder::isLess(D, A)) {
            throw std::runtime_error("Wrong degree compare");
        }

        // A < D
        if (!DegreeLexOrder::isLess(A, D)) {
            throw std::runtime_error("Wrong degree-lexicographical compare");
        }
    }

    void test_polynomials() {
        Polynomial<int, DegreeLexOrder> polyA({{Monomial({1, 2}), 2}, {{Monomial({0, 1, 2})}, 3}});
        Polynomial<int, DegreeLexOrder> polyB({{Monomial({1, 2}), 2}, {{Monomial({0, 1, 2})}, -3}});

        auto polyAplusB = polyA + polyB;
        auto polyAplusBexplicit = Polynomial<int, DegreeLexOrder>({{Monomial({1, 2}), 4}});
        if (polyAplusB != polyAplusBexplicit) {
            throw std::runtime_error("Polynomial addition does not work as intended");
        }

        auto polyAminusB = polyA - polyB;
        auto polyAminusBexplicit = Polynomial<int, DegreeLexOrder>({{Monomial({0, 1, 2}), 6}});
        if (polyAminusB != polyAminusBexplicit) {
            throw std::runtime_error("Polynomial substraction does not work as intended");
        }

        auto polyAxB = polyA * polyB;
        auto polyAxBexplicit = Polynomial<int, DegreeLexOrder>({{Monomial({2, 4}), 4}, {Monomial({0, 2, 4}), -9}});
        if (polyAxB != polyAxBexplicit) {
            throw std::runtime_error("Polynomial multiplication does not work as intended");
        }

        Polynomial<int, DegreeLexOrder> x({{Monomial({1}), 1}});
        if (x + x != x + Monomial({1})) {
            throw std::runtime_error("Addition with monomial does not work as intended");
        }
    }

    void test_all() {
        test_monomials();
        test_monomial_order();
        test_polynomials();
    }
}