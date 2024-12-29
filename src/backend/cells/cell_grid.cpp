#include "cell_grid.hpp"

#include "common.hpp"
#include "exceptions.hpp"

using namespace cells_std;

// -------------------- PUBLIC METHODS -------------------- //

// Main methods

const cell &cell_grid::get_cell(pos pos) {
    if (!is_set(pos)) {
        throw cells_std::cell_not_set();
    }
    str ref = pos_to_str(pos);
    return m_cells[ref];
};

cell &cell_grid::get_cell_mut(pos pos) {
    if (!is_set(pos)) {
        throw cells_std::cell_not_set();
    }
    str ref = pos_to_str(pos);
    return m_cells[ref];
};

void cell_grid::create_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw cells_std::pos_out_of_range();
    }
    if (is_set(pos)) {
        throw cells_std::cell_already_set();
    }
    str ref = pos_to_str(pos);
    m_cells[ref] = cell();
}

void cell_grid::set_cell(pos pos, cell cell) {
    if (!valid_pos(pos)) {
        throw cells_std::pos_out_of_range();
    }
    str ref = pos_to_str(pos);
    m_cells[ref] = cell;
}

void cell_grid::delete_cell(pos pos) {
    if (!valid_pos(pos)) {
        throw cells_std::pos_out_of_range();
    }
    str ref = pos_to_str(pos);
    m_cells.erase(ref);
}

// Helper methods
pos cell_grid::str_to_pos(str pos_str) {
    str letters;
    str numbers;

    // Separate letters and numbers
    for (char c : pos_str) {
        if (std::isalpha(c)) {
            letters += c;
        } else if (std::isdigit(c)) {
            numbers += c;
        }
    }

    // Convert letters to number
    int letter_num = 0;
    for (char c : letters) {
        letter_num *= 26;
        letter_num += (c - 'A');
    }
    return pos(letter_num, numbers.empty() ? 0 : std::stoi(numbers));
}

str cell_grid::pos_to_str(pos pos) {
    return num_to_alpha(pos.row + 1) + std::to_string(pos.col + 1);
}

str cell_grid::num_to_alpha(int num) {
    str res;
    while (num > 0) {
        num--;  // Adjust for 1-based indexing
        res.insert(res.begin(), 'A' + (num % 26));
        num /= 26;
    }
    return res;
}

// -------------------- PRIVATE METHODS -------------------- //

// Main methods
bool cell_grid::valid_pos(pos pos) {
    if (pos.row < 0 || pos.row >= m_rows || pos.col < 0 || pos.col >= m_cols) {
        return false;
    }
    return true;
}

// Helper methods
bool cell_grid::is_set(pos pos) {
    if (!valid_pos(pos)) {
        throw cells_std::pos_out_of_range();
    }
    str ref = pos_to_str(pos);
    if(m_cells.find(ref) != m_cells.end()) {
        return true;
    }

    return false;
}
