#ifndef GROEBNERBASIS_MONOMIAL_H
#define GROEBNERBASIS_MONOMIAL_H

#include <vector>
#include <iostream>

namespace Groebner {
    using DegreeType = size_t;
    using IntegerVector = std::vector<DegreeType>;
    class Monomial {
     public:
        Monomial() = default;
        Monomial(const Monomial&) = default;
        Monomial(Monomial&&) = default;
        Monomial(std::initializer_list<size_t> ilist);

        Monomial& operator*=(const Monomial& other);
        friend Monomial operator*(const Monomial&, const Monomial&);

        bool isDivisibleBy(const Monomial &other) const;
        Monomial&operator/=(const Monomial& other);
        friend Monomial operator/(const Monomial&, const Monomial&);

        friend bool operator==(const Monomial&, const Monomial&);
        friend bool operator!=(const Monomial&, const Monomial&);

        friend std::ostream& operator<<(std::ostream &os, const Monomial& m);
     private:
        IntegerVector Degrees_;
        void trimTrailingZeroes();
    };
}

#endif //GROEBNERBASIS_MONOMIAL_H
