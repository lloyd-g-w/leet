#pragma once

#include <string>
#include <vector>

class Cell {
  public:
    enum parse_result {
        OK,
        INVALID_SYNTAX
    };
    std::string raw_value;
    std::string computed_value = "";

  private:
    struct parsed_data {
        std::vector<int> nums;
    };
    parsed_data m_parsed_data;

  public:
    Cell(std::string raw_value = "") : raw_value(raw_value) {}

    bool is_computed() {
        return !computed_value.empty();
    }

    parse_result parse();
};
