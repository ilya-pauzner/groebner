#ifndef GROEBNER_MONOMIAL_ORDER_H
#define GROEBNER_MONOMIAL_ORDER_H

#include <functional>
#include "monomial.h"

namespace Groebner {
    class LexOrder {
     public:
        static bool isLess(const Monomial& lhs, const Monomial& rhs);
    };

    class RevLexOrder {
     public:
        static bool isLess(const Monomial& lhs, const Monomial& rhs);
    };

    class DegreeOrder {
     public:
        static bool isLess(const Monomial& lhs, const Monomial& rhs);
    };

    template <class TOrder1, class TOrder2>
    class Sum {
     public:
        static bool isLess(const Monomial& first, const Monomial& second) {
            if (TOrder1::isLess(first, second))
                return true;
            if (TOrder1::isLess(second, first))
                return false;
            return TOrder2::isLess(first, second);
        }
    };

    using DegreeLexOrder = Sum<DegreeOrder, LexOrder>;
    using DegreeRevLexOrder = Sum<DegreeOrder, RevLexOrder>;

}

#endif //GROEBNER_MONOMIAL_ORDER_H
