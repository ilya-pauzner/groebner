#include "monomial.h"

namespace Groebner {
    Monomial::Monomial(const DegreeContainer& container) : Degrees_(container) {}

    Monomial::Monomial(DegreeContainer&& container) : Degrees_(std::move(container)) {}

    Monomial::Monomial(std::initializer_list<Monomial::DegreeType> ilist) : Degrees_(ilist) {
        trimTrailingZeroes();
    }

    Monomial::DegreeType Monomial::degree(size_t variableIndex) const {
        if (variableIndex < Degrees_.size()) {
            return Degrees_[variableIndex];
        }
        return 0;
    }

    Monomial::DegreeType Monomial::totalDegree() const {
        return std::accumulate(Degrees_.begin(), Degrees_.end(), DegreeType(0));
    }

    size_t Monomial::greatestVariableIndex() const {
        return Degrees_.size();
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

    bool Monomial::isDivisibleBy(const Monomial& other) const {
        for (size_t variableIndex = 0; variableIndex < Degrees_.size() && variableIndex < other.Degrees_.size(); ++variableIndex) {
            if (other.Degrees_[variableIndex] > Degrees_[variableIndex]) {
                return false;
            }
        }

        return (Degrees_.size() >= other.Degrees_.size());
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        if (other.Degrees_.size() > Degrees_.size()) {
            throw std::runtime_error("Not divisible.");
        }

        for (size_t variableIndex = 0; variableIndex < other.Degrees_.size(); ++variableIndex) {
            if (Degrees_[variableIndex] < other.Degrees_[variableIndex]) {
                throw std::runtime_error("Not divisible.");
            }
            Degrees_[variableIndex] -= other.Degrees_[variableIndex];
        }
        trimTrailingZeroes();
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
        return !(lhs == rhs);
    }

    void Monomial::trimTrailingZeroes() {
        if (Degrees_.empty()) {
            return;
        }
        size_t nonZeroIndex = Degrees_.size() - 1;
        while (nonZeroIndex > 0 && Degrees_[nonZeroIndex] == 0) {
            --nonZeroIndex;
        }
        size_t newSize = nonZeroIndex;
        if (Degrees_[nonZeroIndex] != 0) {
            ++newSize;
        }
        Degrees_.resize(newSize);
    }

    std::ostream& operator<<(std::ostream& os, const Monomial& m) {
        for (size_t variableIndex = 0; variableIndex < m.Degrees_.size(); ++variableIndex) {
            if (m.Degrees_[variableIndex] > 1) {
                os << "(" << char('a' + variableIndex) << "^" << m.Degrees_[variableIndex] << ")";
            } else if (m.Degrees_[variableIndex] == 1) {
                os << char('a' + variableIndex);
            }
        }
        return os;
    }
}