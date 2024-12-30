#include "grid.hpp"

using namespace std_cells;

// -------------------- PUBLIC METHODS -------------------- //

// Converts a string like "AB23" to a position
pos grid::str_to_pos(str pos_str) {
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

// Converts a position to a string like "AB23"
str grid::pos_to_str(pos pos) {
    return num_to_alpha(pos.row + 1) + std::to_string(pos.col + 1);
}

// Converts a number to an Excel-like column name
str grid::num_to_alpha(int num) {
    str res;
    while (num > 0) {
        num--;  // Adjust for 1-based indexing
        res.insert(res.begin(), 'A' + (num % 26));
        num /= 26;
    }
    return res;
}

// -------------------- PRIVATE METHODS -------------------- //

bool grid::valid_pos(pos pos) {
    if (pos.row < 0 || pos.row >= m_rows || pos.col < 0 || pos.col >= m_cols) {
        return false;
    }
    return true;
}

bool grid::is_set(pos pos) {
    return m_cells.contains(pos);
}
