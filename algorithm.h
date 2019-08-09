#ifndef GROEBNER_ALGORITHM_H
#define GROEBNER_ALGORITHM_H

#include "polynomial.h"
#include "helpers.h"

namespace Groebner {
    template <typename FieldElement, MonomialOrder* Order>
    bool tryReduce(const Polynomial<FieldElement, Order>& f, Polynomial<FieldElement, Order>* g) {
        auto leadingTerm = f.leadingTerm();
        for (const auto& term : *g) {
            if (term.first.isDivisibleBy(leadingTerm.first)) {
                Monomial monomialQuotient = term.first / leadingTerm.first;
                Polynomial<FieldElement, Order> quotient({{monomialQuotient, term.second / leadingTerm.second}});
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
        auto leadingTerm1 = f1.leadingTerm();
        auto leadingTerm2 = f2.leadingTerm();
        Monomial lcm12 = lcm(leadingTerm1.first, leadingTerm2.first);
        Monomial m1 = lcm12 / leadingTerm1.first;
        Monomial m2 = lcm12 / leadingTerm2.first;
        return Polynomial<FieldElement, Order>({{m1, leadingTerm2.second}}) * f1 - Polynomial<FieldElement, Order>({{m2, leadingTerm1.second}}) * f2;
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
    void ReduceSetOverItself(std::vector<Polynomial<FieldElement, Order>>* set) {
        while (true) {
            bool reductions = false;
            for (auto& polynomial : *set) {
                auto new_set = *set;
                new_set.erase(std::find(new_set.begin(), new_set.end(), polynomial));
                reductions |= ReduceOverSet(new_set, &polynomial);
                if (polynomial == Polynomial<FieldElement, Order>()) {
                    *set = new_set;
                    break;
                }
            }
            if (!reductions) {
                break;
            }
        }
    }

    template <typename FieldElement, MonomialOrder* Order>
    void LeadingTermToOne(std::vector<Polynomial<FieldElement, Order>>* set) {
        for (auto& polynomial : *set) {
            polynomial /= polynomial.leadingTerm().second;
        }
    }

    template <typename FieldElement, MonomialOrder* Order>
    void DoBuhberger(std::vector<Polynomial<FieldElement, Order>>* set) {
        LeadingTermToOne(set);
        ReduceSetOverItself(set);
        while (true) {
            std::vector<Polynomial<FieldElement, Order>> newbies;
            for (size_t polynomialIndex1 = 0; polynomialIndex1 < set->size(); ++polynomialIndex1) {
                for (size_t polynomialIndex2 = polynomialIndex1 + 1; polynomialIndex2 < set->size(); ++polynomialIndex2) {
                    auto& p = (*set)[polynomialIndex1];
                    auto& q = (*set)[polynomialIndex2];
                    if (lcm(p.leadingTerm().first, q.leadingTerm().first) != p.leadingTerm().first * q.leadingTerm().first) {
                        auto S = S_Polynomial((*set)[polynomialIndex1], (*set)[polynomialIndex2]);
                        ReduceOverSet(*set, &S);
                        if (S != Polynomial<FieldElement, Order>()) {
                            newbies.emplace_back(std::move(S));
                        }
                    }
                }
            }
            if (newbies.empty()) {
                break;
            }
            LeadingTermToOne(&newbies);
            for (auto& polynomial : newbies) {
                if (std::find(set->begin(), set->end(), polynomial) == set->end()) {
                    (*set).emplace_back(std::move(polynomial));
                }
            }
        }
        ReduceSetOverItself(set);
    }
}

#endif //GROEBNER_ALGORITHM_H