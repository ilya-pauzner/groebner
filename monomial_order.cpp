#include "monomial_order.h"

namespace Groebner {
    bool lexCompare(const Monomial& lhs, const Monomial& rhs) {
        if (lhs == rhs) {
            return false;
        }
        size_t nonEqualVariableIndex = 0;
        while (lhs.degree(nonEqualVariableIndex) == rhs.degree(nonEqualVariableIndex)) {
            ++nonEqualVariableIndex;
        }
        return lhs.degree(nonEqualVariableIndex) < rhs.degree(nonEqualVariableIndex);
    }

    bool degreeCompare(const Monomial& lhs, const Monomial& rhs) {
        return lhs.totalDegree() < rhs.totalDegree();
    }

    MonomialOrder combine(const MonomialOrder& a, const MonomialOrder& b) {
        auto combination = [=](const Monomial& lhs, const Monomial& rhs) {
            if (a.isLess(lhs, rhs)) {
                return true;
            } else if (a.isLess(rhs, lhs)) {
                return false;
            } else {
                return b.isLess(lhs, rhs);
            }
        };
        return MonomialOrder(combination);
    }
}