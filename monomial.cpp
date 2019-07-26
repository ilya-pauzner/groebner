#include "monomial.h"

namespace Groebner {
    Monomial::Monomial(std::initializer_list<size_t> ilist) : Degrees_(ilist) {
        trimTrailingZeroes();
        for (auto variableDegree : Degrees_) {
            if (variableDegree < 0) {
                throw std::runtime_error("Negative powers are not allowed.");
            }
        }
    }

    Monomial& Monomial::operator*=(const Monomial& other) {
        Degrees_.resize(std::max(Degrees_.size(), other.Degrees_.size()));
        for (size_t variableIndex = 0; variableIndex < other.Degrees_.size(); ++variableIndex) {
            Degrees_[variableIndex] += other.Degrees_[variableIndex];
        }
        return *this;
    }

    Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
        Monomial ret(lhs);
        ret *= rhs;
        return ret;
    }

    bool Monomial::isDivisibleBy(const Monomial &other) const {
        for (size_t variableIndex = 0; variableIndex < Degrees_.size() && variableIndex < other.Degrees_.size(); ++variableIndex) {
            if (other.Degrees_[variableIndex] > Degrees_[variableIndex]) {
                return false;
            }
        }

        return (Degrees_.size() >= other.Degrees_.size());
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        Degrees_.resize(std::max(Degrees_.size(), other.Degrees_.size()));
        for (size_t variableIndex = 0; variableIndex < other.Degrees_.size(); ++variableIndex) {
            if (Degrees_[variableIndex] < other.Degrees_[variableIndex]) {
                throw std::runtime_error("Not divisible.");
            }
            Degrees_[variableIndex] -= other.Degrees_[variableIndex];
        }
        this->trimTrailingZeroes();
        return *this;
    }

    Monomial operator/(const Monomial& lhs, const Monomial& rhs) {
        Monomial ret(lhs);
        ret /= rhs;
        return ret;
    }

    bool operator==(const Monomial& lhs, const Monomial& rhs) {
        return lhs.Degrees_ == rhs.Degrees_;
    }

    bool operator!=(const Monomial& lhs, const Monomial& rhs) {
        return lhs.Degrees_ != rhs.Degrees_;
    }

    void Monomial::trimTrailingZeroes() {
        int nonZeroIndex = int(Degrees_.size()) - 1;
        while (nonZeroIndex > -1 && Degrees_[nonZeroIndex] == 0) {
            --nonZeroIndex;
        }
        Degrees_.resize(size_t(nonZeroIndex + 1));
    }

    std::ostream& operator<<(std::ostream &os, const Monomial& m) {
        for (size_t variableIndex = 0; variableIndex < m.Degrees_.size(); ++variableIndex) {
            if (m.Degrees_[variableIndex] > 1) {
                os << "(" << (char) ('a' + variableIndex) << "^" << m.Degrees_[variableIndex] << ")";
            } else if (m.Degrees_[variableIndex] == 1) {
                os << (char) ('a' + variableIndex);
            }
        }
        return os;
    }
}