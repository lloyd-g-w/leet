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

    try {
        const str raw = cell.get_raw();
        std::cout << "Evaluating cell " << pos_to_str(pos) << ": " << raw
                  << std::endl;
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
        if (cell.is_computed()) {
            cell.clear_evaluated();
        }
        std::cerr << "Error computing cell " << pos_to_str(pos) << ": "
                  << e.what() << std::endl;
    }
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
