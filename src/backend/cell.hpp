#pragma once

#include <string>
#include <vector>

class Cell {
  public:
    std::string buffer;
    std::string raw_value;
    std::string computed_value;

    bool is_computed = false;

  private:
    struct parsed_data {
        std::vector<int> nums;
    };
    parsed_data m_parsed_data;

  public:
    Cell(std::string raw_value = "") {
        if (raw_value != "") {
            this->raw_value = raw_value;
        }
    }

    bool parse();

    void compute();
};
