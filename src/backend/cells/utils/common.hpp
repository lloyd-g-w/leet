#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace std_cells {

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

struct pos_hash {
    std::size_t operator()(const pos &p) const {
        return std::hash<int>()(p.row) ^ std::hash<int>()(p.col);
    }
};

// Constants
const pos NULL_POS = {-1, -1};
}  // namespace std_cells
