#pragma once

#include <string>

#include "grid/grid.hpp"
#include "window/window.hpp"

class app {
  public:
    app(std::string window_name, int window_width, int window_height,
        int grid_rows, int grid_cols);
    ~app();

    void run();

  private:
    window *p_window;
    grid_space::grid *p_grid;
};
