#include "cell.hpp"
#include <cmath>
#include <sstream>

using namespace cell_space;

// Returns false if the cell is not a formula
cell::parse_res_t cell::parse() {
    std::istringstream iss(this->raw_value);
    computed_value.clear();

    char indicator;
    int num;
    int num2;
    char op;

    iss >> indicator;

    if (indicator != '=') {
        return OK;
    }

    iss >> num;
    iss >> op;
    iss >> num2;

    if (!op) {
        return INVALID_SYNTAX;
    }

    switch (op) {
        case '+': this->computed_value = std::to_string(num + num2); break;
        case '-': this->computed_value = std::to_string(num - num2); break;
        case '*': this->computed_value = std::to_string(num * num2); break;
        case '/': this->computed_value = std::to_string(num / num2); break;
        default: return INVALID_SYNTAX;
    }

    return OK;
}
