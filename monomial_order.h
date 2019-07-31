#ifndef GROEBNER_MONOMIAL_ORDER_H
#define GROEBNER_MONOMIAL_ORDER_H

#include <functional>
#include "monomial.h"

namespace Groebner {
    class MonomialOrder {
     public:
        using MonomialComparator = std::function<bool(const Monomial&, const Monomial&)>;
        explicit MonomialOrder(MonomialComparator cmp) : compareFunction(std::move(cmp)) {}
        bool isLess(const Monomial& lhs, const Monomial& rhs) const { return compareFunction(lhs, rhs); }
     private:
         MonomialComparator compareFunction;
    };

    bool lexCompare(const Monomial&, const Monomial&);
    static MonomialOrder lexOrder(lexCompare);

    bool degreeCompare(const Monomial&, const Monomial&);
    static MonomialOrder degreeOrder(degreeCompare);

    MonomialOrder combine(const MonomialOrder&, const MonomialOrder&);

    static MonomialOrder degreeLexOrder = combine(degreeOrder, lexOrder);

}

#endif //GROEBNER_MONOMIAL_ORDER_H
