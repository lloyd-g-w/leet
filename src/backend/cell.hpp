#pragma once

#include <string>
#include <vector>

namespace cell_space {

// Standard type definitions
typedef std::string str;
typedef std::vector<str> v_str;
typedef std::vector<int> v_int;

class cell {
  public:
    enum type {
        NUMBER,
        FLOAT,
        OPERATOR,
        FUNCTION,
        CELL_REFERENCE,
    };

    str raw_value;
    str computed_value = "";

  private:
    struct parsed_data {
        v_int nums;
    };
    parsed_data m_parsed_data;

  public:
    cell(std::string raw_value = "") : raw_value(raw_value) {}

    bool is_computed() {
        return !computed_value.empty();
    }

    enum parse_res_t {
        OK,
        INVALID_SYNTAX
    };

    parse_res_t parse();

    std::vector<std::string> tokenize();
};
}  // namespace cell_space
