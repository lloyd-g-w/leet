#pragma once

#include <string>

#include "grid_gui/grid_gui.hpp"
#include "window_renderer/window.hpp"

class app {
  public:
    app(std::string window_name, int window_width, int window_height,
        int grid_rows, int grid_cols);
    ~app();

    void run();

  private:
    window *p_window;
    grid_gui *p_grid_gui;
    cells_std::cell_grid *p_cell_grid;
};
