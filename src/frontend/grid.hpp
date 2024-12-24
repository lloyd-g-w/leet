#pragma once

#include "../backend/cell.hpp"
#include "colours.cpp"
#include "imgui.h"
#include "window.hpp"

#define NULL_POS {-1, -1}
static constexpr int COL_LABEL = -1;
static constexpr int ROW_LABEL = -1;

class Grid {
  public:
    struct Position {
        int row, col;

        // Overload == operator
        bool operator==(const Position &other) const {
            return row == other.row && col == other.col;
        }
    };

    struct Dimensions {
        float width, height;
    };

    static constexpr float DEFAULT_CELL_WIDTH = 75.0;
    static constexpr float DEFAULT_CELL_HEIGHT = 30.0;

    struct cell_data {
        Cell cell;

        bool is_editing = false;
        bool is_focused = false;
        Position position;
        Dimensions dimensions;
        std::string buffer;

        cell_data(Position pos,
                  Dimensions dim = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT})
            : position(pos), dimensions(dim) {}

        bool is_populated() {
            return !cell.raw_value.empty();
        }
    };

    Position active_cell = NULL_POS;

    Grid(int rows, int cols, Window &window);

  public:
    float next_populated_cell_dist(Position pos);

    void draw();
    void reset_for_next_draw() {
        num_rows_drawn = 0;
        num_cols_drawn = 0;
    };

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

    struct label_data {
        bool is_resizing = false;

        int index;

        Dimensions dimensions;

        label_data(int index,
                   Dimensions dim = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT})
            : index(index), dimensions(dim) {}
    };

    struct Labels {
        std::vector<label_data> data;

        label_data *at(int index) {
            return &data.at(index);
        }

        void push(label_data label) {
            data.push_back(label);
        }
    };

    Window &window;

    int rows, cols;

    Cells cells;
    Labels row_labels;
    Labels col_labels;

    int num_rows_drawn = 0;
    int num_cols_drawn = 0;

    // Utils
    void draw_col_labels();
    void draw_row_label();
    void draw_column();
    void draw_row();
    void draw_cell();
    void set_column_width(int col, float width);
    void set_row_height(int row, float height);
    std::string num_to_alpha(int num);
};
