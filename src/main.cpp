#include "frontend/grid.hpp"
#include "frontend/window.hpp"

int main() {
    {
        Window window = Window();
        Grid grid = Grid(30, 15, window);

        while (!window.should_close()) {
            window.run();
            grid.draw();
            window.render();
        }
    }

    return 0;
}
