#ifndef GROEBNER_POLYNOMIAL_H
#define GROEBNER_POLYNOMIAL_H
#include "monomial_order.h"
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_set>
#include <boost/rational.hpp>

namespace Groebner {
    template <typename OrderType>
    class OrderAdaptor {
     public:
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return OrderType::isLess(lhs, rhs);
        }
    };

    template <typename FieldElement, typename OrderType>
    class Polynomial {
        using TermMap = std::map<Monomial, FieldElement, OrderAdaptor<OrderType>>;
     public:
        using Term = typename TermMap::value_type;

        Polynomial() = default;
        Polynomial(std::initializer_list<Term> source) {
            for (const Term& term : source) {
                data[term.first] = term.second;
            }
            trimZeroes();
        }

        Polynomial(Monomial m) : Polynomial{{std::move(m), 1}} {}

        Polynomial(FieldElement f) : Polynomial{{Monomial(), std::move(f)}} {}

        Polynomial(Term t) : Polynomial{{std::move(t)}} {}

        typename TermMap::iterator begin() {
            return data.begin();
        };

        typename TermMap::const_iterator begin() const {
            return data.cbegin();
        };

        typename TermMap::iterator end() {
            return data.end();
        };

        typename TermMap::const_iterator end() const {
            return data.cend();
        };

        typename TermMap::reverse_iterator rbegin() {
            return data.rbegin();
        };

        typename TermMap::const_reverse_iterator rbegin() const {
            return data.crbegin();
        };

        typename TermMap::reverse_iterator rend() {
            return data.rend();
        };

        typename TermMap::const_reverse_iterator rend() const {
            return data.crend();
        };

        const Term& leadingTerm() const {
            assert(!data.empty());
            return *rbegin();
        }

        Polynomial& operator+=(const Polynomial& other) {
            for (const Term& term : other.data) {
                data[term.first] += term.second;
            }
            trimZeroes();
            return *this;
        }

        friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
            Polynomial ret(lhs);
            ret += rhs;
            return ret;
        }

        Polynomial& operator-=(const Polynomial& other) {
            for (const Term& term : other.data) {
                data[term.first] -= term.second;
            }
            trimZeroes();
            return *this;
        }

        friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
            Polynomial ret(lhs);
            ret -= rhs;
            return ret;
        }

        // It's intentional!
        Polynomial& operator*=(const Polynomial& other) {
            *this = (*this) * other;
            return *this;
        }

        friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
            Polynomial res;
            for (const Term& termLeft : lhs.data) {
                for (const Term& termRight : rhs.data) {
                    res.data[termLeft.first * termRight.first] += termLeft.second * termRight.second;
                }
            }
            res.trimZeroes();
            return res;
        }

        Polynomial& operator/=(const FieldElement& f) {
            for (const Term& term : data) {
                data[term.first] /= f;
            }
            return *this;
        }

        friend Polynomial operator/(const Polynomial& lhs, const FieldElement& f) {
            Polynomial ret(lhs);
            ret /= f;
            return ret;
        }

        static const Monomial& getMonomial(const Term& pair) {
            return pair.first;
        }

        static const FieldElement& getCoefficient(const Term& pair) {
            return pair.second;
        }

        static FieldElement& getCoefficient(Term& pair) {
            return pair.second;
        }

        friend std::size_t hash_value(const Polynomial& p) {
            std::size_t seed = 0;
            for (const Term& term : p) {
                boost::hash_combine(seed, term);
            }
            return seed;
        }

        friend bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
            return lhs.data == rhs.data;
        }

        friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
            for (auto termIterator = p.data.begin(); termIterator != p.data.end(); ++termIterator) {
                if (termIterator != p.data.begin()) {
                    os << " + ";
                }

                if (getCoefficient(*termIterator) != FieldElement(1) || getMonomial(*termIterator) == Monomial()) {
                    os << termIterator->second;
                }
                os << termIterator->first;
            }
            return os;
        }
     private:
        TermMap data;

        void trimZeroes() {
            for (auto iter = data.cbegin(); iter != data.cend(); (getCoefficient(*iter) == 0 ? data.erase(iter++) : ++iter));
        }
    };

    template <typename FieldElement, typename OrderType>
    using PolynomialSet = std::unordered_set<Polynomial<FieldElement, OrderType>, boost::hash<Polynomial<FieldElement, OrderType>>>;
}

namespace boost {
    template <typename IntType>
    size_t hash_value(const boost::rational<IntType>& coeff)
    {
        size_t seed = 0;
        boost::hash_combine(seed, coeff.numerator());
        boost::hash_combine(seed, coeff.denominator());
        return seed;
    }
}
#endif //GROEBNER_POLYNOMIAL_H
