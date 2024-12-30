#pragma once

#include "common.hpp"

namespace std_cells {

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
}  // namespace std_cells
