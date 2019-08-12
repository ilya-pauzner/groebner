#ifndef GROEBNER_TEST_H
#define GROEBNER_TEST_H

#include "algorithm.h"
#include "helpers.h"
#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "boost/rational.hpp"
#include <random>

namespace Groebner {
    void test_monomials();
    void test_monomial_order();
    void test_polynomials();
    void test_algorithm();

    Monomial random_monomial();
    Polynomial<boost::rational<long long>, LexOrder> random_polynomial();


    void test_all();
}

#endif //GROEBNER_TEST_H
