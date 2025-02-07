#pragma once

#include <cmath>
#include <memory>

#include "../common.hpp"

namespace std_cells {

// Main class
class cell {
  public:
    enum class type {
        NOT_SET,
        INT,
        DECIMAL,
        STRING,
        DATE,
    };

  private:
    friend class grid;

    struct data {
        str raw;
        str eval_str;
        double eval_float = INFINITY;
        type eval_type = type::NOT_SET;
    };

    // Members
    data m_data{};
    std::vector<pos> m_deps;
    std::shared_ptr<void> m_user_data = nullptr;

    // Public methods
    // Setters
    void set_raw(const str value);
    void set_str_eval(const str value);
    void set_float_eval(const double value);
    void set_type(const type type);
    void append_dep(const pos &dep);
    void set_deps(const std::vector<pos> &deps);

    // Getters
    const str &get_raw() const;
    const str &get_str_eval() const;
    const double &get_float_eval() const;
    const type &get_type() const;
    const std::vector<pos> &get_deps() const;

    // Clearing
    void clear_evaluated();
    void remove_dep(const pos &dep);
    void clear_deps();

    // Verification
    bool is_empty();
    bool is_dep(const pos &dep);
    bool is_evaluated();

    // User data methods
    bool has_user_data();
    template <typename T> void set_user_data(const T &data) {
        m_user_data = std::make_shared<T>(data);
    }
    template <typename T> const T &get_user_data() {
        return *std::static_pointer_cast<T>(m_user_data);
    }
};

}  // namespace std_cells
