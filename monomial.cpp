#include <assert.h>
#include "monomial.h"

namespace Groebner {
    Monomial& Monomial::operator*=(const Monomial& other) {
        (*this).resize(std::max((*this).size(), other.size()));
        for (size_t variableIndex = 0; variableIndex < other.size(); ++variableIndex) {
            (*this)[variableIndex] += other[variableIndex];

        }
        return *this;
    }

    Monomial Monomial::operator*(const Monomial& other) const {
        Monomial ret(*this);
        ret *= other;
        return ret;
    }

    bool Monomial::DivisibleBy(const Monomial& other) const {
        for (size_t variableIndex = 0; variableIndex < (*this).size() && variableIndex < other.size(); ++variableIndex) {
            if (other[variableIndex] > (*this)[variableIndex]) {
                return false;
            }
        }
        // checking tail is zero
        for (size_t variableIndex = (*this).size(); variableIndex < other.size(); ++variableIndex) {
            if (other[variableIndex] > 0) {
                return false;
            }
        }

        return true;
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        (*this).resize(std::max((*this).size(), other.size()));
        for (size_t variableIndex = 0; variableIndex < other.size(); ++variableIndex) {
            (*this)[variableIndex] -= other[variableIndex];
            assert((*this)[variableIndex] >= 0);
        }
        (*this).Trim();
        return *this;
    }

    Monomial Monomial::operator/(const Monomial& other) const {
        Monomial ret(*this);
        ret /= other;
        return ret;
    }

    void Monomial::Trim() {
        while (!(*this).empty() && !(*this).back()) {
            (*this).pop_back();
        }
    }

    std::ostream& operator<<(std::ostream &os, const Monomial& m) {
        for (size_t variableIndex = 0; variableIndex < m.size(); ++variableIndex) {
            if (m[variableIndex] > 1) {
                os << "(" << (char) ('a' + variableIndex) << "^" << m[variableIndex] << ")";
            } else if (m[variableIndex] == 1) {
                os << (char) ('a' + variableIndex);
            }
        }
        return os;
    }
}