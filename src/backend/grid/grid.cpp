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
    evaluate_cell(cell, pos);
}

void grid::evaluate_cell(cell &cell, pos caller_cell_pos) {
    try {
        std::cout << "Evaluating cell " << caller_cell_pos.row << " "
                  << caller_cell_pos.col << std::endl;

        const str raw = cell.get_raw();
        auto tokens = tokenize(raw);
        auto ast = parse_tokens(tokens);

        // To do - make this not segfault
        /*this->remove_parents(caller_cell_pos);*/

        auto res = evaluate(ast, *this, caller_cell_pos);

        cell.set_type(res.type);
        if (res.type == cell::type::INT || res.type == cell::type::DECIMAL) {
            cell.set_float_eval(res.number);
        } else {
            cell.set_str_eval(res.string);
        }

        for (const auto &dep_pos : m_deps[caller_cell_pos]) {
            evaluate_cell(m_cells[dep_pos], dep_pos);
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
    evaluate_cell(cell, pos);
}

// ----------------------------- GETTERS -------------------- //

const cell::data &grid::get_cell_data(str pos_str) {
    return get_cell_data(str_to_pos(pos_str));
}

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

// ==================== DEPENDENCIES ==================== //

void grid::add_dependency(pos parent_pos, pos dep_pos) {
    if (!valid_pos(parent_pos) || !valid_pos(dep_pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(parent_pos) || !is_set(dep_pos)) {
        throw exception::cell_not_set();
    }

    m_deps[parent_pos].insert(dep_pos);
    m_parents[dep_pos].insert(parent_pos);
}

void grid::remove_dependency(pos parent_pos, pos dep_pos) {
    if (!valid_pos(parent_pos) || !valid_pos(dep_pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(parent_pos) || !is_set(dep_pos)) {
        throw exception::cell_not_set();
    }

    m_deps[parent_pos].erase(dep_pos);
    m_parents[dep_pos].erase(parent_pos);
}

void grid::remove_dependencies(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }

    if (m_deps.find(pos) == m_deps.end()) {
        return;
    }

    for (const auto &dep_pos : m_deps[pos]) {
        m_parents[dep_pos].erase(pos);
    }

    m_deps.erase(pos);
}

void grid::remove_parents(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }

    if (m_parents.find(pos) == m_parents.end()) {
        return;
    }

    for (const auto &parent_pos : m_parents[pos]) {
        m_deps[parent_pos].erase(pos);
    }

    m_parents.erase(pos);
}

std::vector<pos> grid::get_dependencies(pos pos) {
    if (!valid_pos(pos)) {
        throw exception::pos_out_of_range();
    }
    if (!is_set(pos)) {
        throw exception::cell_not_set();
    }

    std::vector<struct pos> deps;

    if (m_deps.find(pos) == m_deps.end()) {
        return deps;
    }

    for (const auto &dep_pos : m_deps[pos]) {
        deps.push_back(dep_pos);
    }

    return deps;
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
