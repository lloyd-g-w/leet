#pragma once

#include <vector>

#include "../backend/cell.hpp"
#include "colours.cpp"
#include "window.hpp"

class Grid {
  public:
    struct Position {
        int x, y;

        // Overload == operator
        bool operator==(const Position &other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Dimensions {
        float width, height;
    };

    static constexpr float DEFAULT_CELL_WIDTH = 75.0;
    static constexpr float DEFAULT_CELL_HEIGHT = 20.0;

    struct cell_data {
        Cell cell;

        bool is_active;
        Position position;
        Dimensions dimensions;
        std::string buffer;

        cell_data(Position pos,
                  Dimensions dim = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT},
                  Cell cell = Cell(), bool active = false)
            : position(pos), dimensions(dim), cell(cell), is_active(active) {}
    };

    Position active_cell;

    Grid(int rows, int cols, Window &window);

  public:
    void draw();
    ImFont *jetbrains_mono;
    Colours colours = Colours();

  private:
    struct Cells {
        std::vector<std::vector<cell_data>> data;

        cell_data *at(int row, int col) {
            return &data.at(row).at(col);
        }

        void push_row(std::vector<cell_data> row) {
            data.push_back(row);
        }
    };

    int num_rows_drawn = 0;
    int num_cols_drawn = 0;
    int rows, cols;
    Cells cells;
    Window &window;

    void draw_col_labels();
    void draw_row_label();
    void draw_column();
    void draw_row();
    void draw_cell();
};
