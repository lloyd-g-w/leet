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

void cell::set_computed(const str value) {
    m_computed_value = value;
}

void cell::clear_computed() {
    m_computed_value.clear();
}

// -------------------- GETTERS -------------------- //
const str &cell::get_raw() const {
    return m_raw_value;
}

str &cell::get_raw_mut() {
    return m_raw_value;
}

const str &cell::get_computed() const {
    return m_computed_value;
}

const cell_type_t &cell::get_type() const {
    return m_type;
}
