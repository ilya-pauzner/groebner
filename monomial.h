#ifndef GROEBNERBASIS_MONOMIAL_H
#define GROEBNERBASIS_MONOMIAL_H

#include <iostream>
#include <numeric>
#include <vector>
#include <boost/functional/hash.hpp>

namespace Groebner {
    class Monomial {
     public:
        using DegreeType = size_t;
        using DegreeContainer = std::vector<DegreeType>;

        Monomial() = default;
        explicit Monomial(DegreeContainer);
        Monomial(std::initializer_list<DegreeType>);

        DegreeType degree(size_t) const;

        DegreeType totalDegree() const;

        size_t greatestVariableIndex() const;

        Monomial& operator*=(const Monomial& other);
        friend Monomial operator*(const Monomial&, const Monomial&);

        bool isDivisibleBy(const Monomial& other) const;
        Monomial& operator/=(const Monomial& other);
        friend Monomial operator/(const Monomial&, const Monomial&);

        friend std::size_t hash_value(const Monomial&);

        friend bool operator==(const Monomial&, const Monomial&);
        friend bool operator!=(const Monomial&, const Monomial&);

        friend std::ostream& operator<<(std::ostream& os, const Monomial& m);

        static Monomial getNthVariable(size_t variableIndex, size_t variableDegree = 1);
     private:
        DegreeContainer Degrees_;
        void trimTrailingZeroes();
    };
}

#endif //GROEBNERBASIS_MONOMIAL_H
