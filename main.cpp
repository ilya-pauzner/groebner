#include "test.h"
#include <sstream>

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

int main() {
    Groebner::test_all();
    std::cout << std::string(80, '=') << std::endl;
    for (size_t i = 1; i <= 10; ++i) {
        auto family = GenerateCyclicFamily<Groebner::LexOrder>(i);
        std::ostringstream os;
        os << "Groebner for " << i << " calculation";
        {
            Groebner::Timer t(os.str());
            Groebner::DoBuhberger(&family);
        }
        for (const auto& elem : family) {
            std::cout << elem << std::endl;
        }
        std::cout << std::endl;
    }
}