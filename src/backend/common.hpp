#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace std_cells {

class cell;
class grid;
struct eval_res;

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

// ==================== START EXCEPTIONS ==================== //
namespace exception {

// Base exception class
class cell_exception : public std::exception {
  public:
    explicit cell_exception(const str &message) : m_message(message) {};

    const char *what() const noexcept override {
        return m_message.c_str();
    }

  private:
    str m_message;
};

// Custom exceptions
class pos_out_of_range : public cell_exception {
  public:
    explicit pos_out_of_range(const str &message = "Position out of range")
        : cell_exception(message) {};
};

class cell_not_set : public cell_exception {
  public:
    explicit cell_not_set(const str &message = "Cell is not set")
        : cell_exception(message) {};
};

class cell_already_set : public cell_exception {
  public:
    explicit cell_already_set(const str &message = "Cell already set")
        : cell_exception(message) {};
};

class invalid_reference : public cell_exception {
  public:
    explicit invalid_reference(const str &message = "Invalid reference")
        : cell_exception(message) {};
};

class invalid_syntax : public cell_exception {
  public:
    explicit invalid_syntax(const str &message = "Invalid syntax")
        : cell_exception(message) {};
};
}  // namespace exception
}  // namespace std_cells
