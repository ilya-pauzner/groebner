#ifndef GROEBNER_HELPERS_H
#define GROEBNER_HELPERS_H

#include "monomial.h"
#include <ctime>

namespace Groebner {
    Monomial lcm(const Monomial&, const Monomial&);
    class Timer {
     public:
        Timer(std::string);
        ~Timer();
     private:
        std::string message;
        clock_t t0;
    };
}

#endif //GROEBNER_HELPERS_H
