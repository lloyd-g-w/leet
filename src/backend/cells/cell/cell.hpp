#pragma once

#include <memory>

#include "../common.hpp"

namespace std_cells {

// Custom types and enums

enum cell_type_t {
    UNKNOWN,
    STRING,
    NUMBER,
    FLOAT,
    OPERATOR,
    FUNCTION,
    CELL_REFERENCE
};

// Main class
class cell {
  public:
    cell(str value = "") : m_raw_value(value) {}

    // Public methods
    // Setters
    void set_raw(str value);
    void set_type(cell_type_t type);

    // Getters
    const str &get_raw() const;
    str &get_raw_mut();
    const str &get_computed() const;
    const cell_type_t &get_type() const;

    // Bool methods
    bool is_empty() {
        return m_raw_value.empty();
    }

    bool is_computed() {
        return !m_computed_value.empty();
    }

    // User data methods
    bool has_user_data() {
        return m_user_data != nullptr;
    }
    template <typename T> void set_user_data(const T &data) {
        m_user_data = std::make_shared<T>(data);
    }
    template <typename T> const T &get_user_data() {
        return *std::static_pointer_cast<T>(m_user_data);
    }
    template <typename T> T &get_user_data_mut() {
        return *std::static_pointer_cast<T>(m_user_data);
    }

  private:
    friend class cell_grid;

    // Members
    str m_raw_value;
    str m_computed_value;
    cell_type_t m_type = UNKNOWN;
    std::shared_ptr<void> m_user_data = nullptr;

    // Main private methods
    void set_computed(str value);
};

}  // namespace std_cells
