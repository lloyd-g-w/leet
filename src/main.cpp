#include "headers/grid.hpp"
#include "headers/window.hpp"

int main() {
    {
        Window window = Window();
        Grid grid = Grid(100, 100);

        while (!window.should_close()) {
            window.run();
            grid.draw();
            window.render();
        }
    }

    return 0;
}
