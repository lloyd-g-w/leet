#include "application.hpp"

app::app(std::string window_name, int window_width, int window_height,
         int grid_rows, int grid_cols) {
    int rows = 100;
    int cols = 100;
    p_window = new window(1920, 1080, "Holy Sheet");
    p_cell_grid = new cells_std::cell_grid(rows, cols);
    p_grid_gui = new grid_gui(*p_cell_grid, *p_window);
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
