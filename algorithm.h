#ifndef GROEBNER_ALGORITHM_H
#define GROEBNER_ALGORITHM_H

#include "polynomial.h"
#include "helpers.h"

namespace Groebner {
    template <typename FieldElement, typename OrderType>
    bool TryReduceOnce(const Polynomial<FieldElement, OrderType>& f, Polynomial<FieldElement, OrderType>* g) {
        using Poly = Polynomial<FieldElement, OrderType>;
        const auto& leadingTerm = f.leadingTerm();
        auto divisibleBy = [&](const typename Poly::Term& t) {
            return Poly::getMonomial(t).isDivisibleBy(Poly::getMonomial(leadingTerm));
        };
        auto divisibleTermPtr = std::find_if(g->begin(), g->end(), divisibleBy);
        if (divisibleTermPtr == g->end()) {
            return false;
        }

        Monomial monomialQuotient = Poly::getMonomial(*divisibleTermPtr) / Poly::getMonomial(leadingTerm);
        FieldElement coefficientQuotient = Poly::getCoefficient(*divisibleTermPtr) / Poly::getCoefficient(leadingTerm);
        Poly quotient({{monomialQuotient, coefficientQuotient}});
        *g -= quotient * f;
        return true;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceWhilePossible(const Polynomial<FieldElement, OrderType>& f, Polynomial<FieldElement, OrderType>* g) {
        size_t reductionsMade = 0;
        while (TryReduceOnce(f, g)) {
            ++reductionsMade;
        }
        return reductionsMade > 0;
    }

    template <typename FieldElement, typename OrderType>
    Polynomial<FieldElement, OrderType> S_Polynomial(const Polynomial<FieldElement, OrderType>& f1,
                                                     const Polynomial<FieldElement, OrderType>& f2) {
        using Poly = Polynomial<FieldElement, OrderType>;
        const auto& leadingTerm1 = f1.leadingTerm();
        const auto& leadingTerm2 = f2.leadingTerm();
        Monomial lcm12 = lcm(Poly::getMonomial(leadingTerm1), Poly::getMonomial(leadingTerm2));
        Monomial m1 = lcm12 / Poly::getMonomial(leadingTerm1);
        Monomial m2 = lcm12 / Poly::getMonomial(leadingTerm2);
        return f1 * m1 * Poly::getCoefficient(leadingTerm2) - f2 * m2 * Poly::getCoefficient(leadingTerm1);
    }

    template <typename FieldElement, typename OrderType>
    bool TryReduceOverSetOnce(const PolynomialSet<FieldElement, OrderType>& set, Polynomial<FieldElement, OrderType>* g) {
        size_t reductionsMade = 0;
        for (const auto& f : set) {
            reductionsMade += ReduceWhilePossible(f, g);
        }
        return reductionsMade > 0;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceOverSetWhilePossible(const PolynomialSet<FieldElement, OrderType>& set, Polynomial<FieldElement, OrderType>* g) {
        size_t totalReductions = 0;
        size_t iterationReductions = 0;
        do {
            iterationReductions = TryReduceOverSetOnce(set, g);
            totalReductions += iterationReductions;
        } while (iterationReductions > 0);
        return totalReductions > 0;
    }

    template <typename FieldElement, typename OrderType>
    bool TryReduceSetOverItselfOnce(PolynomialSet<FieldElement, OrderType>* set) {
        using Poly = Polynomial<FieldElement, OrderType>;
        auto it = set->begin();
        size_t reductionsMade = 0;
        PolynomialSet<FieldElement, OrderType> reducedSet;
        while (it != set->end()) {
            Poly p = std::move(set->extract(it++).value());
            reductionsMade += ReduceOverSetWhilePossible(reducedSet, &p);
            reductionsMade += ReduceOverSetWhilePossible(*set, &p);
            if (p != FieldElement(0)) {
                reducedSet.insert(std::move(p));
            }
        }
        *set = std::move(reducedSet);
        return reductionsMade > 0;
    }

    template <typename FieldElement, typename OrderType>
    bool ReduceSetOverItselfWhilePossible(PolynomialSet<FieldElement, OrderType>* set) {
        using Poly = Polynomial<FieldElement, OrderType>;
        size_t totalReductions = 0;
        size_t iterationReductions = 0;
        do {
            iterationReductions = TryReduceSetOverItselfOnce(set);
            totalReductions += iterationReductions;
        } while (iterationReductions > 0);
        return totalReductions > 0;
    }

    template <typename FieldElement, typename OrderType>
    void LeadingTermToOne(PolynomialSet<FieldElement, OrderType>* set) {
        using Poly = Polynomial<FieldElement, OrderType>;
        PolynomialSet<FieldElement, OrderType> newSet;
        for (const auto& polynomial : *set) {
            newSet.insert(polynomial / Poly::getCoefficient(polynomial.leadingTerm()));
        }
        *set = std::move(newSet);
    }

    template <typename FieldElement, typename OrderType>
    bool AreLeadingTermsCoPrime(const Polynomial<FieldElement, OrderType>& f, const Polynomial<FieldElement, OrderType>& g) {
        using Poly = Polynomial<FieldElement, OrderType>;
        const auto& fTerm = Poly::getMonomial(f.leadingTerm());
        const auto& gTerm = Poly::getMonomial(g.leadingTerm());
        return fTerm * gTerm == lcm(fTerm, gTerm);
    }

    template <typename FieldElement, typename OrderType>
    void ProcessPair(const Polynomial<FieldElement, OrderType>& f,
                     const Polynomial<FieldElement, OrderType>& g,
                     const PolynomialSet<FieldElement, OrderType>& set,
                     PolynomialSet<FieldElement, OrderType>* newbies)
    {
        if (AreLeadingTermsCoPrime(f, g)) {
            return;
        }
        auto S = S_Polynomial(f, g);
        ReduceOverSetWhilePossible(set, &S);
        if (S == FieldElement(0)) {
            return;
        }
        newbies->insert(std::move(S));
    }

    template <typename FieldElement, typename OrderType>
    PolynomialSet<FieldElement, OrderType> GetReducedPairs(const PolynomialSet<FieldElement, OrderType>& set) {
        using Poly = Polynomial<FieldElement, OrderType>;
        PolynomialSet<FieldElement, OrderType> newbies;
        for (auto it1 = set.begin(); it1 != set.end(); ++it1) {
            for (auto it2 = set.begin(); it2 != it1; ++it2) {
                ProcessPair(*it1, *it2, set, &newbies);
            }
        }
        LeadingTermToOne(&newbies);
        return newbies;
    }

    template <typename FieldElement, typename OrderType>
    void DoBuhberger(PolynomialSet<FieldElement, OrderType>* set) {
        using Poly = Polynomial<FieldElement, OrderType>;
        ReduceSetOverItselfWhilePossible(set);
        LeadingTermToOne(set);
        PolynomialSet<FieldElement, OrderType> newbies = GetReducedPairs(*set);
        while (!newbies.empty()) {
            std::move(newbies.begin(), newbies.end(), std::inserter(*set, set->begin()));
            newbies = GetReducedPairs(*set);
        }
        ReduceSetOverItselfWhilePossible(set);
    }

    template <typename FieldElement, typename OrderType>
    bool LaysInIdeal(PolynomialSet<FieldElement, OrderType> ideal, Polynomial<FieldElement, OrderType> p) {
        DoBuhberger(&ideal);
        ReduceOverSetWhilePossible(ideal, &p);
        return p == FieldElement(0);
    }

    template <typename FieldElement, typename OrderType>
    size_t GetMaxVariableNumber(const Polynomial<FieldElement, OrderType>& p) {
        size_t maxVariableNumber = 0;
        for (const auto& term : p) {
            maxVariableNumber = std::max(maxVariableNumber, Polynomial<FieldElement, OrderType>::getMonomial(term).greatestVariableIndex());
        }
        return maxVariableNumber;
    }

    template <typename FieldElement, typename OrderType>
    Polynomial<FieldElement, OrderType> MakeNewVariable(
        const PolynomialSet<FieldElement, OrderType>& ideal,
        const Polynomial<FieldElement, OrderType>& p = FieldElement(0))
    {
        size_t maxVariableNumber = 0;
        for (const auto& polynomial : ideal) {
            maxVariableNumber = std::max(maxVariableNumber, GetMaxVariableNumber(polynomial));
        }
        maxVariableNumber = std::max(maxVariableNumber, GetMaxVariableNumber(p));
        return Polynomial<FieldElement, OrderType>(Monomial::getNthVariable(maxVariableNumber));
    }

    template <typename FieldElement, typename OrderType>
    bool LaysInRadical(PolynomialSet<FieldElement, OrderType> ideal, Polynomial<FieldElement, OrderType> p) {
        Polynomial<FieldElement, OrderType> one({{Monomial(), 1}});
        auto z = MakeNewVariable(ideal, p);
        ideal.insert(p * z - one);
        return LaysInIdeal(ideal, one);
    }
}

#endif //GROEBNER_ALGORITHM_H