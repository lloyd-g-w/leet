#include "grid.hpp"
#include <iostream>

#include "../evaluator/evaluator.hpp"
#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

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

void grid::compute_cell(pos pos) {
    auto &cell = this->get_cell_mut(pos);

    try {
        const str raw = cell.get_raw();
        std::cout << "Computing cell " << pos_to_str(pos) << ": " << raw
                  << std::endl;
        auto tokens = tokenize(raw);
        auto ast = parse_tokens(tokens);

        str res = evaluate(ast, *this);
        cell.set_computed(res);
    } catch (exception::cell_exception &e) {
        if (cell.is_computed()) {
            cell.clear_computed();
        }
        std::cerr << "Error computing cell " << pos_to_str(pos) << ": "
                  << e.what() << std::endl;
    }
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
