#ifndef GROEBNER_RATIONAL_H
#define GROEBNER_RATIONAL_H

#include <cstdint>
#include <algorithm>

namespace Groebner {
    class Rational {
     private:
        int64_t x;
        int64_t y;
        void Normalise();

     public:
        Rational() : x(0), y(1) {}
        Rational(int64_t, int64_t);

        int64_t Numerator() const;
        int64_t Denominator() const;

        Rational& operator+=(const Rational& other);
        friend Rational operator+(const Rational&, const Rational&);

        Rational& operator-=(const Rational& other);
        friend Rational operator-(const Rational&, const Rational&);

        Rational& operator*=(const Rational& other);
        friend Rational operator*(const Rational&, const Rational&);

        Rational& operator/=(const Rational& other);
        friend Rational operator/(const Rational&, const Rational&);

        friend bool operator==(const Rational&, const Rational&);
        friend bool operator!=(const Rational&, const Rational&);
    };
}
#endif //GROEBNER_RATIONAL_H
