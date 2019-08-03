#ifndef GROEBNER_ALGORITHM_H
#define GROEBNER_ALGORITHM_H

#include "polynomial.h"
#include "helpers.h"

namespace Groebner {
    template <typename FieldElement, MonomialOrder* Order>
    bool tryReduce(const Polynomial<FieldElement, Order>& f, Polynomial<FieldElement, Order>* g) {
        auto leadingTerm = f.crbegin();
        for (const auto& term : *g) {
            if (term.first.isDivisibleBy(leadingTerm->first)) {
                Monomial monomialQuotient = term.first / leadingTerm->first;
                Polynomial<FieldElement, Order> quotient({{monomialQuotient, term.second / leadingTerm->second}});
                *g -= quotient * f;
                return true;
            }
        }
        return false;
    }

    template <typename FieldElement, MonomialOrder* Order>
    bool ReduceAsPossible(const Polynomial<FieldElement, Order>& f, Polynomial<FieldElement, Order>* g) {
        size_t reductionsMade = 0;
        while (tryReduce(f, g)) {
            ++reductionsMade;
        }
        return reductionsMade > 0;
    }

    template <typename FieldElement, MonomialOrder* Order>
    Polynomial<FieldElement, Order> S_Polynomial(const Polynomial<FieldElement, Order>& f1, const Polynomial<FieldElement, Order>& f2) {
        auto leadingTerm1 = f1.crbegin();
        auto leadingTerm2 = f2.crbegin();
        Monomial lcm12 = lcm(leadingTerm1->first, leadingTerm2->first);
        Monomial m1 = lcm12 / leadingTerm1->first;
        Monomial m2 = lcm12 / leadingTerm2->first;
        return Polynomial<FieldElement, Order>({{m1, leadingTerm2->second}}) * f1 - Polynomial<FieldElement, Order>({{m2, leadingTerm1->second}}) * f2;
    }

    template <typename FieldElement, MonomialOrder* Order>
    bool ReduceOverSet(const std::vector<Polynomial<FieldElement, Order>>& set, Polynomial<FieldElement, Order>* g) {
        size_t totalReductions = 0;
        while (true) {
            size_t successfulReductions = 0;
            for (const auto& f : set) {
                successfulReductions += ReduceAsPossible(f, g);
            }
            totalReductions += successfulReductions;
            if (successfulReductions == 0) {
                break;
            }
        }
        return totalReductions > 0;
    }

    template <typename FieldElement, MonomialOrder* Order>
    void DoBuhberger(std::vector<Polynomial<FieldElement, Order>>* set) {
        while (true) {
            std::vector<Polynomial<FieldElement, Order>> newbies;
            for (size_t polynomialIndex1 = 0; polynomialIndex1 < set->size(); ++polynomialIndex1) {
                for (size_t polynomialIndex2 = polynomialIndex1 + 1; polynomialIndex2 < set->size(); ++polynomialIndex2) {
                    auto S = S_Polynomial((*set)[polynomialIndex1], (*set)[polynomialIndex2]);
                    ReduceOverSet(*set, &S);
                }
            }
            if (newbies.empty()) {
                return;
            }
            for (auto polynomial : newbies) {
                (*set).emplace_back(std::move(polynomial));
            }
        }
    }
}

#endif //GROEBNER_ALGORITHM_H
