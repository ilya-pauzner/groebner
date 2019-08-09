#include "test.h"

std::random_device rd;
std::mt19937 mt(rd());


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
        if (!lexOrder.isLess(A, B) || !lexOrder.isLess(D, C)) {
            throw std::runtime_error("Wrong lexicographical compare");
        }

        // A < B
        // A = D
        if (!degreeOrder.isLess(A, B) || degreeOrder.isLess(A, D) || degreeOrder.isLess(D, A)) {
            throw std::runtime_error("Wrong degree compare");
        }

        // A < D
        if (!degreeLexOrder.isLess(A, D)) {
            throw std::runtime_error("Wrong degree-lexicographical compare");
        }
    }

    void test_polynomials() {
        Polynomial<int, &degreeLexOrder> polyA({{Monomial({1, 2}), 2}, {{Monomial({0, 1, 2})}, 3}});
        Polynomial<int, &degreeLexOrder> polyB({{Monomial({1, 2}), 2}, {{Monomial({0, 1, 2})}, -3}});

        auto polyAplusB = polyA + polyB;
        auto polyAplusBexplicit = Polynomial<int, &degreeLexOrder>({{Monomial({1, 2}), 4}});
        if (polyAplusB != polyAplusBexplicit) {
            throw std::runtime_error("Polynomial addition does not work as intended");
        }

        auto polyAminusB = polyA - polyB;
        auto polyAminusBexplicit = Polynomial<int, &degreeLexOrder>({{Monomial({0, 1, 2}), 6}});
        if (polyAminusB != polyAminusBexplicit) {
            throw std::runtime_error("Polynomial substraction does not work as intended");
        }

        auto polyAxB = polyA * polyB;
        auto polyAxBexplicit = Polynomial<int, &degreeLexOrder>({{Monomial({2, 4}), 4}, {Monomial({0, 2, 4}), -9}});
        if (polyAxB != polyAxBexplicit) {
            throw std::runtime_error("Polynomial multiplication does not work as intended");
        }

        Polynomial<int, &degreeLexOrder> x({{Monomial({1}), 1}});
        if (x + x != x + Monomial({1})) {
            throw std::runtime_error("Addition with monomial does not work as intended");
        }

        Polynomial<boost::rational<long long>, &degreeOrder> y({{Monomial({1}), 1}});
    }

    Monomial random_monomial() {
        Monomial::DegreeContainer v;
        for (size_t i = 0; i < 3; ++i) {
            v.push_back(mt() % 5);
        }
        return Monomial(v);
    }

    Polynomial<boost::rational<long long>, &lexOrder> random_polynomial() {
        Polynomial<boost::rational<long long>, &lexOrder> res;
        for (size_t i = 0; i < 3; ++i) {
            res += Polynomial<boost::rational<long long>, &lexOrder>({{random_monomial(), mt() % 5}});
        }
        return res;
    }

    void test_algorithm() {
        Polynomial<boost::rational<long long>, &lexOrder> a({{Monomial({1}), 1}});
        Polynomial<boost::rational<long long>, &lexOrder> b({{Monomial({0, 1}), 1}});
        Polynomial<boost::rational<long long>, &lexOrder> c({{Monomial({0, 0, 1}), 1}});
        Polynomial<boost::rational<long long>, &lexOrder> one({{Monomial(), 1}});
        auto f11 = a * a - one;
        auto f12 = (a - one) * b;
        auto f13 = (a + one) * c;
        std::vector<typeof(f11)> polys1({f11, f12, f13});
        DoBuhberger(&polys1);
        for (const auto& elem : polys1) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;

        auto f21 = f11;
        auto f22 = f12;
        auto f23 = (a - one) * c;
        std::vector<typeof(f11)> polys2({f21, f22, f23});
        DoBuhberger(&polys2);
        for (const auto& elem : polys2) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;


        auto f31 = a * a * a * b * c - a * c * c;
        auto f32 = a * b * b * c - a * b * c;
        auto f33 = a * a * b * b - c;
        std::vector<typeof(f11)> polys3({f31, f32, f33});
        DoBuhberger(&polys3);
        for (const auto& elem : polys3) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;

        auto f41 = a * a * b + a * c + b * b * c;
        auto f42 = a * c * c - b * c;
        auto f43 = a * b * c - b * b;
        std::vector<typeof(f11)> polys4({f41, f42, f43});
        DoBuhberger(&polys4);
        for (const auto& elem : polys4) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;

        auto f51 = a * b * b - c - c * c;
        auto f52 = a * a * b - b;
        auto f53 = b * b - c * c;
        std::vector<typeof(f11)> polys5({f51, f52, f53});
        DoBuhberger(&polys5);
        for (const auto& elem : polys5) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;

        auto f61 = a * b + a * a * c;
        auto f62 = a * c + b * c * c * c;
        auto f63 = b * c - b * b * c * c * c;
        std::vector<typeof(f11)> polys6({f61, f62, f63});
        DoBuhberger(&polys6);
        for (const auto& elem : polys6) {
            std::cout << elem << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;
    }

    void test_all() {
        test_monomials();
        test_monomial_order();
        test_polynomials();
        test_algorithm();
    }
}