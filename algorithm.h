#ifndef GROEBNER_ALGORITHM_H
#define GROEBNER_ALGORITHM_H

#include "polynomial.h"
#include "helpers.h"

namespace Groebner {
    template <typename FieldElement, typename OrderType>
    bool tryReduce(const Polynomial<FieldElement, OrderType>& f, Polynomial<FieldElement, OrderType>* g) {
        using Polynomial_ = Polynomial<FieldElement, OrderType>;
        const auto& leadingTerm = f.leadingTerm();
        auto divisibleBy = [&](const typename Polynomial_::Term& t) {
            return Polynomial_::getMonomial(t).isDivisibleBy(Polynomial_::getMonomial(leadingTerm));
        };
        auto divisibleTermPtr = std::find_if(g->begin(), g->end(), divisibleBy);
        if (divisibleTermPtr == g->end()) {
            return false;
        }

        Monomial monomialQuotient = Polynomial_::getMonomial(*divisibleTermPtr) / Polynomial_::getMonomial(leadingTerm);
        FieldElement coefficientQuotient = Polynomial_::getCoefficient(*divisibleTermPtr) / Polynomial_::getCoefficient(leadingTerm);
        Polynomial_ quotient({{monomialQuotient, coefficientQuotient}});
        *g -= quotient * f;
        return true;
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
    Polynomial<FieldElement, OrderType> S_Polynomial(const Polynomial<FieldElement, OrderType>& f1,
                                                     const Polynomial<FieldElement, OrderType>& f2) {
        using Polynomial_ = Polynomial<FieldElement, OrderType>;
        const auto& leadingTerm1 = f1.leadingTerm();
        const auto& leadingTerm2 = f2.leadingTerm();
        Monomial lcm12 = lcm(Polynomial_::getMonomial(leadingTerm1), Polynomial_::getMonomial(leadingTerm2));
        Monomial m1 = lcm12 / Polynomial_::getMonomial(leadingTerm1);
        Monomial m2 = lcm12 / Polynomial_::getMonomial(leadingTerm2);
        return f1 * m1 * Polynomial_::getCoefficient(leadingTerm2) - f2 * m2 * Polynomial_::getCoefficient(leadingTerm1);
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceOverSet(const PolynomialSet<FieldElement, OrderType>& set, Polynomial<FieldElement, OrderType>* g) {
        size_t totalReductions = 0;
        size_t iterationReductions = 0;
        do {
            iterationReductions = 0;
            for (const auto& f : set) {
                iterationReductions += ReduceAsPossible(f, g);
            }
            totalReductions += iterationReductions;
        } while (iterationReductions > 0);
        return totalReductions > 0;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceSetOverItself(PolynomialSet<FieldElement, OrderType>* set) {
        using Polynomial_ = Polynomial<FieldElement, OrderType>;
        size_t totalReductions = 0;
        size_t iterationReductions = 0;
        do {
            iterationReductions = 0;
            auto it = set->begin();
            while (it != set->end()) {
                auto it2 = (it++);
                Polynomial_ p = *it2;
                set->erase(p);
                iterationReductions += ReduceOverSet(*set, &p);
                if (p != Polynomial_()) {
                    set->insert(p);
                }
            }
            totalReductions += iterationReductions;
        } while (iterationReductions > 0);
        return totalReductions > 0;
    }

    template <typename FieldElement, typename OrderType>
    void LeadingTermToOne(PolynomialSet<FieldElement, OrderType>* set) {
        using Polynomial_ = Polynomial<FieldElement, OrderType>;
        PolynomialSet<FieldElement, OrderType> newSet;
        for (const auto& polynomial : *set) {
            newSet.insert(polynomial / Polynomial_::getCoefficient(polynomial.leadingTerm()));
        }
        *set = std::move(newSet);
    }

    template <typename FieldElement, typename OrderType>
    void DoBuhberger(PolynomialSet<FieldElement, OrderType>* set) {
        using Polynomial_ = Polynomial<FieldElement, OrderType>;
        LeadingTermToOne(set);
        ReduceSetOverItself(set);
        PolynomialSet<FieldElement, OrderType> newbies;
        do {
            newbies.clear();
            for (auto it1 = set->begin(); it1 != set->end(); ++it1) {
                for (auto it2 = it1; it2 != set->end(); ++it2) {
                    if (it1 != it2) {
                        auto& p = *it1;
                        auto& q = *it2;
                        if (lcm(Polynomial_::getMonomial(p.leadingTerm()), Polynomial_::getMonomial(q.leadingTerm())) !=
                            Polynomial_::getMonomial(p.leadingTerm()) * Polynomial_::getMonomial(q.leadingTerm())) {
                            auto S = S_Polynomial(p, q);
                            ReduceOverSet(*set, &S);
                            if (S != Polynomial_()) {
                                newbies.emplace(std::move(S));
                            }
                        }
                    }
                }
            }
            LeadingTermToOne(&newbies);
            for (auto& polynomial : newbies) {
                set->emplace(std::move(polynomial));
            }
        } while (!newbies.empty());
        ReduceSetOverItself(set);
    }

    template <typename FieldElement, typename OrderType>
    bool LaysInIdeal(PolynomialSet<FieldElement, OrderType> ideal, Polynomial<FieldElement, OrderType> p) {
        DoBuhberger(&ideal);
        ReduceOverSet(ideal, &p);
        return p == Polynomial<FieldElement, OrderType>();
    }

    template <typename FieldElement, typename OrderType>
    size_t getMaxVariableNumber(const Polynomial<FieldElement, OrderType>& p) {
        size_t maxVariableNumber = 0;
        for (const auto& term : p) {
            maxVariableNumber = std::max(maxVariableNumber, Polynomial<FieldElement, OrderType>::getMonomial(term).greatestVariableIndex());
        }
        return maxVariableNumber;
    }

    template <typename FieldElement, typename OrderType>
    bool LaysInRadical(PolynomialSet<FieldElement, OrderType> ideal, Polynomial<FieldElement, OrderType> p) {
        size_t maxVariableNumber = 0;
        for (const auto& polynomial : ideal) {
            maxVariableNumber = std::max(maxVariableNumber, getMaxVariableNumber(polynomial));
        }
        Monomial::DegreeContainer degrees(maxVariableNumber);
        degrees.push_back(1);
        Polynomial<FieldElement, OrderType> z({{Monomial(degrees), 1}});
        Polynomial<FieldElement, OrderType> one({{Monomial(), 1}});
        ideal.insert(p * z - one);
        return LaysInIdeal(ideal, one);
    }
}

#endif //GROEBNER_ALGORITHM_H