#ifndef GROEBNER_POLYNOMIAL_H
#define GROEBNER_POLYNOMIAL_H
#include "monomial_order.h"
#include <algorithm>
#include <map>
#include <vector>

namespace Groebner {
    template <MonomialOrder* Order>
    class OrderAdaptor {
     public:
        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return Order->isLess(lhs, rhs);
        }
    };

    template <typename FieldElement, MonomialOrder* Order>
    class Polynomial {
        using TermMap = std::map<Monomial, FieldElement, OrderAdaptor<Order>>;
     private:
        TermMap data;

        void trimZeroes() {
            std::vector<Monomial> zeroMonomials;
            for (const Term& term : data) {
                if (term.second == 0) {
                    zeroMonomials.push_back(term.first);
                }
            }
            for (const Monomial& monomial : zeroMonomials) {
                data.erase(monomial);
            }
        }
     public:
        using Term = std::pair<Monomial, FieldElement>;

        Polynomial() = default;
        Polynomial(std::initializer_list<Term> source) {
            for (const Term& term : source) {
                data[term.first] = term.second;
            }
            trimZeroes();
        }

        Polynomial(const Monomial& m) {
            *this = Polynomial({{m, 1}});
        }

        Polynomial(const FieldElement& f) {
            *this = Polynomial({{Monomial(), f}});
        }

        typename TermMap::iterator begin() {
            return data.begin();
        };

        typename TermMap::const_iterator cbegin() const {
            return data.cbegin();
        };

        typename TermMap::iterator end() {
            return data.end();
        };

        typename TermMap::const_iterator cend() const {
            return data.cend();
        };

        typename TermMap::reverse_iterator rbegin() {
            return data.rbegin();
        };

        typename TermMap::const_reverse_iterator crbegin() const {
            return data.crbegin();
        };

        typename TermMap::reverse_iterator rend() {
            return data.rend();
        };

        typename TermMap::const_reverse_iterator crend() const {
            return data.crend();
        };

        std::vector<Term> dump() const {
            std::vector<Term> result;
            std::copy(data.begin(), data.end(), std::back_inserter(result));
            return result;
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
                os << termIterator->second << termIterator->first;
            }
        }
    };

}
#endif //GROEBNER_POLYNOMIAL_H
