#ifndef GROEBNER_CYCLIC_H
#define GROEBNER_CYCLIC_H

#include "polynomial.h"

using Rational = boost::rational<long long>;

template <typename OrderType>
using RationalPolynomial = Groebner::Polynomial<Rational, OrderType>;

template <typename OrderType>
using RationalPolynomialSet = Groebner::PolynomialSet<Rational, OrderType>;

template <typename OrderType>
using RationalPolynomialVector = std::vector<Groebner::Polynomial<Rational, OrderType> >;

template <typename OrderType>
RationalPolynomialVector<OrderType> GeneratePowerFamily(size_t n) {
    RationalPolynomialVector<OrderType> powerFamily(n + 1);
    for (size_t variableDegree = 1; variableDegree < powerFamily.size(); ++variableDegree) {
        for (size_t variableIndex = 0; variableIndex < n; ++variableIndex) {
            powerFamily[variableDegree] += Groebner::Monomial::getNthVariable(variableIndex, variableDegree);
        }
    }
    return powerFamily;
}

template <typename OrderType>
RationalPolynomialVector<OrderType> GenerateSymmetricFamily(const RationalPolynomialVector<OrderType>& powerFamily) {
    RationalPolynomialVector<OrderType> symmetricFamily(powerFamily.size());
    symmetricFamily[0] = Rational(1);
    for (size_t k = 1; k < symmetricFamily.size(); ++k) {
        for (size_t i = 1; i <= k; ++i) {
            auto summand = symmetricFamily[k - i] * powerFamily[i];
            if (i % 2 == 1) {
                symmetricFamily[k] += summand;
            } else {
                symmetricFamily[k] -= summand;
            }
        }
        symmetricFamily[k] /= Rational(k);
    }
    return symmetricFamily;
}

template <typename OrderType>
RationalPolynomialSet<OrderType> GenerateCyclicFamily(size_t n) {
    auto powerFamily = GeneratePowerFamily<OrderType>(n);
    auto symmetricFamily = GenerateSymmetricFamily<OrderType>(powerFamily);
    if (n % 2 == 0) {
        symmetricFamily[n] += Rational(1);
    } else {
        symmetricFamily[n] -= Rational(1);
    }
    RationalPolynomialSet<OrderType> answer(symmetricFamily.begin() + 1, symmetricFamily.end());
    return answer;
}

using RationalPolynomialLex = RationalPolynomial<Groebner::LexOrder>;
using RationalPolynomialDegRevLex = RationalPolynomial<Groebner::DegreeRevLexOrder>;

#endif //GROEBNER_CYCLIC_H
