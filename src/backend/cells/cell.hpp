#pragma once

#include "common.hpp"

namespace cells_std {

class cell;

// Custom types and enums

enum cell_type_t {
    UNKNOWN,
    STRING,
    NUMBER,
    FLOAT,
    OPERATOR,
    FUNCTION,
    CELL_REFERENCE
};

class cell {
  public:
    cell(str value = "") : m_raw_value(value) {}

    // Public methods
    void set_raw(str value);
    void set_type(cell_type_t type);
    const str &get_raw();
    str &get_raw_mut();
    const str &get_computed();
    const cell_type_t &get_type();

  private:
    friend class cell_grid;

    // Members
    str m_raw_value;
    str m_computed_value = "";
    cell_type_t m_type = UNKNOWN;

    // Main private methods
    v_str tokenize();
    void parse();

    void set_computed(str value);
};

}  // namespace cells_std
