#pragma once

#include <string>

#include "../backend/grid/grid.hpp"
#include "gui/gui.hpp"
#include "window_renderer/window.hpp"

class app {
  public:
    app(std::string window_name, int window_width, int window_height,
        int grid_rows, int grid_cols);
    ~app();

    void run();

  private:
    window *p_window;
    gui::grid *p_grid_gui;
    std_cells::grid *p_cell_grid;
};
