#include "grid.hpp"

using namespace std_cells;

// -------------------- PUBLIC METHODS -------------------- //

// -------------------- CREATION & DELETION -------------------- //
void grid::create_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (is_set(pos)) {
        throw exception::cell_already_set();
    }
    m_cells[pos] = cell();
}

void grid::set_cell(pos pos, cell cell) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    m_cells[pos] = cell;
}

void grid::delete_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    m_cells.erase(pos);
}

// ----------------------------- GETTERS -------------------- //
const cell &grid::get_cell(pos pos) {
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    return m_cells[pos];
};

cell &grid::get_cell_mut(pos pos) {
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    return m_cells[pos];
};

const int grid::get_rows() {
    return m_rows;
}

const int grid::get_cols() {
    return m_cols;
}

// -------------------- PRIVATE METHODS -------------------- //
