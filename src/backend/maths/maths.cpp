#include "maths.hpp"
#include <cmath>

namespace lm {

// ==================== BASIC OPERATIONS ==================== //

double add(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

double mul(double a, double b) {
    return a * b;
}

double div(double a, double b) {
    return a / b;
}

double pow(double a, double b) {
    return std::pow(a, b);
}

double sqrt(double a) {
    return std::sqrt(a);
}

double abs(double a) {
    return std::abs(a);
}

double mod(double a, double b) {
    return std::fmod(a, b);
}

double min(double a, double b) {
    return std::min(a, b);
}

double max(double a, double b) {
    return std::max(a, b);
}

double ceil(double a) {
    return std::ceil(a);
}

double floor(double a) {
    return std::floor(a);
}

// ==================== VARIADIC OPERATIONS ==================== //

double sum(std::vector<double> args) {
    double sum = 0;
    for (auto &i : args) {
        sum += i;
    }
    return sum;
}

double product(std::vector<double> args) {
    double product = 1;
    for (auto &i : args) {
        product *= i;
    }
    return product;
}

};  // namespace lm
