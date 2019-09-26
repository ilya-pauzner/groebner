#include "Rational.h"

namespace Groebner {
    void Rational::Normalise() {
        int64_t a = x;
        int64_t b = y;
        while (b) {
            a %= b;
            std::swap(a, b);
        }
        x /= a;
        y /= a;
        if (y < 0) {
            x *= -1;
            y *= -1;
        }
    }

    Rational::Rational(int64_t num, int64_t den = 1) : x(num), y(den) {
        Normalise();
    }

    int64_t Rational::Numerator() const {
        return x;
    }

    int64_t Rational::Denominator() const {
        return y;
    }

    Rational& Rational::operator+=(const Rational& w) {
        x = x * w.y + w.x * y;
        y *= w.y;
        Normalise();
        return *this;
    }

    Rational operator+(const Rational& u, const Rational& v) {
        auto res = u;
        res += v;
        return res;
    }

    Rational& Rational::operator-=(const Rational& w) {
        x = x * w.y - w.x * y;
        y *= w.y;
        Normalise();
        return *this;
    }
    
    Rational operator-(const Rational& u, const Rational& v) {
        auto res = u;
        res -= v;
        return res;
    }

    Rational& Rational::operator*=(const Rational& w) {
        x *= w.x;
        y *= w.y;
        Normalise();
        return *this;
    }

    Rational operator*(const Rational& u, const Rational& v) {
        auto res = u;
        res *= v;
        return res;
    }

    Rational& Rational::operator/=(const Rational& w) {
        x *= w.y;
        y *= w.x;
        Normalise();
        return *this;
    }

    Rational operator/(const Rational& u, const Rational& v) {
        auto res = u;
        res /= v;
        return res;
    }

    bool operator==(const Rational& u, const Rational& v) {
        return (u.Numerator() == v.Numerator()) && (u.Denominator() == v.Denominator());
    }

    bool operator!=(const Rational& u, const Rational& v) {
        return (u.Numerator() != v.Numerator()) || (u.Denominator() != v.Denominator());
    }

    Rational operator+(const Rational& u) {
        return u;
    }

    Rational operator-(const Rational& u) {
        return Rational(-u.Numerator(), u.Denominator());
    }
}