#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

#include "cell.hpp"
#include "common.hpp"

namespace cells_std {

// Typedefs
typedef std::vector<std::optional<cell>> cell_row_t;
typedef std::vector<cell_row_t> cell_grid_t;
typedef std::unordered_map<str, cell> cell_map_t;

class cell_grid {
  public:
    cell_grid(int rows = 1000, int cols = 1000)
        : m_rows(rows), m_cols(cols), m_cells(rows, cell_row_t(cols)) {}

    // Main methods
    void create_cell(pos pos);
    void set_cell(pos pos, cell cell);
    void delete_cell(pos pos);

    const cell &get_cell(pos pos);
    cell &get_cell_mut(pos pos);
    const int &rows() {
        return m_rows;
    }
    const int &cols() {
        return m_cols;
    }

    // Helper methods
    static pos str_to_pos(str pos_str);
    static str pos_to_str(pos pos);
    static str num_to_alpha(int num);

  private:
    // Members
    int m_rows;
    int m_cols;
    cell_grid_t m_cells;

    // Helper methods
    bool is_set(pos pos);
    bool valid_pos(pos pos);
};

}  // namespace cells_std
