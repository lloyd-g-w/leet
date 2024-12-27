#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace cell_space {

// Classes
class cell;
class cell_grid;

// Standard type definitions
typedef std::string str;
typedef std::vector<str> v_str;
typedef std::vector<int> v_int;
typedef std::unordered_map<str, cell> cell_map;

class cell {
  public:
    // Constructor
    cell(str raw_value = "") : m_raw_value(raw_value) {}

    // Public methods
    void set_raw_val(str raw_val) {
        m_raw_value = raw_val;
    }

    const str get_raw_val() {
        return m_raw_value;
    }

    const str get_computed_val() {
        if (!is_computed()) {
            compute();
        }
        return m_computed_value;
    }

  private:
    friend class cell_grid;

    // Types/Enums
    enum cell_res_t {
        OK,
        INVALID_SYNTAX,
        INVALID_REFERENCE,
    };

    enum cell_type_t {
        NUMBER,
        FLOAT,
        OPERATOR,
        FUNCTION,
        CELL_REFERENCE
    };

    // Members
    str m_raw_value;
    str m_computed_value;

    // Main private methods
    void compute();
    v_str tokenize();
    cell_res_t parse();
    cell_res_t evaluate();

    // Helper methods
    bool is_computed() {
        return !m_computed_value.empty();
    }
};

class cell_group {
  public:
    // Main methods
    const cell get_cell(str pos_str) {
        if (!cell_exists(pos_str)) {
            return cell();
        }
        return m_cells[pos_str];
    };

    void create_cell(str pos_str) {
        if (!cell_exists(pos_str)) {
            return;
        }
    }

    void set_cell(str pos_str, cell cell) {
        m_cells[pos_str] = cell;
    }

    void delete_cell(str pos_str) {}

    // Helper methods
    bool cell_exists(str pos_str) {
        if (m_cells.find(pos_str) == m_cells.end()) {
            return false;
        }
        return true;
    }

  private:
    // Members
    cell_map m_cells;
};
}  // namespace cell_space
