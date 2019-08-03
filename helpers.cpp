#include "helpers.h"

namespace Groebner {
    Monomial lcm(const Monomial& f, const Monomial& g) {
        size_t resultVariablesCount = std::max(f.variablesCount(), g.variablesCount());
        Monomial::DegreeContainer result(resultVariablesCount);
        for (size_t variableIndex = 0; variableIndex < resultVariablesCount; ++variableIndex) {
            result[variableIndex] = std::max(f.degree(variableIndex), g.degree(variableIndex));
        }
        return Monomial(result);
    }
}