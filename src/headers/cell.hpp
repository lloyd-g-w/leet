#pragma once

#include <string>

#define MAX_CELL_SIZE 256

class Cell {
  public:
    std::string raw_value;
    std::string computed_value;
    int x, y;

    Cell(int x, int y, std::string raw_value = "") {
        this->x = x;
        this->y = y;

        this->raw_value.resize(MAX_CELL_SIZE);
        this->computed_value.resize(MAX_CELL_SIZE);

        if (raw_value != "") {
            this->raw_value = raw_value;
        }
    }
};
