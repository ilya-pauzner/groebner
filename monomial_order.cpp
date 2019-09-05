#include "monomial_order.h"

namespace Groebner {
    bool LexOrder::isLess(const Monomial& lhs, const Monomial& rhs) {
        if (lhs == rhs) {
            return false;
        }
        size_t nonEqualVariableIndex = 0;
        while (lhs.degree(nonEqualVariableIndex) == rhs.degree(nonEqualVariableIndex)) {
            ++nonEqualVariableIndex;
        }
        return lhs.degree(nonEqualVariableIndex) < rhs.degree(nonEqualVariableIndex);
    }

    bool RevLexOrder::isLess(const Monomial& lhs, const Monomial& rhs) {
        if (lhs == rhs) {
            return false;
        }
        size_t nonEqualVariableIndex = 0;
        while (lhs.degree(nonEqualVariableIndex) == rhs.degree(nonEqualVariableIndex)) {
            ++nonEqualVariableIndex;
        }
        return lhs.degree(nonEqualVariableIndex) > rhs.degree(nonEqualVariableIndex);
    }

    bool DegreeOrder::isLess(const Monomial& lhs, const Monomial& rhs) {
        return lhs.totalDegree() < rhs.totalDegree();
    }
}