#pragma once
#include <vector>

namespace lm {

// ==================== BASIC OPERATIONS ==================== //
double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double div(double a, double b);
double pow(double a, double b);
double sqrt(double a);
double log(double a, double b);
double ln(double a);
double log10(double a);
double exp(double a);
double abs(double a);
double mod(double a, double b);

double min(double a, double b);
double max(double a, double b);
double ceil(double a);
double floor(double a);

// ==================== VARIADIC OPERATIONS ==================== //
double sum(std::vector<double> args);
double product(std::vector<double> args);

};  // namespace lm
