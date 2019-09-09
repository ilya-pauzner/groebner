#include "helpers.h"

namespace Groebner {
    Monomial lcm(const Monomial& f, const Monomial& g) {
        size_t resultVariablesCount = std::max(f.greatestVariableIndex(), g.greatestVariableIndex());
        Monomial::DegreeContainer result(resultVariablesCount);
        for (size_t variableIndex = 0; variableIndex < resultVariablesCount; ++variableIndex) {
            result[variableIndex] = std::max(f.degree(variableIndex), g.degree(variableIndex));
        }
        return Monomial(result);
    }

    Timer::Timer(std::string _message_ = "") : message(std::move(_message_)), t0(clock()) {}
    Timer::~Timer() {
        std::cout << double(clock() - t0) / CLOCKS_PER_SEC << " elapsed for " << message << std::endl;
    }
}