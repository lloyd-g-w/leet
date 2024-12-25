#pragma once

#include "../../backend/cell/cell.hpp"
#include "../../common/types.hpp"
#include "../utils/colours.hpp"
#include "../window/window.hpp"
#include "imgui.h"

namespace grid_space {
using namespace holy_space;

// Constants
#define NULL_POS {-1, -1}
const int COL_LABEL = -1;
const int ROW_LABEL = -1;
const float DEFAULT_CELL_WIDTH = 75.0;
const float DEFAULT_CELL_HEIGHT = 30.0;

// Structs and types
struct cell_data {
    cell_space::cell cell;

    bool is_editing = false;
    bool is_focused = false;
    pos position;
    dims dimensions;
    str buffer;

    cell_data(pos pos, dims dims = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT})
        : position(pos), dimensions(dims) {}

    bool is_populated() {
        return !cell.raw_value.empty();
    }
};

typedef std::vector<cell_data> v_cell_data;

struct cells {
    std::vector<v_cell_data> data;

    cell_data *at(int row, int col) {
        return &data.at(row).at(col);
    }

    void push_row(std::vector<cell_data> row) {
        data.push_back(row);
    }
};

typedef std::vector<cells> v_cells;

struct label_data {
    bool is_resizing = false;

    int index;

    dims dimensions;

    label_data(int index, dims dims = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT})
        : index(index), dimensions(dims) {}
};
typedef std::vector<label_data> v_label_data;

struct label_group {
    v_label_data data;

    label_data *at(int index) {
        return &data.at(index);
    }

    void push(label_data label) {
        data.push_back(label);
    }
};

// -------------------- GRID CLASS --------------------
class grid {
  public:
    // Variables
    pos m_active_cell = NULL_POS;

    // Main functions
    grid(int rows, int cols, window &window);
    void draw();
    void reset_for_next_draw() {
        m_num_rows_drawn = 0;
        m_num_cols_drawn = 0;
    };

    // Utils
    float next_populated_dist(pos pos);

  private:
    // Basic variables
    ImFont *m_font;
    colour_space::colour_group m_colours = colour_space::colour_group(
        {colour_space::colour_group::preset::HOLY_SHEET});
    int m_num_rows_drawn = 0;
    int m_num_cols_drawn = 0;

    // Window reference
    window &m_window;

    // Main data
    int m_rows, m_cols;
    cells m_cells;
    label_group m_row_labels;
    label_group m_col_labels;

    // Private utils
    void draw_col_labels();
    void draw_row_label();
    void draw_column();
    void draw_row();
    void draw_cell();
    void set_column_width(int col, float width);
    void set_row_height(int row, float height);
    std::string num_to_alpha(int num);
};
}  // namespace grid_space
