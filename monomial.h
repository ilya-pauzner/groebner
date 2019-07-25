#ifndef GROEBNERBASIS_MONOMIAL_H
#define GROEBNERBASIS_MONOMIAL_H

#include <vector>
#include <iostream>

namespace Groebner {
    using IntegerVector = std::vector<int>;
    class Monomial : public IntegerVector {
        using IntegerVector::IntegerVector;
     public:
        Monomial& operator*=(const Monomial& other);
        Monomial operator*(const Monomial& other) const;

        bool DivisibleBy(const Monomial& other) const;
        Monomial&operator/=(const Monomial& other);
        Monomial operator/(const Monomial& other) const;

        friend std::ostream& operator<<(std::ostream &os, const Monomial& m);
     private:
        void Trim();
    };
}

#endif //GROEBNERBASIS_MONOMIAL_H
