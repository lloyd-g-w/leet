#pragma once

#include <deque>

#include "../common.hpp"

namespace std_cells {

struct tok {
    enum class type {
        PUNCTUATION,
        OPERATOR,
        INT,
        FLOAT,
        STRING,
        FUNCTION,
        CELL_REFERENCE
    } type;
    str value;
};

typedef std::deque<tok> q_tok;
q_tok tokenize(str string);
}  // namespace std_cells
