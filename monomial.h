#ifndef GROEBNERBASIS_MONOMIAL_H
#define GROEBNERBASIS_MONOMIAL_H

#include <algorithm>
#include <iostream>
#include <vector>

namespace Groebner {
    class Monomial {
     public:
        using DegreeType = size_t;
        using DegreeContainer = std::vector<DegreeType>;

        Monomial() {}
        Monomial(std::initializer_list<DegreeType>);

        DegreeType degree(size_t) const;

        Monomial& operator*=(const Monomial& other);
        friend Monomial operator*(const Monomial&, const Monomial&);

        bool isDivisibleBy(const Monomial &other) const;
        Monomial&operator/=(const Monomial& other);
        friend Monomial operator/(const Monomial&, const Monomial&);

        friend bool operator==(const Monomial&, const Monomial&);
        friend bool operator!=(const Monomial&, const Monomial&);

        friend std::ostream& operator<<(std::ostream &os, const Monomial& m);
     private:
        DegreeContainer Degrees_;
        void trimTrailingZeroes();
    };
}

#endif //GROEBNERBASIS_MONOMIAL_H
