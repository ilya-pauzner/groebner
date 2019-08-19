#include "test.h"
#include <sstream>

Groebner::PolynomialSet<boost::rational<long long>, Groebner::LexOrder> GenerateCyclicFamily(size_t n) {
    std::vector<Groebner::Polynomial<boost::rational<long long>, Groebner::LexOrder> > powerFamily(n + 1);
    for (size_t index = 1; index < powerFamily.size(); ++index) {
        for (size_t variable = 0; variable < n; ++variable) {
            Groebner::Monomial::DegreeContainer summand(variable, 0);
            summand.push_back(index);
            powerFamily[index] += Groebner::Monomial(summand);
        }
    }

    std::vector<Groebner::Polynomial<boost::rational<long long>, Groebner::LexOrder> > symmetricFamily(n + 1);
    symmetricFamily[0] = boost::rational<long long>(1);
    for (size_t k = 1; k < symmetricFamily.size(); ++k) {
        for (size_t i = 1; i <= k; ++i) {
            auto summand = symmetricFamily[k - i] * powerFamily[i];
            if (i % 2 == 1) {
                symmetricFamily[k] += summand;
            } else {
                symmetricFamily[k] -= summand;
            }
        }
        symmetricFamily[k] /= boost::rational<long long>(k);
    }
    Groebner::PolynomialSet<boost::rational<long long>, Groebner::LexOrder> answer;
    if (n % 2 == 0) {
        symmetricFamily[n] += boost::rational<long long>(1);
    } else {
        symmetricFamily[n] -= boost::rational<long long>(1);
    }
    for (size_t index = 1; index < symmetricFamily.size(); ++index) {
        answer.insert(symmetricFamily[index]);
    }
    return answer;
}

int main() {
    Groebner::test_all();
    std::cout << std::string(80, '=') << std::endl;
    for (size_t i = 1; i <= 10; ++i) {
        auto family = GenerateCyclicFamily(i);
        std::ostringstream os;
        os << "Groebner for " << i << " calculation";
        {
            Groebner::Timer t(os.str());
            Groebner::DoBuhberger(&family);
        }
        for (const auto& elem : family) {
            std::cout << elem << std::endl;
        }
        std::cout << std::endl;
    }
}