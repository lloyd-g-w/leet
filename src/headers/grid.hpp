#pragma once

#include <vector>

#include "cell.hpp"
#include "window.hpp"

class Grid {
  public:
    Grid(int rows, int cols, Window &window);

  public:
    void draw();

  private:
    int rows, cols;
    std::vector<std::vector<Cell>> cells;
    Window &window;
};
