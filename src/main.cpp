#include "frontend/grid.hpp"
#include "frontend/window.hpp"

int main() {
    {
        Window window = Window(1920, 1080, "Holy Sheet");
        grid_space::grid grid = grid_space::grid(30, 30, window);

        while (!window.should_close()) {
            window.run();
            grid.draw();
            window.render();
        }
    }

    return 0;
}
