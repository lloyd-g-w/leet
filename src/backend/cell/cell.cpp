#include "cell.hpp"

using namespace std_cells;

// ==================== SETTERS ==================== //
void cell::set_raw(const str value) {
    m_data.raw = value;
}

void cell::set_str_eval(const str value) {
    m_data.eval_str = value;
}

void cell::set_float_eval(const double value) {
    m_data.eval_float = value;
}

void cell::set_type(const cell::type type) {
    m_data.eval_type = type;
}

void cell::append_dep(const pos &dep) {
    m_deps.push_back(dep);
}

void cell::set_deps(const std::vector<pos> &deps) {
    m_deps = deps;
}

// ==================== GETTERS ==================== //
const str &cell::get_raw() const {
    return m_data.raw;
}

const str &cell::get_str_eval() const {
    return m_data.eval_str;
}

const double &cell::get_float_eval() const {
    return m_data.eval_float;
}

const cell::type &cell::get_type() const {
    return m_data.eval_type;
}

const std::vector<pos> &cell::get_deps() const {
    return m_deps;
}

// ==================== CLEARING ==================== //

void cell::clear_evaluated() {
    m_data.eval_str.clear();
    m_data.eval_float = INFINITY;
    m_data.eval_type = type::NOT_SET;
}

void cell::remove_dep(const pos &dep) {
    int index = -1;
    for (int i = 0; i < m_deps.size(); i++) {
        if (m_deps[i] == dep) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        m_deps.erase(m_deps.begin() + index);
    }
}

void cell::clear_deps() {
    m_deps.clear();
}

// ==================== VERIFICATION ==================== //
bool cell::is_empty() {
    return m_data.raw.empty();
}

bool cell::is_dep(const pos &dep) {
    for (const auto &d : m_deps) {
        if (d == dep) {
            return true;
        }
    }
    return false;
}

bool cell::is_evaluated() {
    return m_data.eval_type != type::NOT_SET;
}

// ==================== USER DATA ==================== //
bool cell::has_user_data() {
    return m_user_data != nullptr;
}
