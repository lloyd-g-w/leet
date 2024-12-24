#include "grid.hpp"

class menu {
  public:
    menu(grid_space::grid *grid) : grid(grid) {};

  private:
    grid_space::grid *grid;
};
