#include "cell.hpp"
#include <cmath>
#include <sstream>

// Returns false if the cell is not a formula
bool Cell::parse() {
    std::istringstream iss(this->raw_value);

    char indicator;
    int num;
    int num2;
    char op;

    iss >> indicator;

    if (indicator != '=') {
        return false;
    }

    iss >> num;
    iss >> op;
    iss >> num2;

    if (!op) {
        this->computed_value = "err";
        this->is_computed = true;
        return true;
    }

    switch (op) {
        case '+': this->computed_value = std::to_string(num + num2); break;
        case '-': this->computed_value = std::to_string(num - num2); break;
        case '*': this->computed_value = std::to_string(num * num2); break;
        case '/': this->computed_value = std::to_string(num / num2); break;
        default: this->computed_value = "err"; break;
    }

    this->is_computed = true;
    return true;
}
