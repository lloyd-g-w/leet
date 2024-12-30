#include "cell.hpp"

using namespace std_cells;

// -------------------- PUBLIC METHODS -------------------- //

// -------------------- SETTERS -------------------- //
void cell::set_raw(str value) {
    m_raw_value = value;
}

void cell::set_type(cell_type_t type) {
    m_type = type;
}

// -------------------- GETTERS -------------------- //
const str &cell::get_raw() {
    return m_raw_value;
}

str &cell::get_raw_mut() {
    return m_raw_value;
}

const str &cell::get_computed() {
    return m_computed_value;
}

const cell_type_t &cell::get_type() {
    return m_type;
}
