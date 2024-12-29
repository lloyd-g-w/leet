#pragma once

#include <string>
#include <vector>

namespace cells_std {

// Commonly used types
typedef std::string str;
typedef std::vector<str> v_str;
typedef std::vector<int> v_int;
typedef std::vector<float> v_float;

struct pos {
    int row, col;

    pos(int row = 0, int col = 0) : row(row), col(col) {}

    // Overload == operator
    bool operator==(const pos &other) const {
        return row == other.row && col == other.col;
    }
};
}  // namespace cells_std
