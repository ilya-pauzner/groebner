#ifndef GROEBNER_ALGORITHM_H
#define GROEBNER_ALGORITHM_H

#include "polynomial.h"
#include "helpers.h"

namespace Groebner {
    template <typename FieldElement, typename OrderType>
    bool tryReduce(const Polynomial<FieldElement, OrderType>& f, Polynomial<FieldElement, OrderType>* g) {
        auto leadingTerm = f.leadingTerm();
        for (const auto& term : *g) {
            if (term.first.isDivisibleBy(leadingTerm.first)) {
                Monomial monomialQuotient = term.first / leadingTerm.first;
                Polynomial<FieldElement, OrderType> quotient({{monomialQuotient, term.second / leadingTerm.second}});
                *g -= quotient * f;
                return true;
            }
        }
        return false;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceAsPossible(const Polynomial<FieldElement, OrderType>& f, Polynomial<FieldElement, OrderType>* g) {
        size_t reductionsMade = 0;
        while (tryReduce(f, g)) {
            ++reductionsMade;
        }
        return reductionsMade > 0;
    }

    template <typename FieldElement, typename OrderType>
    Polynomial<FieldElement, OrderType> S_Polynomial(const Polynomial<FieldElement, OrderType>& f1, const Polynomial<FieldElement, OrderType>& f2) {
        auto leadingTerm1 = f1.leadingTerm();
        auto leadingTerm2 = f2.leadingTerm();
        Monomial lcm12 = lcm(leadingTerm1.first, leadingTerm2.first);
        Monomial m1 = lcm12 / leadingTerm1.first;
        Monomial m2 = lcm12 / leadingTerm2.first;
        return Polynomial<FieldElement, OrderType>({{m1, leadingTerm2.second}}) * f1 - Polynomial<FieldElement, OrderType>({{m2, leadingTerm1.second}}) * f2;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceOverSet(const std::vector<Polynomial<FieldElement, OrderType>>& set, Polynomial<FieldElement, OrderType>* g) {
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

    template <typename FieldElement, typename OrderType>
    void ReduceSetOverItself(std::vector<Polynomial<FieldElement, OrderType>>* set) {
        while (true) {
            bool reductions = false;
            for (auto& polynomial : *set) {
                auto new_set = *set;
                new_set.erase(std::find(new_set.begin(), new_set.end(), polynomial));
                reductions |= ReduceOverSet(new_set, &polynomial);
                if (polynomial == Polynomial<FieldElement, OrderType>()) {
                    *set = new_set;
                    break;
                }
            }
            if (!reductions) {
                break;
            }
        }
    }

    template <typename FieldElement, typename OrderType>
    void LeadingTermToOne(std::vector<Polynomial<FieldElement, OrderType>>* set) {
        for (auto& polynomial : *set) {
            polynomial /= polynomial.leadingTerm().second;
        }
    }

    template <typename FieldElement, typename OrderType>
    void DoBuhberger(std::vector<Polynomial<FieldElement, OrderType>>* set) {
        LeadingTermToOne(set);
        ReduceSetOverItself(set);
        while (true) {
            std::vector<Polynomial<FieldElement, OrderType>> newbies;
            for (size_t polynomialIndex1 = 0; polynomialIndex1 < set->size(); ++polynomialIndex1) {
                for (size_t polynomialIndex2 = polynomialIndex1 + 1; polynomialIndex2 < set->size(); ++polynomialIndex2) {
                    auto& p = (*set)[polynomialIndex1];
                    auto& q = (*set)[polynomialIndex2];
                    if (lcm(p.leadingTerm().first, q.leadingTerm().first) != p.leadingTerm().first * q.leadingTerm().first) {
                        auto S = S_Polynomial((*set)[polynomialIndex1], (*set)[polynomialIndex2]);
                        ReduceOverSet(*set, &S);
                        if (S != Polynomial<FieldElement, OrderType>()) {
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

    template <typename FieldElement, typename OrderType>
    bool LaysInIdeal(std::vector<Polynomial<FieldElement, OrderType>> ideal, Polynomial<FieldElement, OrderType> p) {
        ReduceSetOverItself(&ideal);
        ReduceOverSet(ideal, &p);
        return p == Polynomial<FieldElement, OrderType>();
    }

    template <typename FieldElement, typename OrderType>
    bool LaysInRadical(std::vector<Polynomial<FieldElement, OrderType>> ideal, Polynomial<FieldElement, OrderType> p) {
        size_t maxVariableNumber = 0;
        for (auto& polynomial : ideal) {
            for (auto& term : polynomial) {
                maxVariableNumber = std::max(maxVariableNumber, term.first.greatestVariableIndex());
            }
        }
        Monomial::DegreeContainer degrees(maxVariableNumber);
        degrees.push_back(1);
        Polynomial<FieldElement, OrderType> z({{Monomial(degrees), 1}});
        Polynomial<FieldElement, OrderType> one({{Monomial(), 1}});
        ideal.push_back(p * z - one);
        return LaysInIdeal(ideal, one);
    }
}

#endif //GROEBNER_ALGORITHM_H