#include "application.hpp"

#define GRID_SHOW_FPS

app::app(std::string window_name, int window_width, int window_height,
         int grid_rows, int grid_cols) {
    int rows = 500;
    int cols = 500;
    p_window = new window(1920, 1080, "Holy Sheet");
    p_cell_grid = new std_cells::grid(rows, cols);
    p_grid_gui = new gui::grid(*p_cell_grid, *p_window);
}

app::~app() {
    delete p_grid_gui;
    delete p_cell_grid;
    delete p_window;
}

void app::run() {
    while (!p_window->should_close()) {
        p_window->run();
        p_grid_gui->draw();
        p_window->render();
    }
}
