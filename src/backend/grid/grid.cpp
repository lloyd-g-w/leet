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

void grid::delete_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    m_cells.erase(pos);
}

void grid::evaluate_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }

    auto &cell = get_cell_mut(pos);
    evaluate_cell(cell);
}

void grid::evaluate_cell(cell &cell) {
    try {
        const str raw = cell.get_raw();
        auto tokens = tokenize(raw);
        auto ast = parse_tokens(tokens);

        auto res = evaluate(ast, *this);

        cell.set_type(res.type);

        if (res.type == cell::type::INT || res.type == cell::type::DECIMAL) {
            cell.set_float_eval(res.number);
        } else {
            cell.set_str_eval(res.string);
        }

    } catch (exception::cell_exception &e) {
        if (cell.is_evaluated()) {
            cell.clear_evaluated();
        }
        std::cerr << "Error during cell evaluation: " << e.what() << std::endl;
    }
}

void grid::set_cell_raw(pos pos, const str &raw) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    auto &cell = m_cells[pos];
    if (cell.get_raw() == raw) {
        return;
    }
    cell.set_raw(raw);
    evaluate_cell(cell);
}

// ----------------------------- GETTERS -------------------- //

const cell::data &grid::get_cell_data(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    return m_cells[pos].m_data;
}

const int grid::get_rows() {
    return m_rows;
}

const int grid::get_cols() {
    return m_cols;
}

// ==================== VERIFICATION ==================== //

const bool grid::has_user_data(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    return m_cells[pos].has_user_data();
}

// -------------------- PRIVATE METHODS -------------------- //

cell &grid::get_cell_mut(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }
    return m_cells[pos];
}
