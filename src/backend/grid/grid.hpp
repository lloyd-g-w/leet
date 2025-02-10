#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../cell/cell.hpp"
#include "../common.hpp"

namespace std_cells {

class grid {
  private:
    typedef std::unordered_map<pos, cell, pos_hash> cell_map;
    typedef std::unordered_map<pos, std::unordered_set<pos, pos_hash>, pos_hash>
        dep_map;

  public:
    grid(int rows = 1000, int cols = 1000)
        : m_rows(rows), m_cols(cols), m_cells() {}

    // Main methods
    // Creation, deletion, and evaluation
    void create_cell(pos pos);
    void delete_cell(pos pos);
    void evaluate_cell(pos pos);
    void evaluate_cell(cell &cell, pos caller_cell_pos);
    void set_cell_raw(pos pos, const str &raw);
    template <typename T> void set_cell_user_data(pos pos, const T &data) {
        if (!valid_pos(pos)) {
            throw exception::pos_out_of_range();
        }
        if (!is_set(pos)) {
            throw exception::cell_not_set();
        }
        m_cells[pos].set_user_data(data);
    }

    // Getters
    const cell::data &get_cell_data(str pos_str);
    const cell::data &get_cell_data(pos pos);
    template <typename T> const T &get_cell_user_data(pos pos) {
        if (!valid_pos(pos)) {
            throw exception::pos_out_of_range();
        }
        if (!is_set(pos)) {
            throw exception::cell_not_set();
        }
        return m_cells[pos].get_user_data<T>();
    }
    const int get_rows();
    const int get_cols();

    // Verification
    const bool has_user_data(pos pos);
    const bool is_set(pos pos);

    // Dependencies
    void add_dependency(pos parent_pos, pos dep_pos);
    void remove_dependency(pos parent_pos, pos dep_pos);
    void remove_dependencies(pos pos);
    void remove_parents(pos pos);
    std::vector<pos> get_dependencies(pos pos);

    // Public helper methods
    static pos str_to_pos(str pos_str);
    static str pos_to_str(pos pos);
    static str num_to_alpha(int num);

  private:
    // Members
    int m_rows;
    int m_cols;
    cell_map m_cells{};
    dep_map m_parents{};
    dep_map m_deps{};

    // Private helper methods
    bool valid_pos(pos pos);
    cell &get_cell_mut(pos pos);
};

}  // namespace std_cells
