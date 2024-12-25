#include "application.hpp"

app::app(std::string window_name, int window_width, int window_height,
         int grid_rows, int grid_cols) {
    int rows = 50;
    int cols = 50;
    p_window = new window(1920, 1080, "Holy Sheet");
    p_grid = new grid_space::grid(rows, cols, *p_window);
}

app::~app() {
    delete p_grid;
    delete p_window;
}

void app::run() {
    while (!p_window->should_close()) {
        p_window->run();
        p_grid->draw();
        p_window->render();
    }
}
