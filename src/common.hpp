#pragma once

#include <string>
#include <vector>

namespace holy_space {
// Commonly used types
typedef std::string str;
typedef std::vector<str> v_str;
typedef std::vector<int> v_int;
typedef std::vector<float> v_float;
struct pos {
    int row, col;
    // Overload == operator
    bool operator==(const pos &other) const {
        return row == other.row && col == other.col;
    }
};
struct dims {
    float width, height;
};
}  // namespace holy_space
